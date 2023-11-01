#pragma once

#include <chrono>
#include <mutex>

namespace ll::thread {
class InterruptableSleep {
private:
    bool                    interrupted{false};
    std::mutex              mutex;
    std::condition_variable cv;

public:
    InterruptableSleep(const InterruptableSleep&)                     = delete;
    InterruptableSleep(InterruptableSleep&&) noexcept                 = delete;
    InterruptableSleep& operator=(const InterruptableSleep&) noexcept = delete;
    InterruptableSleep& operator=(InterruptableSleep&&) noexcept      = delete;

    InterruptableSleep() = default;

    ~InterruptableSleep() = default;

    template <class R, class P>
    void sleepFor(std::chrono::duration<R, P> duration) {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait_for(lock, duration, [this] { return interrupted; });
        interrupted = false;
    }

    template <class C, class D>
    void sleepUntil(std::chrono::time_point<C, D> time) {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait_until(lock, time, [this] { return interrupted; });
        interrupted = false;
    }

    void sleep() {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, [this] { return interrupted; });
        interrupted = false;
    }

    void interrupt() {
        std::lock_guard<std::mutex> lg(mutex);
        interrupted = true;
        cv.notify_one();
    }
};
} // namespace ll::thread
