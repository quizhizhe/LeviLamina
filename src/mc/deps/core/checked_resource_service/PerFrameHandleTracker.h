#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/deps/core/checked_resource_service/ValidityCheckType.h"

namespace mce {

class PerFrameHandleTracker {
public:
    std::atomic<unsigned short> mCheckCount; // 0x0  (2 bytes)
    std::atomic<bool>           mIsValid;    // 0x2  (1 byte), 对齐后 size 0x4

public:
    std::tuple<bool, bool> Validate(ValidityCheckType validityCheckType) {
        bool checkResult = false;

        if (validityCheckType == ValidityCheckType::Increment) {
            checkResult = (++mCheckCount == 0);
        } else if (validityCheckType != ValidityCheckType::Immutable) {
            
        }

        return {checkResult, mIsValid.load()};
    }
};

} // namespace mce
