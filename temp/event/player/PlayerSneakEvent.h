#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/Player.hpp"

namespace ll::event::player {

/**
 * @brief Player sneak event.
 * @details This event is called when a player toggles their sneaking state.
 */
class PlayerSneakEvent : public Event<PlayerSneakEvent> {
    LL_RO_MEMBER(Player*, player, getPlayer);
    LL_RW_MEMBER(bool, sneaking, isSneaking, setSneaking);

    explicit PlayerSneakEvent(Player* player, bool sneaking);

    LL_DELETE_COPY(PlayerSneakEvent);
    LL_DELETE_MOVE(PlayerSneakEvent);
};

} // namespace ll::event::player
