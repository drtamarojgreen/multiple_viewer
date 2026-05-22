#include "worker_pool.h"

namespace analytics {

WorkerPool::WorkerPool(size_t numThreads) : activeTasks(0), stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&WorkerPool::workerThread, this);
    }
}

WorkerPool::~WorkerPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void WorkerPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
        activeTasks++;
    }
    condition.notify_one();
}

void WorkerPool::waitAll() {
    std::unique_lock<std::mutex> lock(queueMutex);
    completionCondition.wait(lock, [this]() {
        return tasks.empty() && activeTasks == 0;
    });
}

void WorkerPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() {
                return stop || !tasks.empty();
            });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
        activeTasks--;
        if (activeTasks == 0) {
            completionCondition.notify_all();
        }
    }
}

} // namespace analytics
