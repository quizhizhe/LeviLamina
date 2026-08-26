#pragma once

#include "mc/_HeaderOutputPredefine.h"


#include "mc/deps/core/memory/MemoryCategory.h"
#include "mc/deps/core/memory/MemoryTracker.h"
#include "mc/deps/core/memory/ScopedMemoryTrackingTag.h"
#include "mc/deps/core/threading/Async.h"
#include "mc/deps/core/threading/AsyncStatus.h"
#include "mc/deps/core/threading/BackgroundTaskBase.h"
#include "mc/deps/core/threading/IBackgroundTaskOwner.h"
#include "mc/deps/core/threading/IAsyncResult.h"
#include "mc/deps/core/threading/ITaskExecutionContext.h"
#include "mc/deps/core/threading/ResultTraits.h"
#include "mc/deps/core/threading/SharedAsync.h"
#include "mc/deps/core/threading/TaskResult.h"
#include "mc/deps/core/threading/TaskRunResult.h"
#include "mc/deps/core/threading/TaskStartInfo.h"
#include "mc/deps/core/threading/TaskStatus.h"
#include "mc/platform/brstd/move_only_function.h"
#include "mc/platform/threading/Mutex.h"
#include "mc/diagnostics/Diagnostics.h"

#include <thread>

namespace TaskDetails {

// General template. TResult is the task return type, TInput is the task input type.
template <typename TResult, typename TInput>
class BackgroundTaskDetails : public ::Bedrock::Threading::IAsyncResult<TResult> {
public:
    using AsyncResultType = TResult;
    using AsyncResult     = ::Bedrock::Threading::IAsyncResult<TResult>;
    using TaskFunction    = ::brstd::move_only_function<TResult(TInput)>;

public:
    // prevent constructor by default
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&);
    BackgroundTaskDetails(BackgroundTaskDetails const&);
    BackgroundTaskDetails();

    BackgroundTaskDetails(TaskFunction task);

    virtual ~BackgroundTaskDetails();

public:
    // virtual functions
    // NOLINTBEGIN
    virtual TResult getResult() const override;
    // NOLINTEND

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke();

    void _releaseTaskLambda();
    // NOLINTEND

protected:
    // member variables
    // NOLINTBEGIN
    TaskFunction mTask;
    ::std::optional<TInput> mInput;
    ::std::optional<typename ::Bedrock::Threading::AsyncResult::ResultTraits<TResult>::Result> mResult;
    // NOLINTEND
};

// Specialization with void input: no mInput member.
template <typename TResult>
class BackgroundTaskDetails<TResult, void> : public ::Bedrock::Threading::IAsyncResult<TResult> {
public:
    using AsyncResultType = TResult;
    using AsyncResult     = ::Bedrock::Threading::IAsyncResult<TResult>;
    using TaskFunction    = ::brstd::move_only_function<TResult()>;

public:
    // prevent constructor by default
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&);
    BackgroundTaskDetails(BackgroundTaskDetails const&);
    BackgroundTaskDetails();

    BackgroundTaskDetails(TaskFunction task);

    virtual ~BackgroundTaskDetails();

public:
    // virtual functions
    // NOLINTBEGIN
    virtual TResult getResult() const override;
    // NOLINTEND

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke();

    void _releaseTaskLambda();
    // NOLINTEND

protected:
    // member variables
    // NOLINTBEGIN
    TaskFunction mTask;
    ::std::optional<typename ::Bedrock::Threading::AsyncResult::ResultTraits<TResult>::Result> mResult;
    // NOLINTEND
};

// Specialization for TaskResult tasks: the async result exposed to users is void.
template <typename TInput>
class BackgroundTaskDetails<::TaskResult, TInput> : public ::Bedrock::Threading::IAsyncResult<void> {
public:
    using AsyncResultType = void;
    using AsyncResult     = ::Bedrock::Threading::IAsyncResult<void>;
    using TaskFunction    = ::brstd::move_only_function<::TaskResult(TInput)>;

public:
    // prevent constructor by default
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&);
    BackgroundTaskDetails(BackgroundTaskDetails const&);
    BackgroundTaskDetails();

    BackgroundTaskDetails(TaskFunction task);

    virtual ~BackgroundTaskDetails();

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke();

    void _releaseTaskLambda();
    // NOLINTEND

