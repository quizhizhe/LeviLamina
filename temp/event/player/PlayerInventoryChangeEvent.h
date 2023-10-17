#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/ItemStack.hpp"
#include "mc/Player.hpp"

namespace ll::event::player {

/**
 * @brief Player inventory change event.
 * @details This event is called when a player's inventory changes.
 */
class PlayerInventoryChangeEvent : public Event<PlayerInventoryChangeEvent> {
public:
    LL_RO_MEMBER(Player*, player, getPlayer);
    LL_RW_MEMBER(int, slot, getSlot, setSlot);
    LL_RO_MEMBER(ItemStack*, previousItemStack, getPreviousItemStack);
    LL_RO_MEMBER(ItemStack*, newItemStack, getNewItemStack);

    PlayerInventoryChangeEvent(Player* player, int slot, ItemStack* oldItem, ItemStack* newItem);

    LL_DELETE_COPY(PlayerInventoryChangeEvent);
    LL_DELETE_MOVE(PlayerInventoryChangeEvent);
};

} // namespace ll::event::player
