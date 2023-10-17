#pragma once

#include "ll/api/event/Event.h"
#include "ll/api/utils/AccessorMacro.h"
#include "mc/MobEffectInstance.hpp"
#include "mc/Player.hpp"


namespace ll::event::player {

/**
 * @brief Player effect change event.
 * @details This event is called when a player's effect is added, removed, or updated.
 */
class PlayerEffectChangeEvent : public Event<PlayerEffectChangeEvent> {
public:
    enum class ActionType : schar { Add, Remove, Update };

    LL_RO_MEMBER(Player*, player, getPlayer);
    LL_RO_MEMBER(MobEffectInstance*, effect, getEffect);
    LL_RO_MEMBER(ActionType, action, getAction);

    explicit PlayerEffectChangeEvent(Player* player, MobEffectInstance* effect, ActionType action);

    LL_DELETE_COPY(PlayerEffectChangeEvent);
    LL_DELETE_MOVE(PlayerEffectChangeEvent);
};

} // namespace ll::event::player
