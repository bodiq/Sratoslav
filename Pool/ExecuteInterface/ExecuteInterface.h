#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>
#include <future>
#include <vector>

class ExecuteInterface
{
public:
    virtual void addJob(int unique_id, std::function<void()>) = 0;
};