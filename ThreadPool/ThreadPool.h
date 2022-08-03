#pragma once

#include "SafeQueue/SafeQueue.h"

class ThreadPool
{
public:
    ThreadPool() = default;
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    void init(int num);
    void terminate();
    void cancel();

    bool inited() const;
    bool is_running() const;
    int size() const;

    template<class F, class... Args>
    auto Add_Job(F &&f, Args&&... args) const -> std::future<decltype(f(args...))>;

    ~ThreadPool();
private:
    void spawn();

    std::atomic<bool> var_inited{false};
    std::atomic<bool> var_stop{false};
    std::atomic<bool> var_cancel{false};

    std::vector<std::thread> staff;
    SafeQueue<std::function<void()>> tasks;
    mutable std::condition_variable cov;
    mutable std::mutex mtx;
    std::once_flag once;
};


