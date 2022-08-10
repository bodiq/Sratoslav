#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool()
{
    int num_thread = std::max(std::thread::hardware_concurrency(), 2u);
    is_completed = false;
    for(size_t i = 0; i < num_thread; i++)
    {
        m_threads.emplace_back(&ThreadPool::spawnLoop, this);
    }
}

void ThreadPool::spawnLoop()
{
    while (!is_completed)
    {
        std::function<void()> currFunc;
        bool popped;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cov.wait(lock, [this] () {
                return !m_l_queues.empty() || is_completed.load();
            });

            popped = m_l_queues.pop(currFunc);
        }
        std::cout << "Lol" << std::endl;
        if(popped)
        {
            currFunc();
        }
    }
}

void ThreadPool::setDone()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        is_completed.store(true);
    }
    cov.notify_all();
    for(auto &ele : m_threads)
    {
        ele.join();
    }
    m_threads.clear();
}

void ThreadPool::addJob(int unique_id, std::function<void()> function)
{
    m_l_queues.push(function);
    cov.notify_one();
}

ThreadPool::~ThreadPool() noexcept
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        is_completed.store(true);
    }
    cov.notify_all();
    for(auto &ele : m_threads)
    {
        ele.join();
    }
    m_threads.clear();
}