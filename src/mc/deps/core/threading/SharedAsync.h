#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/deps/core/threading/Async.h"
#include "mc/deps/core/threading/IAsyncResult.h"

namespace Bedrock::Threading {

template <typename T>
class SharedAsync {
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
    SharedAsync() = default;
    SharedAsync(std::nullptr_t) {}
    explicit SharedAsync(Handle result) : mResult(std::move(result)) {}
    SharedAsync(::Bedrock::Threading::Async<T> const& other) : mResult(other.mResult) {}
    SharedAsync(::Bedrock::Threading::Async<T>&& other) : mResult(std::move(other.mResult)) {}
    SharedAsync(SharedAsync const&)            = default;
    SharedAsync(SharedAsync&&) noexcept        = default;
    SharedAsync& operator=(SharedAsync const&) = default;
    SharedAsync& operator=(SharedAsync&&) noexcept = default;
    SharedAsync& operator=(std::nullptr_t) {
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