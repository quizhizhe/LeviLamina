#include <unordered_map>

#include "ll/api/memory/Hook.h"
#include "mc/PropertiesSettings.hpp"
#include "mc/SharedConstants.hpp"

#include "ll/core/Config.h"

#include <Windows.h>
using namespace ll;
using namespace ll::memory;

LL_AUTO_TYPED_INSTANCE_HOOK(
    UnBindDefaultPort,
    PropertiesSettings,
    HookPriority::Normal,
    "??0PropertiesSettings@@QEAA@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
    size_t,
    std::string const& file
) {
    auto out = origin(file);
    if (ll::globalConfig.enableUnoccupyPort19132) {
        // logger.warn("If you turn on this feature, your server will not be displayed on the LAN");
        DWORD v4Flag, v6Flag, _;
        VirtualProtect((void*)&SharedConstants::NetworkDefaultGamePort, 4, PAGE_READWRITE, &v4Flag);
        *(ushort*)&SharedConstants::NetworkDefaultGamePort = getServerPort();
        VirtualProtect((void*)&SharedConstants::NetworkDefaultGamePort, 4, v4Flag, &_);

        VirtualProtect((void*)&SharedConstants::NetworkDefaultGamePortv6, 4, PAGE_READWRITE, &v6Flag);
        *(ushort*)&SharedConstants::NetworkDefaultGamePortv6 = getServerPortv6();
        VirtualProtect((void*)&SharedConstants::NetworkDefaultGamePortv6, 4, v6Flag, &_);
    }
    return out;
}
