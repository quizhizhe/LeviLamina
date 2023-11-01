#include "ll/api/utils/WinHelper.h"

#include <string>

#include "ll/api/utils/StringUtils.h"

#include "ll/api/i18n/I18nAPI.h"

#include "ll/core/Config.h"
#include "ll/core/LeviLamina.h"

#include "Psapi.h"
#include "Windows.h"

using namespace ll::string_utils;

std::string GetLastErrorMessage(DWORD errorMessageId) {
    if (errorMessageId == 0) return "";

    LPWSTR message_buffer = nullptr;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        errorMessageId,
        MAKELANGID(0x09, SUBLANG_DEFAULT),
        (LPWSTR)&message_buffer,
        0,
        nullptr
    );
    std::string res = wstr2str(std::wstring(message_buffer));
    LocalFree(message_buffer);
    return res;
}

std::string GetLastErrorMessage() {
    DWORD error_message_id = ::GetLastError();
    if (error_message_id == 0) return "";

    LPWSTR message_buffer = nullptr;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        error_message_id,
        MAKELANGID(0x09, SUBLANG_DEFAULT),
        (LPWSTR)&message_buffer,
        0,
        nullptr
    );
    std::string res = wstr2str(std::wstring(message_buffer));
    LocalFree(message_buffer);
    return res;
}

std::string GetSystemLocaleName() {
    wchar_t buf[256] = {0};
    auto    lcid     = GetSystemDefaultLCID();
    GetSystemDefaultLocaleName(buf, (int)lcid);
    auto str = wstr2str(buf);
    std::replace(str.begin(), str.end(), '-', '_');
    return str;
}

inline bool isWine() {
    HMODULE ntdll = GetModuleHandle(L"ntdll.dll");
    if (!ntdll) return false;
    auto funcWineGetVersion = GetProcAddress(ntdll, "wine_get_version");
    if (funcWineGetVersion) return true;
    else return false;
}

bool IsWineEnvironment() {
    static bool result = isWine();
    return result;
}

#define IN_RANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define GET_BYTE(x)       (GET_BITS((x)[0]) << 4 | GET_BITS((x)[1]))
#define GET_BITS(x)                                                                                                    \
    (IN_RANGE(((x) & (~0x20)), 'A', 'F') ? (((x) & (~0x20)) - 'A' + 0xa) : (IN_RANGE(x, '0', '9') ? (x) - '0' : 0))

inline DWORD_PTR GetProcessBaseAddress(DWORD processId) {
    DWORD_PTR baseAddress   = 0;
    HANDLE    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    HMODULE*  moduleArray;
    LPBYTE    moduleArrayBytes;
    DWORD     bytesRequired = 0;

    if (!processHandle) return baseAddress;

    if (!EnumProcessModules(processHandle, nullptr, 0, &bytesRequired) || !bytesRequired) goto Ret;

    moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);
    if (!moduleArrayBytes) { goto Ret; }

    moduleArray = (HMODULE*)moduleArrayBytes;
    if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired)) {
        baseAddress = (DWORD_PTR)moduleArray[0];
    }
    LocalFree(moduleArrayBytes);

Ret:
    CloseHandle(processHandle);
    return baseAddress;
}

inline std::vector<std::string> split(std::string str, std::string const& pattern) {
    std::string::size_type   pos;
    std::vector<std::string> result;
    str         += pattern;
    size_t size  = str.size();
    for (size_t i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

uintptr_t FindSig(char const* szSignature) {
    char const*            pattern    = szSignature;
    uintptr_t              firstMatch = 0;
    DWORD                  processId  = GetCurrentProcessId();
    static const uintptr_t rangeStart = GetProcessBaseAddress(processId);
    static MODULEINFO      miModInfo;
    static bool            init = false;

    if (!init) {
        init = true;
        GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
    }

    static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;
    BYTE                   patByte  = GET_BYTE(pattern);
    char const*            oldPat   = pattern;

    for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
        if (!*pattern) return firstMatch;

        while (*(PBYTE)pattern == ' ') pattern++;

        if (!*pattern) return firstMatch;

        if (oldPat != pattern) {
            oldPat = pattern;
            if (*(PBYTE)pattern != '\?') patByte = GET_BYTE(pattern);
        }
        if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte) {
            if (!firstMatch) firstMatch = pCur;

            if (!pattern[2] || !pattern[1]) return firstMatch;
            pattern += 2;
        } else {
            pattern    = szSignature;
            firstMatch = 0;
        }
    }
    return 0;
}

#undef IN_RANGE
#undef GET_BYTE
#undef GET_BITS
