#include "../bdd_runner.h"
#include "model/quanta_glia.h"
#include <iostream>
#include <cassert>

namespace bdd {

void registerQuantaGliaSteps() {
    auto& runner = BDDRunner::getInstance();

    runner.registerStep("a QuantaGlia manager", [](BDDContext& ctx, const std::vector<std::string>& args) {
        model::QuantaGlia::getInstance().clear();
        std::cout << "[STEP] QuantaGlia manager reset\n";
    });

    runner.registerStep("I create a repository named \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string name = args[0];
        model::QuantaGlia::getInstance().createRepository(name);
        std::cout << "[STEP] Created repository: " << name << "\n";
    });

    runner.registerStep("I should have (\\d+) active repositories", [](BDDContext& ctx, const std::vector<std::string>& args) {
        int expected = std::stoi(args[0]);
        int actual = model::QuantaGlia::getInstance().getAllRepositoryIds().size();
        assert(actual == expected);
        std::cout << "[STEP] Verified " << actual << " active repositories\n";
    });

    runner.registerStep("I should be able to retrieve \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string name = args[0];
        auto repo = model::QuantaGlia::getInstance().getRepository(name);
        assert(repo != nullptr);
        std::cout << "[STEP] Retrieved repository: " << name << "\n";
    });

    runner.registerStep("a repository named \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string name = args[0];
        model::QuantaGlia::getInstance().createRepository(name);
    });

    runner.registerStep("I load a region \"(.*)\" into \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string regionId = args[0];
        std::string repoId = args[1];
        auto repo = model::QuantaGlia::getInstance().getRepository(repoId);

        model::BrainRegion region;
        region.id = regionId;
        region.name = "Test Region";
        repo->addRegion(region);
        std::cout << "[STEP] Loaded region " << regionId << " into " << repoId << "\n";
    });

    runner.registerStep("\"(.*)\" should contain region \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string repoId = args[0];
        std::string regionId = args[1];
        auto repo = model::QuantaGlia::getInstance().getRepository(repoId);
        assert(repo->getModel().getRegion(regionId) != nullptr);
        std::cout << "[STEP] Verified " << repoId << " contains " << regionId << "\n";
    });

    runner.registerStep("\"(.*)\" should NOT contain region \"(.*)\"", [](BDDContext& ctx, const std::vector<std::string>& args) {
        std::string repoId = args[0];
        std::string regionId = args[1];
        auto repo = model::QuantaGlia::getInstance().getRepository(repoId);
        assert(repo->getModel().getRegion(regionId) == nullptr);
        std::cout << "[STEP] Verified " << repoId << " does NOT contain " << regionId << "\n";
    });
}

} // namespace bdd
