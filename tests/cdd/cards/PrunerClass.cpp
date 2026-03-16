#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include "../cpp/util/fact_utils.h"

using namespace Chai::Cdd::Util;

struct RepoRecord {
    float usage_score;
    float redundancy_score;
    float ethics_risk_score;
};

float calculate_composite_score(const RepoRecord& repo, int age_days, const std::map<std::string, float>& weights) {
    float max_age_for_norm = 365.0;
    float normalized_age = std::min((float)age_days / max_age_for_norm, 1.0f);

    float usage_contribution = (1.0f - repo.usage_score) * weights.at("usage_weight");
    float age_contribution = normalized_age * weights.at("age_weight");
    float redundancy_contribution = repo.redundancy_score * weights.at("redundancy_weight");
    float ethics_contribution = repo.ethics_risk_score * weights.at("ethics_risk_weight");

    return usage_contribution + age_contribution + redundancy_contribution + ethics_contribution;
}

std::string make_pruning_decision(float score, const std::map<std::string, float>& thresholds) {
    if (score >= thresholds.at("delete_threshold")) return "DELETE";
    if (score >= thresholds.at("archive_threshold")) return "ARCHIVE";
    if (score >= thresholds.at("review_threshold")) return "REVIEW";
    return "KEEP";
}

// @Card: pruner_logic_verification
// @Results pruner_logic_operational == true
void pruner_logic_verification_card(const std::map<std::string, std::string>& facts) {
    std::map<std::string, float> weights = {
        {"usage_weight", std::stof(facts.at("usage_weight"))},
        {"age_weight", std::stof(facts.at("age_weight"))},
        {"redundancy_weight", std::stof(facts.at("redundancy_weight"))},
        {"ethics_risk_weight", std::stof(facts.at("ethics_risk_weight"))}
    };
    std::map<std::string, float> thresholds = {
        {"delete_threshold", std::stof(facts.at("delete_threshold"))},
        {"archive_threshold", std::stof(facts.at("archive_threshold"))},
        {"review_threshold", std::stof(facts.at("review_threshold"))}
    };
    int max_age_days = std::stoi(facts.at("max_age_days"));

    // Scenario: Old, unused, redundant repo (Should result in DELETE)
    RepoRecord repo = {0.0, 1.0, 0.0};
    float score = calculate_composite_score(repo, max_age_days, weights);
    std::string decision = make_pruning_decision(score, thresholds);

    bool score_ok = std::abs(score - thresholds.at("delete_threshold")) < 0.001f;
    bool decision_ok = (decision == "DELETE");

    std::cout << "pruner_logic_operational = " << (score_ok && decision_ok ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("tests/cdd/facts/pruner.facts");
    if (facts.empty()) {
        std::cerr << "Error: Could not read facts from tests/cdd/facts/pruner.facts" << std::endl;
        return 1;
    }
    pruner_logic_verification_card(facts);
    return 0;
}
