#include <iostream>
#include <vector>
#include <atomic>
#include <chrono>
#include "analytics/worker_pool.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

// @Card: worker_pool_verification
// @Results worker_pool_operational == true
void worker_pool_card(const std::map<std::string, std::string>& facts) {
    int num_tasks = std::stoi(facts.at("task_count"));
    std::atomic<int> completed_tasks(0);

    {
        analytics::WorkerPool pool(4);
        for (int i = 0; i < num_tasks; ++i) {
            pool.enqueue([&completed_tasks]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                completed_tasks++;
            });
        }
        pool.waitAll();
    }

    bool operational = (completed_tasks == num_tasks);
    std::cout << "worker_pool_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("worker_pool.facts");
    if (facts.empty()) return 1;
    worker_pool_card(facts);
    return 0;
}
