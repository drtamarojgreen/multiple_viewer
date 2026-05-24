#ifndef WORKER_POOL_H
#define WORKER_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace analytics {

class WorkerPool {
public:
    WorkerPool(size_t numThreads);
    ~WorkerPool();

    void enqueue(std::function<void()> task);
    void waitAll();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable completionCondition;
    std::atomic<size_t> activeTasks;
    bool stop;

    void workerThread();
};

} // namespace analytics

#endif // WORKER_POOL_H
