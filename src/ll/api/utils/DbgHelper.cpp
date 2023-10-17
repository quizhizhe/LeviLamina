﻿#include "ll/api/utils/DbgHelper.h"

#include <map>
#include <string>
#include <thread>

#include "ll/api/LoggerAPI.h"
#include "ll/api/utils/StringUtils.h"
#include "ll/api/utils/WinHelper.h"

#include "ll/core/Config.h"
#include "ll/core/LeviLamina.h"

#include "Psapi.h"
#include "windows.h"

#include "dbghelp.h"

#pragma comment(lib, "dbghelp.lib")

using namespace ll::StringUtils;

using ll::logger;

#define DBGHELP_TRANSLATE_TCHAR

/////////////////////////////////// Symbol Loader ///////////////////////////////////

std::set<std::wstring> loadedSymbolDir;
bool                   symbolsLoaded = false;

void FindSymbols(std::wstring& collection, std::string const& nowPath, bool recursion = false) {
    std::filesystem::directory_iterator list(nowPath);
    for (auto& it : list) {
        if (it.is_directory() && recursion) {
            FindSymbols(collection, it.path().string(), recursion);
        } else if (it.path().extension() == ".pdb") {
            std::filesystem::path dir     = std::filesystem::canonical(it.path());
            std::wstring          dirPath = dir.remove_filename().native();

            if (loadedSymbolDir.find(dirPath) == loadedSymbolDir.end()) {
                collection += L";" + dirPath.substr(0, dirPath.size() - 1);
                loadedSymbolDir.insert(dirPath);
            }
        }
    }
}

bool LoadSymbols() {
    if (symbolsLoaded) return true;

    loadedSymbolDir.clear();
    std::wstring symbolPath{L"srv*C:\\Windows\\symbols*http://msdl.microsoft.com/download/symbols"};
    FindSymbols(symbolPath, ".", false);
    FindSymbols(symbolPath, ".\\plugins", true);

    if (!SymInitializeW(GetCurrentProcess(), symbolPath.c_str(), TRUE)) {
        logger.warn("Fail to load Symbol Files! Error Code: {}", GetLastError());
        return false;
    }
    loadedSymbolDir.clear();
    symbolsLoaded = true;
    return true;
}

bool CleanupSymbols() {
    symbolsLoaded = false;
    return SymCleanup(GetCurrentProcess());
}


/////////////////////////////////// Symbol Reader ///////////////////////////////////

std::map<DWORD, std::wstring> moduleMap;

PSYMBOL_INFOW GetSymbolInfo(HANDLE hProcess, void* address) {
    auto* pSymbol         = (SYMBOL_INFOW*)new char[sizeof(SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(TCHAR)];
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFOW);
    pSymbol->MaxNameLen   = MAX_SYM_NAME;

    DWORD64 displacement = 0;
    if (SymFromAddrW(hProcess, (DWORD64)address, &displacement, pSymbol)) return pSymbol;
    else return nullptr;
}

void CleanSymbolInfo(PSYMBOL_INFOW pSymbol) { delete[] ((char*)pSymbol); }

BOOL CALLBACK EnumerateModuleCallBack(PCTSTR ModuleName, DWORD64 ModuleBase, ULONG /*ModuleSize*/, PVOID UserContext) {
    auto*   pModuleMap               = (std::map<DWORD, std::wstring>*)UserContext;
    LPCWSTR name                     = wcsrchr(ModuleName, TEXT('\\')) + 1;
    (*pModuleMap)[(DWORD)ModuleBase] = name;
    return TRUE;
}

bool CreateModuleMap(HANDLE hProcess) {
    if (!EnumerateLoadedModulesW64(hProcess, EnumerateModuleCallBack, &moduleMap)) {
        logger.error("Fail to Enumerate loaded modules! Error Code: {}", GetLastError());
        return false;
    }
    return true;
}

