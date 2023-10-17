#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/Actor.hpp"
#include "mc/BlockInstance.hpp"

namespace ll::event::entity {
/**
 * @brief Projectile hit block event.
 */
class ProjectileHitBlockEvent : public Event<ProjectileHitBlockEvent> {
    LL_RO_MEMBER(BlockInstance, blockInstance, getBlockInstance);
    LL_RO_MEMBER(Actor*, source, getSource);

    ProjectileHitBlockEvent(BlockInstance const& blockInstance, Actor* source);

    LL_DELETE_COPY(ProjectileHitBlockEvent);
    LL_DELETE_MOVE(ProjectileHitBlockEvent);
};
} // namespace ll::event::entity
