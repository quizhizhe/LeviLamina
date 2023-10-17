#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/Actor.hpp"
#include "mc/BlockInstance.hpp"
#include "mc/BlockPos.hpp"

namespace ll::event::block {
/**
 * @brief Liquid spread event.
 */
class LiquidSpreadEvent : public Event<LiquidSpreadEvent>, public Cancellable {
    LL_RO_MEMBER(BlockInstance, blockInstance, getBlockInstance);
    LL_RO_MEMBER(BlockPos, target, getTarget);
    LL_RO_MEMBER(int, dimensionId, getDimensionId);

    LiquidSpreadEvent(BlockInstance const& blockInstance, BlockPos const& target, int dimensionId = -1);

    LL_DELETE_COPY(LiquidSpreadEvent);
    LL_DELETE_MOVE(LiquidSpreadEvent);
};
} // namespace ll::event::block
