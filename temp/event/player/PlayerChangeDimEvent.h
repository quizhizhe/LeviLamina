#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/Player.hpp"

namespace ll::event::player {

/**
 * @brief Player change dimension event.
 * @details This event is called when a player changes dimension.
 */
class PlayerChangeDimEvent : public Event<PlayerChangeDimEvent>, public Cancellable {
    LL_RO_MEMBER(Player*, player, getPlayer);
    LL_RO_MEMBER(int, toDimensionId, getToDimensionId);

    explicit PlayerChangeDimEvent(Player* player, int toDimensionId);

    LL_DELETE_COPY(PlayerChangeDimEvent);
    LL_DELETE_MOVE(PlayerChangeDimEvent);
};

} // namespace ll::event::player
