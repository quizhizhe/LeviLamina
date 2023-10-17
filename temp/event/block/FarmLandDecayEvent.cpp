#include "ll/api/event/block/FarmLandDecayEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/Level.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

namespace ll::event::block {

FarmLandDecayEvent::FarmLandDecayEvent(BlockInstance const& blockInstance, Actor* actor)
: blockInstance(blockInstance), actor(actor) {}

LL_GETTER_IMPL(FarmLandDecayEvent, BlockInstance, blockInstance, getBlockInstance)
LL_GETTER_IMPL(FarmLandDecayEvent, Actor*, actor, getActor)

LL_AUTO_INSTANCE_HOOK(
    FarmLandDecayEventHook,
    HookPriority::Normal,
    "?transformOnFall@FarmBlock@@UEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@M@Z",
    void,
    BlockSource* blockSource,
    BlockPos*    blockPos,
    Actor*       actor,
    float        a5
) {
    using EventManager = EventManager<FarmLandDecayEvent>;

    auto               blockInstance = Level::getBlockInstance(blockPos, blockSource);
    FarmLandDecayEvent event(blockInstance, actor);
    EventManager::fireEvent(event);
    if (event.isCancelled()) {
        return;
    }

    return origin(blockSource, blockPos, actor, a5);
}

} // namespace ll::event::block

LL_EVENT_IMPL(block::FarmLandDecayEvent)
