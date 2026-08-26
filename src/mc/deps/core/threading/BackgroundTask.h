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
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&) = delete;
    BackgroundTaskDetails(BackgroundTaskDetails const&)            = delete;
    BackgroundTaskDetails()                                       = default;

    BackgroundTaskDetails(TaskFunction task) {
        mTask = std::move(task);
        mInput.emplace();
        mResult.reset();
    }

    virtual ~BackgroundTaskDetails() {}

public:
    // virtual functions
    // NOLINTBEGIN
    virtual TResult getResult() const override {
        if constexpr (std::is_same_v<TResult, ::TaskResult> || std::is_void_v<TResult>) {
            return;
        } else if constexpr (std::is_same_v<
                                 typename ::Bedrock::Threading::AsyncResult::ResultTraits<TResult>::Result,
                                 TResult>) {
            return *mResult;
        } else {
            return ::Bedrock::Threading::Async(mResult->get());
        }
    }
    // NOLINTEND

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke() {
        if constexpr (std::is_void_v<TResult>) {
            mTask(std::move(*mInput));
            mResult.emplace();
        } else if constexpr (std::is_same_v<
                                 typename ::Bedrock::Threading::AsyncResult::ResultTraits<TResult>::Result,
                                 TResult>) {
            mResult = std::move(mTask(std::move(*mInput)));
        } else {
            mResult.emplace(std::move(mTask(std::move(*mInput))));
        }
        return ::TaskResult::Done();
    }

    void _releaseTaskLambda() { mTask = {}; }
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
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&) = delete;
    BackgroundTaskDetails(BackgroundTaskDetails const&)            = delete;
    BackgroundTaskDetails()                                       = default;

    BackgroundTaskDetails(TaskFunction task) {
        mTask = std::move(task);
        mResult.reset();
    }

    virtual ~BackgroundTaskDetails() {}

public:
    // virtual functions
    // NOLINTBEGIN
    virtual TResult getResult() const override {
        if constexpr (std::is_same_v<TResult, ::TaskResult> || std::is_void_v<TResult>) {
            return;
        } else if constexpr (std::is_same_v<
                                 typename ::Bedrock::Threading::AsyncResult::ResultTraits<TResult>::Result,
                                 TResult>) {
            return *mResult;
        } else {
            return ::Bedrock::Threading::Async(mResult->get());
        }
    }
    // NOLINTEND

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke() {
        if constexpr (std::is_void_v<TResult>) {
            mTask();
            mResult.emplace();
        } else if constexpr (std::is_same_v<
                                 typename ::Bedrock::Threading::AsyncResult::ResultTraits<TResult>::Result,
                                 TResult>) {
            mResult = std::move(mTask());
        } else {
            mResult.emplace(std::move(mTask()));
        }
        return ::TaskResult::Done();
    }

    void _releaseTaskLambda() { mTask = {}; }
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
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&) = delete;
    BackgroundTaskDetails(BackgroundTaskDetails const&)            = delete;
    BackgroundTaskDetails()                                       = default;

    BackgroundTaskDetails(TaskFunction task) {
        mTask = std::move(task);
        mInput.emplace();
    }

    virtual ~BackgroundTaskDetails() {}

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke() { return mTask(std::move(*mInput)); }

    void _releaseTaskLambda() { mTask = {}; }
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
    BackgroundTaskDetails& operator=(BackgroundTaskDetails const&) = delete;
    BackgroundTaskDetails(BackgroundTaskDetails const&)            = delete;
    BackgroundTaskDetails()                                       = default;

    BackgroundTaskDetails(TaskFunction task) { mTask = std::move(task); }

    virtual ~BackgroundTaskDetails() {}

protected:
    // member functions
    // NOLINTBEGIN
    ::TaskResult _invoke() { return mTask(); }

    void _releaseTaskLambda() { mTask = {}; }
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
    )
    : ::BackgroundTaskBase(group, startInfo, isAsync),
      ::TaskDetails::BackgroundTaskDetails<TResult, TInput>(std::move(task)),
      mStatus(TaskStatus::Value::Pending),
      mPredecessor(nullptr) {
        if (!this->mTask) {
            //print "BackgroundTask is null." error
        }
    }

    virtual ~BackgroundTask() {}

