#include "ll/api/event/player/PlayerInventoryChangeEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/Player.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

namespace ll::event::player {

PlayerInventoryChangeEvent::PlayerInventoryChangeEvent(Player* player, int slot, ItemStack* oldItem, ItemStack* newItem)
: player(player), slot(slot), previousItemStack(oldItem), newItemStack(newItem) {}

using EventManager = EventManager<PlayerInventoryChangeEvent>;

LL_AUTO_TYPED_INSTANCE_HOOK(
    PlayerInventoryChangeEventHook,
    Player,
    HookPriority::Normal,
    "?inventoryChanged@Player@@UEAAXAEAVContainer@@HAEBVItemStack@@1_N@Z",
    void,
    void*      container,
    int        slotNumber,
    ItemStack* oldItem,
    ItemStack* newItem,
    bool       is
) {
    if (this->isPlayer()) {
        PlayerInventoryChangeEvent event(this, slotNumber, oldItem, newItem);
        EventManager::fireEvent(event);
    }

    return origin(container, slotNumber, oldItem, newItem, is);
}

} // namespace ll::event::player

LL_EVENT_IMPL(player::PlayerInventoryChangeEvent)
