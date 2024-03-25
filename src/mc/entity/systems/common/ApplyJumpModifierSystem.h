#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/common/wrapper/Exclude.h"
#include "mc/common/wrapper/Include.h"
#include "mc/common/wrapper/ViewT.h"
#include "mc/world/components/FlagComponent.h"

class ApplyJumpModifierSystem {
public:
    // prevent constructor by default
    ApplyJumpModifierSystem& operator=(ApplyJumpModifierSystem const&);
    ApplyJumpModifierSystem(ApplyJumpModifierSystem const&);
    ApplyJumpModifierSystem();

public:
    // NOLINTBEGIN
    // symbol:
    // ?_tickApplyJumpModifierSystem@ApplyJumpModifierSystem@@SAXV?$ViewT@VStrictEntityContext@@U?$Include@V?$FlagComponent@UOnGroundFlag@@@@V?$FlagComponent@UPowerJump@@@@@@U?$Exclude@V?$FlagComponent@UMobIsJumpingFlag@@@@@@$$CBUTriggerJumpRequestComponent@@$$CBUActorRotationComponent@@$$CBUMovementAttributesComponent@@$$CBUMobEffectsComponent@@$$CBUMobTravelComponent@@UJumpPendingScaleComponent@@UStateVectorComponent@@@@@Z
    MCAPI static void _tickApplyJumpModifierSystem(
        class ViewT<
            class StrictEntityContext,
            struct Include<class FlagComponent<struct OnGroundFlag>, class FlagComponent<struct PowerJump>>,
            struct Exclude<class FlagComponent<struct MobIsJumpingFlag>>,
            struct TriggerJumpRequestComponent const,
            struct ActorRotationComponent const,
            struct MovementAttributesComponent const,
            struct MobEffectsComponent const,
            struct MobTravelComponent const,
            struct JumpPendingScaleComponent,
            struct StateVectorComponent> view
    );

    // symbol: ?createApplyJumpModifierSystem@ApplyJumpModifierSystem@@SA?AUTickingSystemWithInfo@@XZ
    MCAPI static struct TickingSystemWithInfo createApplyJumpModifierSystem();

    // symbol:
    // ?tickApplyJumpModifierSystem@ApplyJumpModifierSystem@@SAXAEBVStrictEntityContext@@AEBUTriggerJumpRequestComponent@@AEBUActorRotationComponent@@AEBUMovementAttributesComponent@@AEBUMobEffectsComponent@@AEBUMobTravelComponent@@AEAUJumpPendingScaleComponent@@AEAUStateVectorComponent@@@Z
    MCAPI static void
    tickApplyJumpModifierSystem(class StrictEntityContext const&, struct TriggerJumpRequestComponent const&, struct ActorRotationComponent const&, struct MovementAttributesComponent const&, struct MobEffectsComponent const&, struct MobTravelComponent const&, struct JumpPendingScaleComponent&, struct StateVectorComponent&);

    // NOLINTEND
};
