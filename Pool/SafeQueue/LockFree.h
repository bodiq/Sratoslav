#pragma once

#include <atomic>
#include <iostream>

template<typename T>
class LockFree
{
public:
    void push(T &data)
    {

    }
    bool pop(T &ele)
    {

    }
    bool empty()
    {
        return count == 0;
    }
private:
    struct Node
    {
        std::atomic<Node*> n_next;
        T data;
        Node(T &m_data) : data(m_data)
        {
            n_next = nullptr;
        }
        Node()
        {
            n_next = nullptr;
        }
    };
    std::atomic<Node*> tail;
    std::atomic<Node*> head;
    std::atomic<int> count{0};
};