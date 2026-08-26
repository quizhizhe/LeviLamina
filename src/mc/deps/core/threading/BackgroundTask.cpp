#include "mc/deps/core/threading/BackgroundTask.h"

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
    );

    virtual ~BackgroundTask();

public:
    // static functions
    // NOLINTBEGIN
    static ::std::shared_ptr<::BackgroundTask<TResult, void>> create(
        ::gsl::not_null<::IBackgroundTaskOwner*> group,
        ::TaskStartInfo const& startInfo,
        ::Bedrock::Threading::Async<void> predecessor,
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

    void _setPredecessor(::Bedrock::Threading::SharedAsync<void> predecessor, bool linkCancellation);
    // NOLINTEND
};

// =============================== TaskDetails implementations ===============================

namespace TaskDetails {

// General template: TResult is the task return type, TInput is the task input type.

template <typename TResult, typename TInput>
BackgroundTaskDetails<TResult, TInput>::BackgroundTaskDetails(TaskFunction task) {
    mTask = std::move(task);
    mInput.emplace();
    mResult.reset();
}

template <typename TResult, typename TInput>
BackgroundTaskDetails<TResult, TInput>::~BackgroundTaskDetails() {}

template <typename TResult, typename TInput>
TResult BackgroundTaskDetails<TResult, TInput>::getResult() const {
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

template <typename TResult, typename TInput>
::TaskResult BackgroundTaskDetails<TResult, TInput>::_invoke() {
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

template <typename TResult, typename TInput>
void BackgroundTaskDetails<TResult, TInput>::_releaseTaskLambda() {
    mTask = {};
}

// Specialization with void input: no mInput member.

template <typename TResult>
BackgroundTaskDetails<TResult, void>::BackgroundTaskDetails(TaskFunction task) {
    mTask = std::move(task);
    mResult.reset();
}

template <typename TResult>
BackgroundTaskDetails<TResult, void>::~BackgroundTaskDetails() {}

template <typename TResult>
TResult BackgroundTaskDetails<TResult, void>::getResult() const {
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

template <typename TResult>
::TaskResult BackgroundTaskDetails<TResult, void>::_invoke() {
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

template <typename TResult>
void BackgroundTaskDetails<TResult, void>::_releaseTaskLambda() {
    mTask = {};
}

// Specialization for TaskResult tasks: the async result exposed to users is void.

template <typename TInput>
BackgroundTaskDetails<::TaskResult, TInput>::BackgroundTaskDetails(TaskFunction task) {
    mTask = std::move(task);
    mInput.emplace();
}

template <typename TInput>
BackgroundTaskDetails<::TaskResult, TInput>::~BackgroundTaskDetails() {}

template <typename TInput>
::TaskResult BackgroundTaskDetails<::TaskResult, TInput>::_invoke() {
    return mTask(std::move(*mInput));
}

template <typename TInput>
void BackgroundTaskDetails<::TaskResult, TInput>::_releaseTaskLambda() {
    mTask = {};
}

// Specialization for TaskResult tasks with void input.

BackgroundTaskDetails<::TaskResult, void>::BackgroundTaskDetails(TaskFunction task) {
    mTask = std::move(task);
}

BackgroundTaskDetails<::TaskResult, void>::~BackgroundTaskDetails() {}

::TaskResult BackgroundTaskDetails<::TaskResult, void>::_invoke() {
    return mTask();
}

void BackgroundTaskDetails<::TaskResult, void>::_releaseTaskLambda() {
    mTask = {};
}

} // namespace TaskDetails

// =============================== BackgroundTask implementations ===============================

// General template. TResult is the task return type, TInput is the task input type.

template <typename TResult, typename TInput>
BackgroundTask<TResult, TInput>::BackgroundTask(
    ::gsl::not_null<::IBackgroundTaskOwner*> group,
    ::TaskStartInfo const& startInfo,
    bool isAsync,
    TaskFunction task
)
: ::BackgroundTaskBase(group, startInfo, isAsync)
, ::TaskDetails::BackgroundTaskDetails<TResult, TInput>(std::move(task))
, mStatus(TaskStatus::Value::Pending)
, mPredecessor(nullptr) {
    if (!this->mTask) {

    }
}

template <typename TResult, typename TInput>
BackgroundTask<TResult, TInput>::~BackgroundTask() {}

template <typename TResult, typename TInput>
::std::shared_ptr<::BackgroundTask<TResult, TInput>> BackgroundTask<TResult, TInput>::create(
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

template <typename TResult, typename TInput>
void BackgroundTask<TResult, TInput>::cancel() {
    _tryTransitionTo(TaskStatus::Value::Canceled, true, false);
}

template <typename TResult, typename TInput>
bool BackgroundTask<TResult, TInput>::isReady() const {
    std::lock_guard lock(mLock);
    return mStatus.mValue == TaskStatus::Value::Pending;
}

template <typename TResult, typename TInput>
::TaskRunResult
BackgroundTask<TResult, TInput>::run(::ITaskExecutionContext const* executionContext, bool isAsyncContext) {
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

template <typename TResult, typename TInput>
::Bedrock::Threading::AsyncStatus BackgroundTask<TResult, TInput>::getStatus() const {
    std::lock_guard lock(mLock);
    return mStatus.toAsyncStatus();
}

template <typename TResult, typename TInput>
::std::error_code BackgroundTask<TResult, TInput>::getError() const {
    std::lock_guard lock(mLock);
    return mStatus.toErrorCode();
}

template <typename TResult, typename TInput>
void BackgroundTask<TResult, TInput>::addOnComplete(::std::function<void(const AsyncResult&)> callback) {
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

template <typename TResult, typename TInput>
bool BackgroundTask<TResult, TInput>::_tryTransitionTo(::TaskStatus status, bool a3, bool a4) {
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
                    mGroup->taskComplete(this);
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

template <typename TResult, typename TInput>
void BackgroundTask<TResult, TInput>::_setPredecessor(
    ::Bedrock::Threading::SharedAsync<TInput> predecessor,
    bool                                      linkCancellation
) {
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

// Specialization with void input.

template <typename TResult>
BackgroundTask<TResult, void>::BackgroundTask(
    ::gsl::not_null<::IBackgroundTaskOwner*> group,
    ::TaskStartInfo const& startInfo,
    bool isAsync,
    TaskFunction task
)
: ::BackgroundTaskBase(group, startInfo, isAsync)
, ::TaskDetails::BackgroundTaskDetails<TResult, void>(std::move(task))
, mStatus(TaskStatus::Value::Pending)
, mPredecessor(nullptr) {
    if (!this->mTask) {

    }
}

template <typename TResult>
BackgroundTask<TResult, void>::~BackgroundTask() {}

template <typename TResult>
::std::shared_ptr<::BackgroundTask<TResult, void>> BackgroundTask<TResult, void>::create(
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

template <typename TResult>
void BackgroundTask<TResult, void>::cancel() {
    _tryTransitionTo(TaskStatus::Value::Canceled, true, false);
}

template <typename TResult>
bool BackgroundTask<TResult, void>::isReady() const {
    std::lock_guard lock(mLock);
    return mStatus.mValue == TaskStatus::Value::Pending;
}

template <typename TResult>
::TaskRunResult
BackgroundTask<TResult, void>::run(::ITaskExecutionContext const* executionContext, bool isAsyncContext) {
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

template <typename TResult>
::Bedrock::Threading::AsyncStatus BackgroundTask<TResult, void>::getStatus() const {
    std::lock_guard lock(mLock);
    return mStatus.toAsyncStatus();
}

template <typename TResult>
::std::error_code BackgroundTask<TResult, void>::getError() const {
    std::lock_guard lock(mLock);
    return mStatus.toErrorCode();
}

template <typename TResult>
void BackgroundTask<TResult, void>::addOnComplete(::std::function<void(const AsyncResult&)> callback) {
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

template <typename TResult>
bool BackgroundTask<TResult, void>::_tryTransitionTo(::TaskStatus status, bool a3, bool a4) {
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
                    mGroup->taskComplete(this);
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

template <typename TResult>
void BackgroundTask<TResult, void>::_setPredecessor(
    ::Bedrock::Threading::SharedAsync<void> predecessor,
    bool                                    linkCancellation
) {
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