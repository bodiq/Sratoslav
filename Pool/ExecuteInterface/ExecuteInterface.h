#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>

class ExecuteInterface
{
public:
    virtual void addJob(std::function<void()>) = 0;
};