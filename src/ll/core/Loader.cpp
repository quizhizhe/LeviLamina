#include "ll/core/Loader.h"

#include <filesystem>
#include <string>
#include <vector>

#include "ll/api/utils/DbgHelper.h"
#include "ll/api/utils/StringUtils.h"
#include "ll/api/utils/WinHelper.h"

#include "ll/api/LLAPI.h"
#include "ll/api/LoggerAPI.h"
#include "ll/api/i18n/I18nAPI.h"
#include "ll/api/perm/PermissionAPI.h"


#include "ll/core/Config.h"
#include "ll/core/LeviLamina.h"
#include "ll/core/PluginManager.h"
#include "ll/core/Version.h"

#include "windows.h"

using namespace ll::StringUtils;

inline std::vector<std::string> getPreloadList() {
    // Not load if is already loaded in preloader
    std::vector<std::string> preloadList{};

    if (std::filesystem::exists(std::filesystem::path(".\\plugins\\preload.conf"))) {
        std::ifstream preloadConf{".\\plugins\\preload.conf"};
        if (preloadConf) {
            std::string preloadName;
            while (getline(preloadConf, preloadName)) {
                if (preloadName.back() == '\n') preloadName.pop_back();
                if (preloadName.back() == '\r') preloadName.pop_back();

                if (preloadName.empty() || preloadName.front() == '#') continue;
                if (preloadName.find("LeviLamina.dll") != std::string::npos) continue;
                if (preloadName.find("LiteXLoader.dll") != std::string::npos) continue;

                preloadList.emplace_back(preloadName);
            }
            preloadConf.close();
        }
    }
    return preloadList;
}

void ll::LoadMain() {
    ll::logger.info(tr("ll.loader.loadMain.start"));

    // Load plugins
    int                      pluginCount = 0;
    std::vector<std::string> preloadList = getPreloadList();

    std::filesystem::directory_iterator ent("plugins");
    for (auto& file : ent) {

        if (!file.is_regular_file()) { continue; }
        std::filesystem::path const& path = file.path();

        auto ext = path.extension().u8string();

        // Check is dll
        if (ext != u8".dll") { continue; }

        // Avoid preloaded plugin
        auto pluginFileName = u8str2str(path.filename().u8string());
        bool loaded         = false;
        for (auto& p : preloadList)
            if (p.find(pluginFileName) != std::string::npos) {
                loaded = true;
                break;
            }
        if (loaded) continue;

        // Do load
        auto lib = LoadLibraryW(path.wstring().c_str());
        if (lib) {
            ++pluginCount;

            if (PluginManager::getPlugin(lib) == nullptr) {
                if (!ll::PluginManager::registerPlugin(lib, pluginFileName, pluginFileName, ll::Version(1, 0, 0), {})) {
                    ll::logger.error(tr("ll.pluginManager.error.failToRegisterPlugin", u8str2str(path.u8string())));
                    if (PluginManager::getPlugin(pluginFileName)) {
                        ll::logger.error(tr("ll.pluginManager.error.hasBeenRegistered", pluginFileName));
                    }
                }
            }
        } else {
            DWORD       lastError   = GetLastError();
            std::string fileVersion = GetFileVersionString(u8str2str(path.u8string()), true);
            std::string info        = pluginFileName;
            if (!fileVersion.empty()) { info += " [" + fileVersion + "]"; }
            ll::logger.error("Fail to load plugin <{}>!", info);
            ll::logger.error("Error: Code[{}] {}", lastError, GetLastErrorMessage(lastError));
        }
    }

    // TODO: call OnPostInit Event
    ll::logger.info(tr("ll.loader.loadMain.done", pluginCount));
}