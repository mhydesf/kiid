#pragma once

#include <mutex>
#include <chrono>
#include <thread>
#include <functional>
#include <condition_variable>

namespace Kiid::Executor {

class Executor {
public:
    using milliseconds = std::chrono::milliseconds;
    Executor() {}

    Executor(const Executor& other) = delete;
    Executor& operator=(const Executor& other) = delete;

    Executor(const Executor&& other) {}
    Executor& operator=(const Executor&& other) {
        if (this == &other) {
            return *this;
        }

        return *this;
    }

private:
    void Run(std::stop_token token) {
        while (!token.stop_requested()) {

            RateSleep();
        }
    }
    
    void RateSleep() {
        std::unique_lock<std::mutex> lock{m_sleep_mtx};
        auto untilTime = m_last_rate_update + std::chrono::duration_cast<milliseconds>(m_period_ms);
        m_sleep_cv.wait_until(lock, untilTime);

        m_last_rate_update = std::chrono::steady_clock::now();
    }

private:
    std::jthread m_thread;

    std::mutex m_sleep_mtx;
    std::condition_variable m_sleep_cv;
    std::chrono::steady_clock::time_point m_last_rate_update;
    milliseconds m_period_ms;
};

} // namespace Kiid::Executor