public:
    // static functions
    // NOLINTBEGIN
    static ::std::shared_ptr<::BackgroundTask<TResult, TInput>> create(
        ::gsl::not_null<::IBackgroundTaskOwner*> group,
        ::TaskStartInfo const& startInfo,
        ::Bedrock::Threading::Async<TInput> predecessor,
        bool isAsync,
        TaskFunction task
    ) {
        auto result = std::make_shared<BackgroundTask<TResult, TInput>>(group, startInfo, isAsync, std::move(task));
        group->taskRegister(result);
        if (predecessor) {
            result->_tryTransitionTo(TaskStatus::Value::WaitingForPredecessor, false, false);
            result->_setPredecessor(predecessor, startInfo.mLinkCancellation);
        }
        return result;
    }
    // NOLINTEND

public:
    // virtual functions
    // NOLINTBEGIN
    virtual void cancel() override { _tryTransitionTo(TaskStatus::Value::Canceled, true, false); }

    virtual bool isReady() const override {
        std::lock_guard lock(mLock);
        return mStatus.mValue == TaskStatus::Value::Pending;
    }

    virtual ::TaskRunResult run(::ITaskExecutionContext const* executionContext, bool isAsyncContext) override {
        if (isAsyncContext != isAsync()) {
            return ::TaskRunResult(2 * _tryTransitionTo(TaskStatus::Value::Error, false, false));
        }
        if (!_tryTransitionTo(TaskStatus::Value::Running, false, false)) {
            return ::TaskRunResult::Noop;
        }
        ::TaskResult result = ::TaskResult::Done();
        for (;;) {
            auto groupState = getGroupState();
            if (groupState == ::TaskGroupState::Flush) {
                _tryTransitionTo(TaskStatus::Value::Canceled, false, false);
                return ::TaskRunResult::Noop;
            }
            if (groupState == ::TaskGroupState::Paused) {
                _tryTransitionTo(TaskStatus::Value::Pending, false, false);
                return ::TaskRunResult::Noop;
            }
            {
                ::BackgroundTaskBase::CurrentTaskAutoScope scope(this);
                ::Memory::ScopedMemoryTrackingTag          tag(mMemoryCategory);
                result = _invoke();
            }
            if (!isAsync() || result.isDone() || result.hasDelay()) {
                break;
            }
            {
                std::lock_guard lock(mLock);
                if (mStatus.mValue == TaskStatus::Value::CancelPending) {
                    break;
                }
            }
            if (!executionContext->canTaskRunAgain()) {
                break;
            }
        }
        if (result.isDone()) {
            _tryTransitionTo(TaskStatus::Value::Done, false, false);
            return ::TaskRunResult::Done;
        }
        setStartAfterTime(result.getRunAtTime());
        backDownPriority();
        if (_tryTransitionTo(TaskStatus::Value::Pending, false, false)) {
            return ::TaskRunResult::Requeue;
        }
        return ::TaskRunResult::Done;
    }

    virtual ::Bedrock::Threading::AsyncStatus getStatus() const override {
        std::lock_guard lock(mLock);
        return mStatus.toAsyncStatus();
    }

    virtual ::std::error_code getError() const override {
        std::lock_guard lock(mLock);
        return mStatus.toErrorCode();
    }

    virtual void addOnComplete(::std::function<void(const AsyncResult&)> callback) override {
        if (!callback) {
            return;
        }
        bool isComplete;
        {
            std::lock_guard lock(mLock);
            isComplete = mStatus.isComplete();
            if (!isComplete) {
                mComplete.push_back(CompletionHandlerInfo{
                    ::Memory::MemoryTracker::get().getCurrentCategory(),
                    std::move(callback),
                });
            }
        }
        if (isComplete) {
            callback(static_cast<const AsyncResult&>(*this));
        }
    }
    // NOLINTEND

