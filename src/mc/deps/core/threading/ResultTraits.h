#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/deps/core/threading/Async.h"
#include "mc/deps/core/threading/SharedAsync.h"

namespace Bedrock::Threading::AsyncResult {

template <typename T0>
struct ResultTraits {
    using Result = T0;
};

template <typename T0>
struct ResultTraits<::Bedrock::Threading::Async<T0>> {
    using Result = ::Bedrock::Threading::SharedAsync<T0>;
};

template <>
struct ResultTraits<void> {
    using Result = ::std::monostate;
};

} // namespace Bedrock::Threading::AsyncResult
