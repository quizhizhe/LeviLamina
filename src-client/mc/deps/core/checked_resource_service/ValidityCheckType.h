#pragma once

#include "mc/_HeaderOutputPredefine.h"

namespace mce {

enum class ValidityCheckType : uchar {
    Increment = 0, // 递增检查计数
    Immutable = 1, // 不修改计数, 直接以当前 mIsValid 为准
};

}
