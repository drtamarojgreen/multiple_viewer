#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include "logger.h"
#include "../cpp/util/fact_utils.h"

namespace fs = std::filesystem;
using namespace Chai::Cdd::Util;

// @Card: logging_setup_verification
// @Results logging_setup_operational == true
void logging_setup_verification_card(const std::map<std::string, std::string>& facts) {
    std::string message = facts.at("test_message");

    // Capture stdout to verify logger output
    // In a real terminal this might be tricky, but we can redirect or just call it.
    // Since Logger::info prints to std::cout, we can't easily capture it in-process without
    // redirecting rdbuf. Let's do a simple redirection for the test.

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    Logger::info(message);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    bool operational = (output.find(message) != std::string::npos && output.find("[INFO]") != std::string::npos);

    std::cout << "logging_setup_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("tests/cdd/facts/agent_utils.facts");
    if (facts.empty()) {
        std::cerr << "Error: Could not read facts from tests/cdd/facts/agent_utils.facts" << std::endl;
        return 1;
    }
    logging_setup_verification_card(facts);
    return 0;
}
