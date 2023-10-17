#include "ll/api/event/player/PlayerJoinEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/ServerPlayer.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

namespace ll::event::player {

PlayerJoinEvent::PlayerJoinEvent(Player* player) : player(player) {}

LL_GETTER_IMPL(PlayerJoinEvent, Player*, player, getPlayer)

LL_AUTO_TYPED_INSTANCE_HOOK(
    PlayerJoinEventHook,
    ServerPlayer,
    HookPriority::Normal,
    "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    bool,
) {
    PlayerJoinEvent event(this);
    EventManager<PlayerJoinEvent>::fireEvent(event);
    if (event.isCancelled()) {
        return false;
    }
    return origin();
}

} // namespace ll::event::player

LL_EVENT_IMPL(player::PlayerJoinEvent)
