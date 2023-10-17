#include "ll/api/event/player/PlayerRespawnEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/Player.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

namespace ll::event::player {

PlayerRespawnEvent::PlayerRespawnEvent(Player* player) : player(player) {}

LL_GETTER_IMPL(PlayerRespawnEvent, Player*, player, getPlayer)

using EventManager = EventManager<PlayerRespawnEvent>;

LL_AUTO_TYPED_INSTANCE_HOOK(PlayerRespawnEventHook, Player, HookPriority::Normal, "?respawn@Player@@UEAAXXZ", void) {
    // If the player returns from TheEnd, the health will > 0.
    if (getHealth() <= 0) {
        if (!this) {
            return;
        }
        PlayerRespawnEvent event(this);
        EventManager::fireEvent(event);
    }
    origin();
}

} // namespace ll::event::player

LL_EVENT_IMPL(player::PlayerRespawnEvent)
