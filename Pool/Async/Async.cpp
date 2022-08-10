#include "Async.h"
#include <iostream>

void Async::addJob(int unique_id, std::function<void()> function)
{
    std::cout << "Kil" << std::endl;
    {
        std::unique_lock<std::mutex> lock(mtx);
        for (auto &el : id_delete)
        {
            mp.erase(el);
        }
    }
    /*futures.emplace_back(std::async(std::launch::async, function));*/
    std::unique_lock<std::mutex> lock(mtx);
    mp[unique_id] = std::async(std::launch::async, [function, unique_id, this] () {
        function();
        std::unique_lock<std::mutex> lock(mtx);
        id_delete.push_back(unique_id);
    });
}

Async::~Async() {}