protected:
    // member variables
    // NOLINTBEGIN
    TaskFunction mTask;
    ::std::optional<TInput> mInput;
    // NOLINTEND
};

// Specialization for TaskResult tasks with void input.
template <>
class BackgroundTaskDetails<::TaskResult, void> : public ::Bedrock::Threading::IAsyncResult<void> {
public:
    using AsyncResultType = void;
    using AsyncResult     = ::Bedrock::Threading::IAsyncResult<void>;
    using TaskFunction    = ::brstd::move_only_function<::TaskResult()>;

public:
    // prevent constructor by default
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&);
    BackgroundTaskDetails(BackgroundTaskDetails const&);
    BackgroundTaskDetails();

    BackgroundTaskDetails(TaskFunction task);

    virtual ~BackgroundTaskDetails();

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke();

    void _releaseTaskLambda();
    // NOLINTEND

protected:
    // member variables
    // NOLINTBEGIN
    TaskFunction mTask;
    // NOLINTEND
};

} // namespace TaskDetails

// General template. TResult is the task return type, TInput is the task input type.
template <typename TResult, typename TInput>
class BackgroundTask : public ::BackgroundTaskBase, public ::TaskDetails::BackgroundTaskDetails<TResult, TInput> {
public:
    using TaskFunction = ::brstd::move_only_function<TResult(TInput)>;
    using AsyncResult  = typename ::TaskDetails::BackgroundTaskDetails<TResult, TInput>::AsyncResult;

    // BackgroundTask inner types define
    class CompletionHandlerInfo {
    public:
        // member variables
        // NOLINTBEGIN
        ::Memory::MemoryCategory mMemoryCategory;
        ::std::function<void(const AsyncResult&)> mFn;
        // NOLINTEND
    };

public:
    // member variables
    // NOLINTBEGIN
    mutable ::Bedrock::Threading::Mutex mLock;
    ::TaskStatus mStatus;
    ::std::vector<::BackgroundTask<TResult, TInput>::CompletionHandlerInfo> mComplete;
    bool mLinkCancellation;
    ::Bedrock::Threading::SharedAsync<TInput> mPredecessor;
    // NOLINTEND

public:
    // constructors
    BackgroundTask(
        ::gsl::not_null<::IBackgroundTaskOwner*> group,
        ::TaskStartInfo const& startInfo,
        bool isAsync,
        TaskFunction task
    );

    virtual ~BackgroundTask();

public:
    // static functions
    // NOLINTBEGIN
    static ::std::shared_ptr<::BackgroundTask<TResult, TInput>> create(
        ::gsl::not_null<::IBackgroundTaskOwner*> group,
        ::TaskStartInfo const& startInfo,
        ::Bedrock::Threading::Async<TInput> predecessor,
        bool isAsync,
        TaskFunction task
    );
    // NOLINTEND

public:
    // virtual functions
    // NOLINTBEGIN
    virtual void cancel() override;

    virtual bool isReady() const override;

    virtual ::TaskRunResult run(::ITaskExecutionContext const* executionContext, bool isAsyncContext) override;

    virtual ::Bedrock::Threading::AsyncStatus getStatus() const override;

    virtual ::std::error_code getError() const override;

    virtual void addOnComplete(::std::function<void(const AsyncResult&)> callback) override;
    // NOLINTEND

private:
    // member functions
    // NOLINTBEGIN
    bool _tryTransitionTo(::TaskStatus status, bool a3, bool a4);

    void _setPredecessor(::Bedrock::Threading::SharedAsync<TInput> predecessor, bool linkCancellation);
    // NOLINTEND
};


