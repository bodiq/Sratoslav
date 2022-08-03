#include "ThreadPool.h"

int ThreadPool::size() const
{
    return staff.size();
}

bool ThreadPool::is_running() const
{
    return var_inited && !var_stop && !var_cancel;
}

bool ThreadPool::inited() const
{
    return var_inited;
}

ThreadPool::~ThreadPool()
{
    terminate();
}

void ThreadPool::init(int num)
{
    std::call_once(once, [this, num] () {
        std::unique_lock<std::mutex> lock(mtx);
        var_stop = false;
        var_cancel = false;
        staff.reserve(num);
        for(size_t i = 0; i < num; i++)
        {
            tasks.emplace(std::bind(&ThreadPool::spawn, this));
        }
        var_inited = true;
    });
}

void ThreadPool::spawn()
{
    for(;;)
    {
        bool pop = false;
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cov.wait(lock, [&] ()
            {
               pop = tasks.pop(task);
               return var_cancel || var_stop || pop;
            });
        }
        if(var_cancel || (var_stop && !pop))
        {
            return;
        }
        task();
    }
}

void ThreadPool::terminate()
{
    std::unique_lock<std::mutex> lock(mtx);
    if(is_running())
    {
        var_stop = true;
    }
    else
    {
        return;
    }
    cov.notify_all();
    for(auto &a : staff)
    {
        a.join();
    }
}

void ThreadPool::cancel()
{
    std::unique_lock<std::mutex> lock(mtx);
    if(is_running())
    {
        var_cancel = true;
    }
    else
    {
        return;
    }
    tasks.clear();
    cov.notify_all();
    for(auto &a : staff)
    {
        a.join();
    }
}

template <class F, class... Args>
auto ThreadPool::Add_Job(F&& f, Args&&... args) const -> std::future<decltype(f(args...))>
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(var_stop || var_cancel)
        {
            throw std::runtime_error("Adding task that is stoped or canceled! ;(>");
        }
    }

    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    std::shared_ptr<std::packaged_task<decltype(f(args...))>> work =
    std::make_shared<std::packaged_task<decltype(f(args...))>>(std::move(func));
    std::future<decltype(f(args...))> future = work->get_future();
    tasks.emplace([work] () -> void {
        (*work)();
    });
    cov.notify_all();
    return future;
}
