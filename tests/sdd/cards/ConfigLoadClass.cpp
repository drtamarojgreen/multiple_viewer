#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include "map_logic.h"
#include "../cpp/util/fact_utils.h"

namespace fs = std::filesystem;
using namespace Sorrel::Sdd::Util;

// @Card: config_load_verification
// @Results config_load_operational == true
void config_load_card(const std::map<std::string, std::string>& facts) {
    fs::create_directories("../../tests/temp/");
    std::string test_yaml = "../../tests/temp/test_config.yaml";

    std::ofstream out(test_yaml);
    out << "console_width: 120\n";
    out << "quiet_mode: true\n";
    out.close();

    Config::loadFromYaml(test_yaml);

    bool operational = (Config::consoleWidth == 120) && (Config::quietMode == true);

    std::cout << "config_load_operational = " << (operational ? "true" : "false") << std::endl;

    if (fs::exists(test_yaml)) fs::remove(test_yaml);
}

int main() {
    auto facts = FactReader::readFacts("environment.facts");
    if (facts.empty()) return 1;
    config_load_card(facts);
    return 0;
}