private:
    // member functions
    // NOLINTBEGIN
    bool _tryTransitionTo(::TaskStatus status, bool a3, bool a4) {
        bool                                             result      = false;
        bool                                             runHandlers = false;
        ::Bedrock::Threading::SharedAsync<TInput>        savedPredecessor;
        ::std::vector<CompletionHandlerInfo>             movedHandlers;
        {
            std::lock_guard       lock(mLock);
            auto const            old       = mStatus.mValue;
            auto const            target    = status.mValue;
            bool                  onSuccess = false;
            ::TaskStatus::Value   newValue  = old;
            switch (target) {
            case ::TaskStatus::Value::WaitingForPredecessor:
                if (old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::Pending) {
                    mStatus.mValue = ::TaskStatus::Value::WaitingForPredecessor;
                    result         = true;
                } else if (old == ::TaskStatus::Value::CancelPending) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Canceled;
                    onSuccess = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::Pending:
                if (old == ::TaskStatus::Value::CancelPending) {
                    result    = false;
                    newValue  = ::TaskStatus::Value::Canceled;
                    onSuccess = true;
                } else if (
                    old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::Pending
                    || old == ::TaskStatus::Value::WaitingForPredecessor
                ) {
                    mStatus.mValue = ::TaskStatus::Value::Pending;
                    result         = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::Running:
                if (old == ::TaskStatus::Value::Pending) {
                    mStatus.mValue = ::TaskStatus::Value::Running;
                    result         = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::CancelPending:
                result = false;
                break;
            case ::TaskStatus::Value::Canceled:
                if (old == ::TaskStatus::Value::Pending) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Canceled;
                    onSuccess = true;
                } else if (old == ::TaskStatus::Value::Running) {
                    if (!a3) {
                        result    = true;
                        newValue  = ::TaskStatus::Value::Canceled;
                        onSuccess = true;
                    } else {
                        mStatus.mValue = ::TaskStatus::Value::CancelPending;
                        result         = true;
                    }
                } else if (old == ::TaskStatus::Value::CancelPending) {
                    if (a3) {
                        result = true;
                    } else {
                        result    = true;
                        newValue  = ::TaskStatus::Value::Canceled;
                        onSuccess = true;
                    }
                } else if (old == ::TaskStatus::Value::WaitingForPredecessor) {
                    if (!a4 && mLinkCancellation) {
                        savedPredecessor = mPredecessor;
                    }
                    if (a3 && savedPredecessor) {
                        mStatus.mValue = ::TaskStatus::Value::CancelPending;
                        result         = true;
                    } else {
                        result    = true;
                        newValue  = ::TaskStatus::Value::Canceled;
                        onSuccess = true;
                    }
                } else {
                    result = true;
                }
                break;
            case ::TaskStatus::Value::Error:
                if (old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::CancelPending
                    || old == ::TaskStatus::Value::WaitingForPredecessor) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Error;
                    onSuccess = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::Done:
                if (old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::CancelPending) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Done;
                    onSuccess = true;
                } else {
                    result = false;
                }
                break;
            default:
                result = true;
                break;
            }
            if (onSuccess) {
                mStatus.mValue = newValue;
                movedHandlers  = std::move(mComplete);
                mPredecessor   = nullptr;
                runHandlers    = true;
            }
        }
        if (savedPredecessor) {
            savedPredecessor.cancel();
            if (runHandlers) {
                while (savedPredecessor.getStatus() == ::Bedrock::Threading::AsyncStatus::Started) {
                    if (isAsync()) {
                        std::this_thread::yield();
                    } else {
                        mGroup->processCoroutines();
                    }
                }
            }
        }
        if (runHandlers) {
            ::BackgroundTaskBase::CurrentTaskAutoScope scope(this);
            {
                ::Memory::ScopedMemoryTrackingTag tag(mMemoryCategory);
                _releaseTaskLambda();
            }
            for (auto& handler : movedHandlers) {
                ::Memory::ScopedMemoryTrackingTag tag(handler.mMemoryCategory);
                handler.mFn(static_cast<const AsyncResult&>(*this));
            }
            taskComplete();
        }
        return result;
    }

    void _setPredecessor(::Bedrock::Threading::SharedAsync<TInput> predecessor, bool linkCancellation) {
        {
            std::lock_guard lock(mLock);
            mLinkCancellation = linkCancellation;
            mPredecessor      = predecessor;
        }
        auto weakThis = this->weak_from_this();
        predecessor.addOnComplete([weakThis](const ::Bedrock::Threading::IAsyncResult<TInput>& pred) {
            auto self = weakThis.lock();
            if (!self) {
                return;
            }
            auto selfTask = std::static_pointer_cast<BackgroundTask<TResult, TInput>>(self);
            switch (pred.getStatus()) {
            case ::Bedrock::Threading::AsyncStatus::Canceled:
                selfTask->_tryTransitionTo(TaskStatus::Value::Canceled, false, true);
                break;
            case ::Bedrock::Threading::AsyncStatus::Completed:
                if constexpr (!std::is_void_v<TInput>) {
                    selfTask->mInput = pred.getResult();
                }
                if (selfTask->_tryTransitionTo(TaskStatus::Value::Pending, false, true)) {
                    selfTask->mGroup->requeueTask(selfTask, false);
                }
                break;
            default:
                selfTask->_tryTransitionTo(TaskStatus::Value::Error, false, true);
                break;
            }
        });
    }
    // NOLINTEND
};

