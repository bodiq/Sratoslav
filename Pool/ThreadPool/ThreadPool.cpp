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
                return !m_queues.empty() || is_completed.load();
            });

            popped = m_queues.pop(currFunc);
        }
        if(popped)
        {
            currFunc();
        }
    }
}

void ThreadPool::setDone()
{
    is_completed = true;
}

void ThreadPool::addJob(std::function<void()> function)
{
    std::cout << "Bad" << std::endl;
    m_queues.push(function);
    cov.notify_one();
}

ThreadPool::~ThreadPool() noexcept
{
    std::cout << "Fuck" << std::endl;
    {
        std::unique_lock<std::mutex> lock(mtx);
        is_completed.store(true);
    }
    std::cout << "Fuck" << std::endl;
    cov.notify_all();
    std::cout << "Fuck" << std::endl;
    for(auto &ele : m_threads)
    {
        ele.join();
    }
    m_threads.clear();
}