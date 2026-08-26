#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/deps/core/threading/IAsyncResult.h"

namespace Bedrock::Threading {

template <typename T>
class Async {
public:
    using Handle            = std::shared_ptr<Bedrock::Threading::IAsyncResult<T>>;
    using Result            = T;
    using CompletionHandler = typename Bedrock::Threading::IAsyncResult<T>::CompletionHandler;

public:
    // member variables
    // NOLINTBEGIN
    Handle mResult;
    // NOLINTEND

public:
    Async() = default;
    Async(std::nullptr_t) {}
    explicit Async(Handle result) : mResult(std::move(result)) {}
    Async(Async const&)            = default;
    Async(Async&&) noexcept        = default;
    Async& operator=(Async const&) = default;
    Async& operator=(Async&&) noexcept = default;
    Async& operator=(std::nullptr_t) {
        mResult.reset();
        return *this;
    }

    bool operator==(std::nullptr_t) const noexcept { return mResult == nullptr; }
    bool operator!=(std::nullptr_t) const noexcept { return mResult != nullptr; }
    explicit operator bool() const noexcept { return mResult != nullptr; }

    Handle get() const { return mResult; }

    Bedrock::Threading::IAsyncResult<T>* operator->() const { return mResult.get(); }

    ::Bedrock::Threading::AsyncStatus getStatus() const { return mResult ? mResult->getStatus() : ::Bedrock::Threading::AsyncStatus::Completed; }

    ::std::error_code getError() const { return mResult ? mResult->getError() : ::std::error_code{}; }

    void cancel() const {
        if (mResult) {
            mResult->cancel();
        }
    }

    void addOnComplete(CompletionHandler handler) const {
        if (mResult) {
            mResult->addOnComplete(std::move(handler));
        }
    }
};

} // namespace Bedrock::Threading