// Specialization with void input.
template <typename TResult>
class BackgroundTask<TResult, void> : public ::BackgroundTaskBase,
                                      public ::TaskDetails::BackgroundTaskDetails<TResult, void> {
public:
    using TaskFunction = ::brstd::move_only_function<TResult()>;
    using AsyncResult  = typename ::TaskDetails::BackgroundTaskDetails<TResult, void>::AsyncResult;

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
    ::std::vector<::BackgroundTask<TResult, void>::CompletionHandlerInfo> mComplete;
    bool mLinkCancellation;
    ::Bedrock::Threading::SharedAsync<void> mPredecessor;
    // NOLINTEND

public:
    // constructors
    BackgroundTask(
        ::gsl::not_null<::IBackgroundTaskOwner*> group,
        ::TaskStartInfo const& startInfo,
        bool isAsync,
        TaskFunction task
    )
    : ::BackgroundTaskBase(group, startInfo, isAsync),
      ::TaskDetails::BackgroundTaskDetails<TResult, void>(std::move(task)),
      mStatus(TaskStatus::Value::Pending),
      mPredecessor(nullptr) {
        if (!this->mTask) {
            //print "BackgroundTask is null." error
        }
    }

    virtual ~BackgroundTask() {}

public:
    // static functions
    // NOLINTBEGIN
    static ::std::shared_ptr<::BackgroundTask<TResult, void>> create(
        ::gsl::not_null<::IBackgroundTaskOwner*> group,
        ::TaskStartInfo const& startInfo,
        ::Bedrock::Threading::Async<void> predecessor,
        bool isAsync,
        TaskFunction task
    ) {
        auto result = std::make_shared<BackgroundTask<TResult, void>>(group, startInfo, isAsync, std::move(task));
        group->taskRegister(result);
        if (predecessor) {
            result->_tryTransitionTo(TaskStatus::Value::WaitingForPredecessor, false, false);
            result->_setPredecessor(predecessor, startInfo.mLinkCancellation);
        }
        return result;
    }
    // NOLINTEND

public:
    // virtual functions
    // NOLINTBEGIN
    virtual void cancel() override { _tryTransitionTo(TaskStatus::Value::Canceled, true, false); }

    virtual bool isReady() const override {
        std::lock_guard lock(mLock);
        return mStatus.mValue == TaskStatus::Value::Pending;
    }

    virtual ::TaskRunResult run(::ITaskExecutionContext const* executionContext, bool isAsyncContext) override {
        if (isAsyncContext != isAsync()) {
            return ::TaskRunResult(2 * _tryTransitionTo(TaskStatus::Value::Error, false, false));
        }
        if (!_tryTransitionTo(TaskStatus::Value::Running, false, false)) {
            return ::TaskRunResult::Noop;
        }
        ::TaskResult result = ::TaskResult::Done();
        for (;;) {
            auto groupState = getGroupState();
            if (groupState == ::TaskGroupState::Flush) {
                _tryTransitionTo(TaskStatus::Value::Canceled, false, false);
                return ::TaskRunResult::Noop;
            }
            if (groupState == ::TaskGroupState::Paused) {
                _tryTransitionTo(TaskStatus::Value::Pending, false, false);
                return ::TaskRunResult::Noop;
            }
            {
                ::BackgroundTaskBase::CurrentTaskAutoScope scope(this);
                ::Memory::ScopedMemoryTrackingTag          tag(mMemoryCategory);
                result = _invoke();
            }
            if (!isAsync() || result.isDone() || result.hasDelay()) {
                break;
            }
            {
                std::lock_guard lock(mLock);
                if (mStatus.mValue == TaskStatus::Value::CancelPending) {
                    break;
                }
            }
            if (!executionContext->canTaskRunAgain()) {
                break;
            }
        }
        if (result.isDone()) {
            _tryTransitionTo(TaskStatus::Value::Done, false, false);
            return ::TaskRunResult::Done;
        }
        setStartAfterTime(result.getRunAtTime());
        backDownPriority();
        if (_tryTransitionTo(TaskStatus::Value::Pending, false, false)) {
            return ::TaskRunResult::Requeue;
        }
        return ::TaskRunResult::Done;
    }

    virtual ::Bedrock::Threading::AsyncStatus getStatus() const override {
        std::lock_guard lock(mLock);
        return mStatus.toAsyncStatus();
    }

    virtual ::std::error_code getError() const override {
        std::lock_guard lock(mLock);
        return mStatus.toErrorCode();
    }

    virtual void addOnComplete(::std::function<void(const AsyncResult&)> callback) override {
        if (!callback) {
            return;
        }
        bool isComplete;
        {
            std::lock_guard lock(mLock);
            isComplete = mStatus.isComplete();
            if (!isComplete) {
                mComplete.push_back(CompletionHandlerInfo{
                    ::Memory::MemoryTracker::get().getCurrentCategory(),
                    std::move(callback),
                });
            }
        }
        if (isComplete) {
            callback(static_cast<const AsyncResult&>(*this));
        }
    }
    // NOLINTEND

private:
    // member functions
    // NOLINTBEGIN
    bool _tryTransitionTo(::TaskStatus status, bool a3, bool a4) {
        bool                                         result      = false;
        bool                                         runHandlers = false;
        ::Bedrock::Threading::SharedAsync<void>      savedPredecessor;
        ::std::vector<CompletionHandlerInfo>         movedHandlers;
        {
            std::lock_guard       lock(mLock);
            auto const            old       = mStatus.mValue;
            auto const            target    = status.mValue;
            bool                  onSuccess = false;
            ::TaskStatus::Value   newValue  = old;
            switch (target) {
            case ::TaskStatus::Value::WaitingForPredecessor:
                if (old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::Pending) {
                    mStatus.mValue = ::TaskStatus::Value::WaitingForPredecessor;
                    result         = true;
                } else if (old == ::TaskStatus::Value::CancelPending) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Canceled;
                    onSuccess = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::Pending:
                if (old == ::TaskStatus::Value::CancelPending) {
                    result    = false;
                    newValue  = ::TaskStatus::Value::Canceled;
                    onSuccess = true;
                } else if (
                    old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::Pending
                    || old == ::TaskStatus::Value::WaitingForPredecessor
                ) {
                    mStatus.mValue = ::TaskStatus::Value::Pending;
                    result         = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::Running:
                if (old == ::TaskStatus::Value::Pending) {
                    mStatus.mValue = ::TaskStatus::Value::Running;
                    result         = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::CancelPending:
                result = false;
                break;
            case ::TaskStatus::Value::Canceled:
                if (old == ::TaskStatus::Value::Pending) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Canceled;
                    onSuccess = true;
                } else if (old == ::TaskStatus::Value::Running) {
                    if (!a3) {
                        result    = true;
                        newValue  = ::TaskStatus::Value::Canceled;
                        onSuccess = true;
                    } else {
                        mStatus.mValue = ::TaskStatus::Value::CancelPending;
                        result         = true;
                    }
                } else if (old == ::TaskStatus::Value::CancelPending) {
                    if (a3) {
                        result = true;
                    } else {
                        result    = true;
                        newValue  = ::TaskStatus::Value::Canceled;
                        onSuccess = true;
                    }
                } else if (old == ::TaskStatus::Value::WaitingForPredecessor) {
                    if (!a4 && mLinkCancellation) {
                        savedPredecessor = mPredecessor;
                    }
                    if (a3 && savedPredecessor) {
                        mStatus.mValue = ::TaskStatus::Value::CancelPending;
                        result         = true;
                    } else {
                        result    = true;
                        newValue  = ::TaskStatus::Value::Canceled;
                        onSuccess = true;
                    }
                } else {
                    result = true;
                }
                break;
            case ::TaskStatus::Value::Error:
                if (old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::CancelPending
                    || old == ::TaskStatus::Value::WaitingForPredecessor) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Error;
                    onSuccess = true;
                } else {
                    result = false;
                }
                break;
            case ::TaskStatus::Value::Done:
                if (old == ::TaskStatus::Value::Running || old == ::TaskStatus::Value::CancelPending) {
                    result    = true;
                    newValue  = ::TaskStatus::Value::Done;
                    onSuccess = true;
                } else {
                    result = false;
                }
                break;
            default:
                result = true;
                break;
            }
            if (onSuccess) {
                mStatus.mValue = newValue;
                movedHandlers  = std::move(mComplete);
                mPredecessor   = nullptr;
                runHandlers    = true;
            }
        }
        if (savedPredecessor) {
            savedPredecessor.cancel();
            if (runHandlers) {
                while (savedPredecessor.getStatus() == ::Bedrock::Threading::AsyncStatus::Started) {
                    if (isAsync()) {
                        std::this_thread::yield();
                    } else {
                        mGroup->processCoroutines();
                    }
                }
            }
        }
        if (runHandlers) {
            ::BackgroundTaskBase::CurrentTaskAutoScope scope(this);
            {
                ::Memory::ScopedMemoryTrackingTag tag(mMemoryCategory);
                _releaseTaskLambda();
            }
            for (auto& handler : movedHandlers) {
                ::Memory::ScopedMemoryTrackingTag tag(handler.mMemoryCategory);
                handler.mFn(static_cast<const AsyncResult&>(*this));
            }
            taskComplete();
        }
        return result;
    }

    void _setPredecessor(::Bedrock::Threading::SharedAsync<void> predecessor, bool linkCancellation) {
        {
            std::lock_guard lock(mLock);
            mLinkCancellation = linkCancellation;
            mPredecessor      = predecessor;
        }
        auto weakThis = this->weak_from_this();
        predecessor.addOnComplete([weakThis](const ::Bedrock::Threading::IAsyncResult<void>& pred) {
            auto self = weakThis.lock();
            if (!self) {
                return;
            }
            auto selfTask = std::static_pointer_cast<BackgroundTask<TResult, void>>(self);
            switch (pred.getStatus()) {
            case ::Bedrock::Threading::AsyncStatus::Canceled:
                selfTask->_tryTransitionTo(TaskStatus::Value::Canceled, false, true);
                break;
            case ::Bedrock::Threading::AsyncStatus::Completed:
                if (selfTask->_tryTransitionTo(TaskStatus::Value::Pending, false, true)) {
                    selfTask->mGroup->requeueTask(selfTask, false);
                }
                break;
            default:
                selfTask->_tryTransitionTo(TaskStatus::Value::Error, false, true);
                break;
            }
        });
    }
    // NOLINTEND
};
