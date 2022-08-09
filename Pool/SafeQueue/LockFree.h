#pragma once

#include <atomic>
#include <iostream>

template<typename T>
class LockFree
{
public:
    void push(T &m_data)
    {
        Node *new_node = new Node;
        new_node->data = m_data;
        new_node->n_next = nullptr;

        Node *t = new Node;
        Node *tmp = new Node;
        bool ok = false;
        while (!ok)
        {
            t = tail.load();
            ok = t->n_next.compare_exchange_strong(tmp, new_node);
        }
        tail.compare_exchange_strong(t, new_node);
        delete new_node;
        delete tmp;
        delete t;
    }

    bool pop(T &ele)
    {
        std::cout << "&&&&&" << std::endl;
        Node *p_next = new Node;
        Node *h = new Node;

        while(true)
        {
            h = head.load();

            p_next = h->n_next.load();

            if(head.load() != h)
            {
                continue;
            }

            if(p_next == nullptr)
            {
                return false;
            }

            Node *t = tail.load();

            if(h == t)
            {
                tail.compare_exchange_strong(t, p_next);
                continue;
            }

            if(head.compare_exchange_strong(h, p_next))
            {
                break;
            }
        }
        count--;
        Node *d = head;
        ele = d->data;

        delete p_next;
        delete h;
        return true;
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
    };
    std::atomic<Node*> tail;
    std::atomic<Node*> head;
    std::atomic<int> count{0};
};