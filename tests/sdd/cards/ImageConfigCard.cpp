#include <iostream>
#include <map>
#include "io/image_manager.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

void verify_config_batch(const std::map<std::string, std::string>& facts) {
    io::ImageManager manager;

    std::cout << "v62_configuration_parser_rejects_malformed = " << (manager.testConfiguration("parse_malformed") ? "true" : "false") << std::endl;
    std::cout << "v63_configuration_defaults_load_absent = " << (manager.testConfiguration("load_defaults") ? "true" : "false") << std::endl;
    std::cout << "v64_runtime_configuration_reload_safe = " << (manager.testConfiguration("hot_reload") ? "true" : "false") << std::endl;
    std::cout << "v65_cli_arguments_override_persisted = " << (manager.testCLI("override") ? "true" : "false") << std::endl;
    std::cout << "v66_invalid_cli_arguments_error_msg = " << (manager.testCLI("invalid") ? "true" : "false") << std::endl;
    std::cout << "v67_headless_mode_suppresses_gui = " << (manager.testCLI("headless") ? "true" : "false") << std::endl;
    std::cout << "v98_ci_pipeline_executes_tests = " << (manager.testLifecycle("ci_check") ? "true" : "false") << std::endl;
    std::cout << "v99_release_build_strips_debug = " << (manager.testLifecycle("strip_debug") ? "true" : "false") << std::endl;
    std::cout << "v100_repository_reproducible_build = " << (manager.testLifecycle("reproducible") ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("image_env.facts");
    verify_config_batch(facts);
    return 0;
}
