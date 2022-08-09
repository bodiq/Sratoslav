#include "../ExecuteInterface/ExecuteInterface.h"
#include "../SafeQueue/SafeQueue.h"
#include "../SafeQueue/LockFree.h"

class ThreadPool : public ExecuteInterface
{
public:
    ThreadPool();
    void addJob(std::function<void()>) override;
    void setDone();
    virtual ~ThreadPool();
private:
    void spawnLoop();
    std::atomic_bool is_completed;
    std::mutex mtx;
    std::condition_variable cov;
    std::vector<std::thread> m_threads;
    SafeQueue<std::function<void()>> m_queues;
    LockFree<std::function<void()>> m_l_queues;
};

