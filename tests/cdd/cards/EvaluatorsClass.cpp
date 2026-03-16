#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>
#include "../cpp/util/fact_utils.h"

using namespace Chai::Cdd::Util;

// @Card: keyword_validator_verification
// @Is python_available == true
// @Results keyword_validator_operational == true
void keyword_validator_verification_card(const std::map<std::string, std::string>& facts) {
    std::string text = facts.at("keyword_text");
    std::string keyword = facts.at("keyword_target");

    std::string python_cmd = "python3 -c \"from scripts.app import evaluators; point = {'type': 'keyword', 'params': {'keywords': ['" + keyword + "'], 'min_count': 1}}; ok, _, _, _ = evaluators.run_evaluation_point('" + text + "', point); print(ok)\"";

    // Using popen to capture output
    FILE* pipe = popen(python_cmd.c_str(), "r");
    if (!pipe) {
        std::cout << "keyword_validator_operational = false" << std::endl;
        return;
    }
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);

    bool operational = (result.find("True") != std::string::npos);
    std::cout << "keyword_validator_operational = " << (operational ? "true" : "false") << std::endl;
}

// @Card: citation_validator_verification
// @Is python_available == true
// @Results citation_validator_operational == true
void citation_validator_verification_card(const std::map<std::string, std::string>& facts) {
    std::string text = facts.at("citation_text");
    std::string min_count = facts.at("citation_min_count");

    std::string python_cmd = "python3 -c \"from scripts.app import evaluators; point = {'type': 'citation', 'params': {'min_count': " + min_count + "}}; ok, _, _, _ = evaluators.run_evaluation_point('" + text + "', point); print(ok)\"";

    FILE* pipe = popen(python_cmd.c_str(), "r");
    if (!pipe) {
        std::cout << "citation_validator_operational = false" << std::endl;
        return;
    }
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);

    bool operational = (result.find("True") != std::string::npos);
    std::cout << "citation_validator_operational = " << (operational ? "true" : "false") << std::endl;
}

int main(int argc, char* argv[]) {
    auto facts = FactReader::readFacts("tests/cdd/facts/evaluators.facts");
    if (facts.empty()) {
        std::cerr << "Error: Could not read facts from tests/cdd/facts/evaluators.facts" << std::endl;
        return 1;
    }

    if (argc > 1 && std::string(argv[1]) == "citation") {
        citation_validator_verification_card(facts);
    } else {
        keyword_validator_verification_card(facts);
    }
    return 0;
}
