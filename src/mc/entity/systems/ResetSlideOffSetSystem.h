#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/common/wrapper/Exclude.h"
#include "mc/common/wrapper/ViewT.h"
#include "mc/world/components/FlagComponent.h"

class ResetSlideOffSetSystem {
public:
    // prevent constructor by default
    ResetSlideOffSetSystem& operator=(ResetSlideOffSetSystem const&);
    ResetSlideOffSetSystem(ResetSlideOffSetSystem const&);
    ResetSlideOffSetSystem();

public:
    // NOLINTBEGIN
    // symbol: ?createResetSlideOffSetSystem@ResetSlideOffSetSystem@@SA?AUTickingSystemWithInfo@@XZ
    MCAPI static struct TickingSystemWithInfo createResetSlideOffSetSystem();

    // symbol:
    // ?tickResetSlideOffSetSystem@ResetSlideOffSetSystem@@SAXV?$ViewT@VStrictEntityContext@@U?$Exclude@V?$FlagComponent@UAutoStepRequestFlag@@@@@@$$CBUMoveRequestComponent@@USlideOffsetComponent@@@@@Z
    MCAPI static void tickResetSlideOffSetSystem(class ViewT<
                                                 class StrictEntityContext,
                                                 struct Exclude<class FlagComponent<struct AutoStepRequestFlag>>,
                                                 struct MoveRequestComponent const,
                                                 struct SlideOffsetComponent> view);

    // NOLINTEND
};
