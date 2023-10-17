#include "ll/api/base/Global.h"
#include "ll/api/memory/Hook.h"
#include "mc/IResourcePackRepository.hpp"
#include "mc/Pack.hpp"
#include "mc/PackIdVersion.hpp"
#include "mc/PackInstance.hpp"
#include "mc/PackManifest.hpp"
#include "mc/PackSettings.hpp"
#include "mc/PackSettingsFactory.hpp"
#include "mc/ResourceLocation.hpp"
#include "mc/ResourcePack.hpp"
#include "mc/ResourcePackRepository.hpp"
#include "mc/ResourcePackStack.hpp"

namespace ModifyPack {
vector<PackIdVersion> PackListCache;
}

using namespace ll::memory;

LL_AUTO_STATIC_HOOK(
    MpHook1,
    HookPriority::Normal,
    "??0ResourcePack@@QEAA@AEAVPack@@@Z",
    ResourcePack*,
    ResourcePack* self,
    Pack*         a2
) {
    auto Pack     = origin(self, a2);
    auto manifest = &Pack->getManifest();
    if (manifest && manifest->getPackOrigin() == PackOrigin::PackOrigin_Dev) {
        ModifyPack::PackListCache.push_back(a2->getManifest().getIdentity());
    }
    return Pack;
}

LL_AUTO_STATIC_HOOK(
    MpHook2,
    HookPriority::Normal,
    "?deserialize@ResourcePackStack@@SA?AV?$unique_ptr@VResourcePackStack@@U?$default_delete@VResourcePackStack@@@std@@"
    "@std@@AEAV?$basic_istream@DU?$char_traits@D@std@@@3@AEBVIResourcePackRepository@@@Z",
    std::unique_ptr<ResourcePackStack>,
    int64                   a2,
    ResourcePackRepository* a3
) {
    auto Stack = origin(a2, a3);
    for (auto& id : ModifyPack::PackListCache) {
        auto  Pack            = a3->getResourcePackForPackId(id);
        auto& SettingsFactory = a3->getPackSettingsFactory();
        auto  settings        = SettingsFactory.getPackSettings(Pack->getManifest());
        char  gsl[16];
        auto  gslPath =
            LL_SYMBOL_CALL("??$?0PEAVResourcePack@@X@?$not_null@V?$NonOwnerPointer@VResourcePack@@@Bedrock@@@gsl@@QEAA@AEBV?$not_null@PEAVResourcePack@@@1@@Z", void*, char*, ResourcePack**)(
                gsl, &Pack
            );
        char size[480];
        auto Instance =
            LL_SYMBOL_CALL("??0PackInstance@@QEAA@V?$not_null@V?$NonOwnerPointer@VResourcePack@@@Bedrock@@@gsl@@H_NPEAVPackSettings@@@Z", PackInstance*, char*, void*, int, bool, PackSettings*)(
                size, gslPath, 0, false, settings
            );
        Stack->add(*Instance, *(IResourcePackRepository*)a3, 0);
    }
    ModifyPack::PackListCache.clear();
    return Stack;
}
