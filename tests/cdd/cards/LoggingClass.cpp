#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <map>
#include "../cpp/util/fact_utils.h"

namespace fs = std::filesystem;
using namespace Chai::Cdd::Util;

// @Card: logging_setup_verification
// @Is python_available == true
// @Results logging_setup_operational == true
void logging_setup_verification_card(const std::map<std::string, std::string>& facts) {
    std::string log_file = facts.at("log_file_path");
    std::string message = facts.at("test_message");

    // We use a small python snippet to trigger the setup_logging and log a message
    std::string python_cmd = "python3 -c \"from agent_utils.logging_utils import setup_logging; import logging; setup_logging('" + log_file + "'); logging.info('" + message + "')\"";

    std::system(python_cmd.c_str());

    bool operational = false;
    if (fs::exists(log_file)) {
        std::ifstream f(log_file);
        std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        if (content.find(message) != std::string::npos && content.find("INFO") != std::string::npos) {
            operational = true;
        }
        f.close();
        fs::remove(log_file);
    }

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
