#include "../ExecuteInterface/ExecuteInterface.h"

class Async : public ExecuteInterface
{
public:
    Async() = default;
    void addJob(std::function<void()>) override;
    virtual ~Async();
private:
    std::vector<std::future<void>> futures;
};