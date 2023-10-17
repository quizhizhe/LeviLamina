#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/BlockInstance.hpp"
#include "mc/Player.hpp"


namespace ll::event::player {

/**
 * @brief Player attack block event.
 * @details This event is called when a player attacks a block.
 */
class PlayerAttackBlockEvent : public Event<PlayerAttackBlockEvent>, public Cancellable {
    LL_RO_MEMBER(Player*, player, getPlayer);
    LL_RO_MEMBER(BlockInstance, blockInstance, getBlockInstance);
    LL_RO_MEMBER(ItemStack*, itemStack, getItemStack);

    explicit PlayerAttackBlockEvent(Player* player, BlockInstance const& blockInstance, ItemStack* itemStack);

    LL_DELETE_COPY(PlayerAttackBlockEvent);
    LL_DELETE_MOVE(PlayerAttackBlockEvent);
};

} // namespace ll::event::player
