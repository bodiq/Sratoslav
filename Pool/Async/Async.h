#include "../ExecuteInterface/ExecuteInterface.h"
#include <map>

class Async : public ExecuteInterface
{
public:
    Async() = default;
    void addJob(int unique_id, std::function<void()>) override;
    virtual ~Async();
private:
    std::vector<int> id_delete;
    std::map<int, std::future<void>> mp;
    std::mutex mtx;
};