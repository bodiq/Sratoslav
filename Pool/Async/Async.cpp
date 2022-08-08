#include "Async.h"

void Async::addJob(std::function<void()> function)
{
    futures.emplace_back(std::async(std::launch::async, function));
}

Async::~Async()
{
    for(auto &el : futures)
    {
        el.get();
    }
}