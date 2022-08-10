#pragma once

#include <atomic>
#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

template<typename T>
class LockFree
{
public:

    void push(T &data)
    {
        Node *q = new Node(data);

        bool check = false;

        Node *p_Tail = nullptr;
        Node *tmp = nullptr;

        if(tail == nullptr)
        {
            std::atomic_compare_exchange_strong(&tail, &tmp, q);
            std::atomic_compare_exchange_strong(&head, &tmp, q);
        }

        while(!check)
        {
            p_Tail = tail.load();
            check = p_Tail->n_next.compare_exchange_strong(tmp, q);
            if(!check)
            {
                tail.compare_exchange_strong(p_Tail, p_Tail->n_next);
            }
        }
        tail.compare_exchange_strong(p_Tail, q);
        count++;
    }

    bool pop(T &element)
    {
        Node *tmp = nullptr;
        Node *p = tail.load();
        bool check = false;


        while (p->n_next != nullptr)
        {
            if(p->n_next == head)
            {
                break;
            }
            p = p->n_next.load();
        }

        while(!check)
        {
            tmp = head.load();
            if(tmp == nullptr)
            {
                return false;
            }
            check = head.compare_exchange_strong(tmp, p);
        }

        count--;
        element = p->data;
        return true;
    }

    bool empty()
    {
        return (count == 0);
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