#pragma once

#include "SafeQueue/SafeQueue.h"
#include <thread>

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

    ~ThreadPool();
private:
    void spawn();

    std::atomic<bool> var_inited{false};
    std::atomic<bool> var_stop{false};
    std::atomic<bool> var_cancel{false};

    std::vector<std::thread> staff;
    SafeQueue<std::function<void()>> tasks;
    std::condition_variable cov;
    std::mutex mtx;
    std::once_flag once;
};