std::wstring MapModuleFromAddr(HANDLE hProcess, void* address) {
    return moduleMap[(DWORD)SymGetModuleBase64(hProcess, (DWORD64)address)];
}

/////////////////////////////////// Print Traceback ///////////////////////////////////

#define MACHINE_TYPE IMAGE_FILE_MACHINE_AMD64

bool PrintCurrentStackTraceback(PEXCEPTION_POINTERS e, Logger* l) {
    Logger& debugLogger = l ? *l : logger;
    if (!ll::globalConfig.enableErrorStackTraceback) {
        logger.error("* Stack traceback is disabled by config file.");
        return true;
    }

    HANDLE hProcess    = GetCurrentProcess();
    HANDLE hThread     = GetCurrentThread();
    DWORD  threadId    = GetCurrentThreadId();
    bool   cacheSymbol = ll::globalConfig.cacheErrorStackTracebackSymbol;
    bool   res         = false;

    std::thread printThread([e, hProcess, hThread, threadId, cacheSymbol, &res, &debugLogger]() {
        // Set global SEH-Exception handler
        _set_se_translator(seh_exception::TranslateSEHtoCE);

        LoadSymbols();
        CreateModuleMap(hProcess);

        PCONTEXT pContext;
        CONTEXT  context{};
        if (e) pContext = e->ContextRecord;
        else {
            HANDLE ohThread = OpenThread(THREAD_ALL_ACCESS, TRUE, threadId);
            if (ohThread == nullptr) {
                logger.error("Fail to Open Thread! Error Code: {}", GetLastError());
                return;
            }
            context.ContextFlags = CONTEXT_FULL;
            if (!GetThreadContext(ohThread, &context)) {
                logger.error("Fail to Get Context! Error Code: {}", GetLastError());
                return;
            }
            pContext = &context;
        }

        STACKFRAME64 stackFrame     = {};
        stackFrame.AddrPC.Mode      = AddrModeFlat;
        stackFrame.AddrPC.Offset    = pContext->Rip;
        stackFrame.AddrStack.Mode   = AddrModeFlat;
        stackFrame.AddrStack.Offset = pContext->Rsp;
        stackFrame.AddrFrame.Mode   = AddrModeFlat;
        stackFrame.AddrFrame.Offset = pContext->Rbp;

        bool skipingPrintFunctionsStack = true;

        while (StackWalk64(
            MACHINE_TYPE,
            hProcess,
            hThread,
            &stackFrame,
            pContext,
            nullptr,
            SymFunctionTableAccess64,
            SymGetModuleBase64,
            nullptr
        )) {
            DWORD64 address = stackFrame.AddrPC.Offset;

            // Function
            PSYMBOL_INFOW info;
            auto          moduleName = wstr2str(MapModuleFromAddr(hProcess, (void*)address));
            info                     = GetSymbolInfo(hProcess, (void*)address);
            if (info) {
                if (skipingPrintFunctionsStack) {
                    if (wcscmp(info->Name, L"PrintCurrentStackTraceback") == 0) // Skiping these print functions' stack
                        skipingPrintFunctionsStack = false;
                    continue;
                }

                debugLogger.error("at {} (0x{:X})  [{}]", wstr2str(info->Name), info->Address, moduleName);

                // Line
                DWORD            displacement = 0;
                IMAGEHLP_LINEW64 line{};
                line.SizeOfStruct = sizeof(IMAGEHLP_LINEW64);

                if (SymGetLineFromAddrW64(hProcess, address, &displacement, &line))
                    debugLogger.error("(in File {} : Line {})", wstr2str(line.FileName), line.LineNumber);
                delete info;
            } else debugLogger.error("at ???????? (0x{:X})  [{}]", address, moduleName);
        }
        std::cout << std::endl;

        if (!cacheSymbol) CleanupSymbols();
        res = true;
    });

    printThread.join();
    return res;
}

/////////////////////////////////// Debug Helper ///////////////////////////////////

