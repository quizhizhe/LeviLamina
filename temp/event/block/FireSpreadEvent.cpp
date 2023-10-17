#include "ll/api/event/block/FireSpreadEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/BlockSource.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

namespace ll::event::block {

FireSpreadEvent::FireSpreadEvent(BlockPos const& target, int dimensionId) : target(target), dimensionId(dimensionId) {}

LL_GETTER_IMPL(FireSpreadEvent, BlockPos, target, getTarget)
LL_GETTER_IMPL(FireSpreadEvent, int, dimensionId, getDimensionId)

bool onFireSpread_OnPlace = false;

LL_AUTO_INSTANCE_HOOK(
    FireSpreadEventHook1,
    HookPriority::Normal,
    "?onPlace@FireBlock@@UEBAXAEAVBlockSource@@AEBVBlockPos@@@Z",
    void,
    BlockSource* blockSource,
    BlockPos*    blockPos
) {
    onFireSpread_OnPlace = true;
    origin(blockSource, blockPos);
    onFireSpread_OnPlace = false;
}

LL_AUTO_INSTANCE_HOOK(
    FireSpreadEventHook2,
    HookPriority::Normal,
    "?mayPlace@FireBlock@@UEBA_NAEAVBlockSource@@AEBVBlockPos@@@Z",
    bool,
    BlockSource* blockSource,
    BlockPos*    blockPos
) {
    using EventManager = EventManager<FireSpreadEvent>;

    auto rtn = origin(blockSource, blockPos);
    if (!onFireSpread_OnPlace || !rtn)
        return rtn;

    FireSpreadEvent event(*blockPos, blockSource->getDimensionId());
    EventManager::fireEvent(event);
    if (event.isCancelled()) {
        return false;
    }
    return rtn;
}

} // namespace ll::event::block

LL_EVENT_IMPL(block::FireSpreadEvent)
