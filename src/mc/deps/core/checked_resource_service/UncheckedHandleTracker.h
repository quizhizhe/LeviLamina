#pragma once

#include "mc/_HeaderOutputPredefine.h"

namespace mce {

class UncheckedHandleTracker {
public:
    bool mIsValid; // +0x0

    UncheckedHandleTracker() : mIsValid(false) {}
};

} // namespace mce