HMODULE GetCallerModule(ulong FramesToSkip) {
    static const int maxFrameCount = 1;
    void*            frames[maxFrameCount];
    int              frameCount = CaptureStackBackTrace(FramesToSkip + 2, maxFrameCount, frames, nullptr);

    std::string name;
    if (0 < frameCount) {
        HMODULE hModule;
        GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCWSTR)frames[0],
            &hModule
        );
        return hModule;
    }
    return HMODULE();
}

std::string GetCallerModuleFileName(ulong FramesToSkip) { return GetModuleName(GetCallerModule(FramesToSkip)); }

bool GetFileVersion(const wchar_t* filePath, ushort* ver1, ushort* ver2, ushort* ver3, ushort* ver4, uint* flag) {

    DWORD dwLen = GetFileVersionInfoSizeW(filePath, nullptr);
    if (!dwLen) { return false; }
    auto* pBlock = new (std::nothrow) wchar_t[dwLen];
    if (nullptr == pBlock) { return false; }
    if (!GetFileVersionInfoW(filePath, 0, dwLen, pBlock)) {
        delete[] pBlock;
        return false;
    }

    VS_FIXEDFILEINFO* lpBuffer = nullptr;
    uint              uLen     = 0;
    if (!VerQueryValueW(pBlock, L"\\", (void**)&lpBuffer, &uLen)) {
        delete[] pBlock;
        return false;
    }

    if (ver1) *ver1 = (lpBuffer->dwFileVersionMS >> 16) & 0x0000FFFF;
    if (ver2) *ver2 = lpBuffer->dwFileVersionMS & 0x0000FFFF;
    if (ver3) *ver3 = (lpBuffer->dwFileVersionLS >> 16) & 0x0000FFFF;
    if (ver4) *ver4 = lpBuffer->dwFileVersionLS & 0x0000FFFF;
    if (flag) *flag = lpBuffer->dwFileFlags;

    delete[] pBlock;
    return true;
}

inline std::string
VersionToString(ushort major_ver, ushort minor_ver, ushort revision_ver, ushort build_ver, uint flag = 0) {
    std::string flagStr;
    if (flag & VS_FF_DEBUG) flagStr += " DEBUG";
    if (flag & VS_FF_PRERELEASE) flagStr += " PRERELEASE";
    if (flag & VS_FF_PATCHED) flagStr += " PATCHED";
    if (flag & VS_FF_PRIVATEBUILD) flagStr += " PRIVATEBUILD";
    if (flag & VS_FF_INFOINFERRED) flagStr += " INFOINFERRED";
    if (flag & VS_FF_SPECIALBUILD) flagStr += " SPECIALBUILD";
    return fmt::format("{}.{}.{}.{}{}", major_ver, minor_ver, revision_ver, build_ver, flagStr);
}

std::string GetFileVersionString(HMODULE hModule, bool includeFlag) {
    ushort  major_ver, minor_ver, revision_ver, build_ver;
    uint    flag;
    wchar_t filePath[MAX_PATH] = {0};
    GetModuleFileNameEx(GetCurrentProcess(), hModule, filePath, MAX_PATH);
    if (GetFileVersion(filePath, &major_ver, &minor_ver, &revision_ver, &build_ver, &flag)) {
        return VersionToString(major_ver, minor_ver, revision_ver, build_ver, includeFlag ? flag : 0);
    }
    return "";
}

std::string GetFileVersionString(std::string const& filePath, bool includeFlag) {
    ushort       major_ver, minor_ver, revision_ver, build_ver;
    uint         flag;
    std::wstring wFilePath = str2wstr(filePath);
    if (GetFileVersion(wFilePath.c_str(), &major_ver, &minor_ver, &revision_ver, &build_ver, &flag)) {
        return VersionToString(major_ver, minor_ver, revision_ver, build_ver, includeFlag ? flag : 0);
    }
    return "";
}
