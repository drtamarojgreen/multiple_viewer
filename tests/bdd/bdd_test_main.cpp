#include "bdd_test_main.h"
#include "bdd_runner.h"
#include <iostream>

namespace bdd {
    void registerBrainSteps();
    void registerDataSteps();
    void registerAutomationSteps();
    void registerOriginalSteps();
    void registerCoreSteps();
    void registerDomainSteps();
    void registerUISteps();
    void registerBrainAPISteps();
    void registerBrainAdvancedSteps();
}

#ifndef PROJECT_ROOT_DIR
#define PROJECT_ROOT_DIR "."
#endif

void runBDDTests() {
    std::cout << "\n=== Starting Native BDD Test Suite ===\n";
    
    bdd::registerDataSteps();
    bdd::registerOriginalSteps();
    bdd::registerUISteps();
    bdd::registerBrainSteps();
    bdd::registerAutomationSteps();
    bdd::registerCoreSteps();
    bdd::registerDomainSteps();
    bdd::registerBrainAPISteps();
    bdd::registerBrainAdvancedSteps();
    
    bool allSuccess = true;
    std::string root = PROJECT_ROOT_DIR;

    std::vector<std::string> features = {
        root + "/tests/bdd/features/original_functionality.feature",
        root + "/tests/bdd/features/brain_model.feature",
        root + "/tests/bdd/features/rendering_ui.feature",
        root + "/tests/bdd/features/data_analytics.feature"
    };

    for (const auto& feature : features) {
        if (!bdd::BDDRunner::getInstance().runFeature(feature)) {
            allSuccess = false;
        }
    }
    
    if (allSuccess) {
        std::cout << "\n[BDD] All primary features passed!\n";
    } else {
        std::cerr << "\n[BDD] Some primary features FAILED.\n";
    }
    std::cout << "======================================\n";
}
