#include "ll/api/event/player/PlayerUseRespawnAnchorEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/BlockInstance.hpp"
#include "mc/BlockPos.hpp"
#include "mc/BlockSource.hpp"
#include "mc/Level.hpp"
#include "mc/Player.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

namespace ll::event::player {

PlayerUseRespawnAnchorEvent::PlayerUseRespawnAnchorEvent(Player* player, BlockInstance const& blockInstance)
: player(player), blockInstance(blockInstance) {}

LL_GETTER_IMPL(PlayerUseRespawnAnchorEvent, Player*, player, getPlayer)
LL_GETTER_IMPL(PlayerUseRespawnAnchorEvent, BlockInstance, blockInstance, getBlockInstance)

using EventManager = EventManager<PlayerUseRespawnAnchorEvent>;

LL_AUTO_TYPED_INSTANCE_HOOK(
    PlayerUseRespawnAnchorEventHook,
    Player,
    HookPriority::Normal,
    "?trySetSpawn@RespawnAnchorBlock@@CA_NAEAVPlayer@@AEBVBlockPos@@AEAVBlockSource@@AEAVLevel@@@Z",
    bool,
    BlockPos*    bp,
    BlockSource* bs,
    Level*       a4
) {
    Player*       player = this;
    BlockInstance block  = Level::getBlockInstance(bp, bs);

    PlayerUseRespawnAnchorEvent event(player, block);
    EventManager::fireEvent(event);

    if (event.isCancelled()) {
        return false;
    }

    return origin(bp, bs, a4);
}

} // namespace ll::event::player

LL_EVENT_IMPL(player::PlayerUseRespawnAnchorEvent)
