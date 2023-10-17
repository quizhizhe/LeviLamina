#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/BlockInstance.hpp"

namespace ll::event::block {
/**
 * @brief Block changed event.
 */
class BlockChangedEvent : public Event<BlockChangedEvent>, public Cancellable {
    LL_RO_MEMBER(BlockInstance, previousBlockInstance, getPreviousBlockInstance);
    LL_RO_MEMBER(BlockInstance, newBlockInstance, getNewBlockInstance);

    BlockChangedEvent(BlockInstance const& previousBlockInstance, BlockInstance const& newBlockInstance);

    LL_DELETE_COPY(BlockChangedEvent);
    LL_DELETE_MOVE(BlockChangedEvent);
};
} // namespace ll::event::block
