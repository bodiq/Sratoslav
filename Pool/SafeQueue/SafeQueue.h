#pragma once

#include <queue>
#include <mutex>
#include <atomic>

template <typename T>
class SafeQueue
{
public:
    SafeQueue() = default;
    SafeQueue(const SafeQueue &) = delete;
    SafeQueue(SafeQueue &&) = delete;
    SafeQueue &operator=(const SafeQueue &) = delete;
    SafeQueue &operator=(SafeQueue &&) = delete;

    void push(T &t);
    void push(T &&t);
    bool pop(T &ele);
    T front() const;

    template<typename... Args>
    void emplace(Args&& ...args)
    {
        std::unique_lock<std::mutex> lock(mtx);
        queue.emplace(std::forward<Args>(args)...);
    }

    void clear();
    int size() const;
    bool empty() const;
private:
    mutable std::mutex mtx;
    std::queue<T> queue;
};

template<typename T>
void SafeQueue<T>::clear()
{
    std::unique_lock<std::mutex> lock(mtx);
    while(!queue.empty())
    {
        queue.pop();
    }
}

template<typename T>
void SafeQueue<T>::push(T &t)
{
    std::unique_lock<std::mutex> lock(mtx);

    queue.push(t);
}

template<typename T>
void SafeQueue<T>::push(T &&t)
{
    std::unique_lock<std::mutex> lock(mtx);
    queue.push(t);
}

template<typename T>
int SafeQueue<T>::size() const
{
    std::unique_lock<std::mutex> lock(mtx);
    return queue.size();
}

template<typename T>
bool SafeQueue<T>::pop(T &ele)
{
    std::unique_lock<std::mutex> lock(mtx);

    if(queue.empty())
    {
        return false;
    }

    ele = std::move(queue.front());

    queue.pop();

    return true;
}

template <typename T>
bool SafeQueue<T>::empty() const
{
    std::unique_lock<std::mutex> lock(mtx);
    return queue.empty();
}