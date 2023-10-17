#include "ll/api/event/block/ContainerChangeEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/Level.hpp"
#include "mc/LevelContainerModel.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

namespace ll::event::block {

ContainerChangeEvent::ContainerChangeEvent(
    Player*              player,
    Actor*               actor,
    BlockInstance const& blockInstance,
    Container*           container,
    ItemStack*           previousItemStack,
    ItemStack*           newItemStack,
    int                  slot
)
: player(player), actor(actor), blockInstance(blockInstance), container(container), slot(slot),
  previousItemStack(previousItemStack), newItemStack(newItemStack) {}

LL_GETTER_IMPL(ContainerChangeEvent, Player*, player, getPlayer)
LL_GETTER_IMPL(ContainerChangeEvent, Actor*, player, getActor)
LL_GETTER_IMPL(ContainerChangeEvent, BlockInstance, blockInstance, getBlockInstance)
LL_GETTER_IMPL(ContainerChangeEvent, Container*, container, getContainer)
LL_GETTER_IMPL(ContainerChangeEvent, int, slot, getSlot)
LL_GETTER_IMPL(ContainerChangeEvent, ItemStack*, previousItemStack, getPreviousItemStack)
LL_GETTER_IMPL(ContainerChangeEvent, ItemStack*, newItemStack, getNewItemStack)

LL_AUTO_TYPED_INSTANCE_HOOK(
    ContainerChangeEventHook,
    LevelContainerModel,
    HookPriority::Normal,
    "?_onItemChanged@LevelContainerModel@@MEAAXHAEBVItemStack@@0@Z",
    void,
    int        slotNumber,
    ItemStack* oldItem,
    ItemStack* newItem
) {
    using EventManager = EventManager<ContainerChangeEvent>;

    // IDA LevelContainerModel::LevelContainerModel
    auto player = dAccess<Player*>(reinterpret_cast<char*>(this), 208);

    if (player->hasOpenContainer()) {
        auto blockPos      = reinterpret_cast<BlockPos*>(reinterpret_cast<char*>(this) + 216);
        auto blockInstance = Level::getBlockInstance(blockPos, player->getDimensionId());

        ContainerChangeEvent event(
            player,
            this->getEntity(),
            blockInstance,
            blockInstance.getContainer(),
            oldItem,
            newItem,
            slotNumber + this->_getContainerOffset()
        );
        EventManager::fireEvent(event);

        return origin(slotNumber, oldItem, newItem);
    }
}

} // namespace ll::event::block

LL_EVENT_IMPL(block::ContainerChangeEvent)
