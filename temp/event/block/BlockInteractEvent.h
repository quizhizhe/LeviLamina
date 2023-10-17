#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/BlockInstance.hpp"
#include "mc/Player.hpp"

namespace ll::event::block {
/**
 * @brief Block interact event.
 */
class BlockInteractEvent : public Event<BlockInteractEvent>, public Cancellable {
    LL_RO_MEMBER(BlockInstance, blockInstance, getBlockInstance);
    LL_RO_MEMBER(Player*, player, getPlayer);

    BlockInteractEvent(BlockInstance const& blockInstance, Player* player);

    LL_DELETE_COPY(BlockInteractEvent);
    LL_DELETE_MOVE(BlockInteractEvent);
};
} // namespace ll::event::block
