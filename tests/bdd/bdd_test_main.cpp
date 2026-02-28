#include "bdd_test_main.h"
#include "bdd_runner.h"
#include <iostream>

namespace bdd {
    void registerBrainSteps();
    void registerRenderingSteps();
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
    
    bdd::registerBrainSteps();
    bdd::registerRenderingSteps();
    bdd::registerDataSteps();
    bdd::registerAutomationSteps();
    bdd::registerOriginalSteps();
    bdd::registerCoreSteps();
    bdd::registerDomainSteps();
    bdd::registerUISteps();
    bdd::registerBrainAPISteps();
    bdd::registerBrainAdvancedSteps();
    
    bool allSuccess = true;
    std::string root = PROJECT_ROOT_DIR;
    std::vector<std::string> features = {
        root + "/tests/bdd/features/original_functionality.feature",
        root + "/tests/bdd/features/brain_model.feature",
        root + "/tests/bdd/features/rendering_ui.feature",
        root + "/tests/bdd/features/data_analytics.feature",
        root + "/tests/bdd/features/scripting_research.feature",
        root + "/tests/bdd/features/performance_ecosystem.feature",
        root + "/tests/bdd/features/core_kernel.feature",
        root + "/tests/bdd/features/cognition_domain.feature",
        root + "/tests/bdd/features/emotion_domain.feature",
        root + "/tests/bdd/features/neuro_domain.feature",
        root + "/tests/bdd/features/stress_domain.feature",
        root + "/tests/bdd/features/molecular_domains.feature",
        root + "/tests/bdd/features/neurochemical_domains.feature",
        root + "/tests/bdd/features/connectionist_domains.feature",
        root + "/tests/bdd/features/systemic_domains.feature",
        root + "/tests/bdd/features/brain_api.feature",
        root + "/tests/bdd/features/brain_advanced.feature"
    };

    for (const auto& feature : features) {
        if (!bdd::BDDRunner::getInstance().runFeature(feature)) {
            allSuccess = false;
        }
    }
    
    if (allSuccess) {
        std::cout << "\n[BDD] All features passed!\n";
    } else {
        std::cerr << "\n[BDD] Some features FAILED.\n";
    }
    std::cout << "======================================\n";
}
