#include <iostream>
#include <vector>
#include <set>
#include "analytics/nlp_engine.h"
#include "analytics/trend_analyzer.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

// @Card: trend_analysis_verification
// @Results trend_analysis_operational == true
void trend_analysis_card(const std::map<std::string, std::string>& facts) {
    // NLP Verification
    std::vector<std::string> texts = {"Treatment with aducanumab showed amyloid reduction.", "Emerging kinase inhibitors like imatinib."};
    std::set<std::string> keywords = analytics::NlpEngine::extractKeywords(texts);
    std::set<std::string> entities = analytics::NlpEngine::detectEntities(texts);

    bool has_amyloid = keywords.count("amyloid") > 0;
    bool has_mab = false;
    for (const auto& e : entities) if (e.find("mab") != std::string::npos) has_mab = true;

    // Growth Model Verification
    std::vector<double> series = {10, 20, 50, 100, 200, 400, 500, 550, 580};
    analytics::LogisticParams params = analytics::TrendAnalyzer::fitGrowthModel(series);

    bool operational = has_amyloid && has_mab && params.valid && (params.L > 0);
    std::cout << "trend_analysis_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("mesh_discovery.facts");
    if (facts.empty()) return 1;
    trend_analysis_card(facts);
    return 0;
}
