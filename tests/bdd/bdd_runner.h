#ifndef BDD_RUNNER_H
#define BDD_RUNNER_H

#include <string>
#include <vector>
#include <functional>
#include <map>
#include "map_logic.h"
#include "model/brain_model.h"
#include "model/brain_overlay.h"

namespace bdd {

struct BDDContext {
    Graph graph;
    model::BrainModel brainModel;
    model::BrainOverlay overlay;
    std::string lastResult;
    bool success = true;
};

using StepFunc = std::function<void(BDDContext&, const std::vector<std::string>&)>;

class BDDRunner {
public:
    static BDDRunner& getInstance() {
        static BDDRunner instance;
        return instance;
    }

    void registerStep(const std::string& pattern, StepFunc func);
    bool runFeature(const std::string& filepath);

private:
    BDDRunner() = default;
    
    std::map<std::string, StepFunc> steps_;
    
    bool executeLine(BDDContext& ctx, const std::string& line);
    std::string trim(const std::string& s);
};

// Macro for registration
#define GIVEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)
#define WHEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)
#define THEN(pattern, ctx, args) BDDRunner::getInstance().registerStep(pattern, [](BDDContext& ctx, const std::vector<std::string>& args)

} // namespace bdd

#endif // BDD_RUNNER_H
