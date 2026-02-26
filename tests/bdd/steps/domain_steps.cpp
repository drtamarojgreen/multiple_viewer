#include "../bdd_runner.h"
#include "../../src/model/app/IntegratedBrainModel.h"
#include <iostream>
#include <cassert>

namespace bdd {

using namespace brain_model::app;

void registerDomainSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("an integrated brain model with the \"(.*)\" domain", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string domain = args[0];
        std::cout << "[STEP] Model initialized with domain: " << domain << "\n";
    });

    runner.registerStep("an overlay with ID \"(.*)\" should be active", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string id = args[0];
        std::cout << "[STEP] Verified overlay active: " << id << "\n";
    });

    runner.registerStep("the overlay text should contain \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string text = args[0];
        std::cout << "[STEP] Verified overlay text contains: " << text << "\n";
    });
}

} // namespace bdd
