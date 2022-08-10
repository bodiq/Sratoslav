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

    void push(T& data)
    {
        Node *q = new Node(data);
        Node *tmp = nullptr;

        bool check = false;

        if(tail == nullptr && tail.compare_exchange_strong(tmp, q))
        {
            count++;
            head = q;
            return;
        }

        Node *new_tail;
        Node *ex_tail;

        while(!check)
        {
            new_tail = tail.load();
            ex_tail = nullptr;
            check = new_tail->n_next.compare_exchange_strong(ex_tail, q);
        }

        tail.compare_exchange_strong(new_tail, q);
        count++;
    }

    bool pop(T& element)
    {
        if(head == nullptr)
        {
            return false;
        }

        Node *tmp = nullptr;
        Node *new_head = head.load();
        bool check = false;

        if(head == tail && head.compare_exchange_strong(new_head, tmp))
        {
            element = new_head->data;
            count--;

            tail = nullptr;
            delete tmp;
            return true;
        }

        while(!check)
        {
            new_head = head.load();
            tmp = new_head->n_next;
            check = head.compare_exchange_strong(new_head, tmp);
        }

        count--;
        element = new_head->data;
        delete new_head;
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

        Node(T& m_data) : data(m_data)
        {
            n_next = nullptr;
        }
        Node()
        {
            n_next = nullptr;
        }
    };
    std::atomic<Node*> tail{ nullptr };
    std::atomic<Node*> head{ nullptr };
    std::atomic<int> count{ 0 };
};