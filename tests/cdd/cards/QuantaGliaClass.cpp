#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <map>
#include "../cpp/util/fact_utils.h"

namespace fs = std::filesystem;
using namespace Chai::Cdd::Util;

// @Card: quanta_glia_extraction
// @Is python_available == true
// @Results quanta_glia_extraction_operational == true
void quanta_glia_extraction_card(const std::map<std::string, std::string>& facts) {
    std::string repo_name = facts.at("repo_prefix") + "_extract";
    fs::path repo_path(repo_name);

    // Setup: Create a dummy repository with a README
    fs::create_directory(repo_path);
    {
        std::ofstream readme(repo_path / "README.md");
        readme << "Test Readme Content";
    }

    // Execute Quanta Glia script via system call
    std::string command = "python3 scripts/quanta_glia.py " + repo_name + " > /dev/null 2>&1";
    int result = std::system(command.c_str());

    // Verify: Check if the extracted file exists in the knowledge base
    fs::path kb_path = fs::path("knowledge_base") / repo_name / "README.md";
    bool operational = (result == 0) && fs::exists(kb_path);

    std::cout << "quanta_glia_extraction_operational = " << (operational ? "true" : "false") << std::endl;

    // Cleanup
    fs::remove_all(repo_path);
    if (fs::exists("knowledge_base/" + repo_name)) {
        fs::remove_all("knowledge_base/" + repo_name);
    }
}

// @Card: max_repos_limit_verification
// @Is python_available == true
// @Results quanta_glia_max_repos_limit_operational == true
void max_repos_limit_verification_card(const std::map<std::string, std::string>& facts) {
    int max_repos = std::stoi(facts.at("max_repos"));
    int num_to_create = max_repos + 1;
    std::string repo_prefix = facts.at("repo_prefix") + "_max_";
    std::string repo_list = "";

    for (int i = 1; i <= num_to_create; ++i) {
        std::string repo_name = repo_prefix + std::to_string(i);
        fs::create_directory(repo_name);
        std::ofstream readme(fs::path(repo_name) / "README.md");
        readme << "test";
        readme.close();
        repo_list += " " + repo_name;
    }

    std::string command = "python3 scripts/quanta_glia.py " + repo_list + " > /dev/null 2>&1";
    std::system(command.c_str());

    int count = 0;
    if (fs::exists("knowledge_base")) {
        for (auto const& dir_entry : fs::directory_iterator("knowledge_base")) {
            if (dir_entry.is_directory() && dir_entry.path().filename().string().find(repo_prefix) == 0) {
                count++;
            }
        }
    }

    std::cout << "quanta_glia_max_repos_limit_operational = " << (count == max_repos ? "true" : "false") << std::endl;

    // Cleanup
    for (int i = 1; i <= num_to_create; ++i) {
        std::string repo_name = repo_prefix + std::to_string(i);
        fs::remove_all(repo_name);
        if (fs::exists("knowledge_base/" + repo_name)) {
            fs::remove_all("knowledge_base/" + repo_name);
        }
    }
}

// @Card: custom_config_verification
// @Is python_available == true
// @Results quanta_glia_custom_config_operational == true
void custom_config_verification_card(const std::map<std::string, std::string>& facts) {
    std::string repo_name = facts.at("repo_prefix") + "_custom";
    std::string config_file = facts.at("custom_config_path");
    fs::path repo_path(repo_name);

    fs::create_directory(repo_path);
    {
        std::ofstream f(repo_path / "README.md");
        f << "should be skipped";
    }
    {
        std::ofstream f(repo_path / "IMPORTANT_FILE.txt");
        f << "should be extracted";
    }

    std::string command = "python3 scripts/quanta_glia.py --config " + config_file + " " + repo_name + " > /dev/null 2>&1";
    std::system(command.c_str());

    fs::path kb_path = fs::path("knowledge_base") / repo_name;
    bool important_exists = fs::exists(kb_path / "IMPORTANT_FILE.txt");
    bool readme_exists = fs::exists(kb_path / "README.md");

    std::cout << "quanta_glia_custom_config_operational = " << (important_exists && !readme_exists ? "true" : "false") << std::endl;

    // Cleanup
    fs::remove_all(repo_path);
    if (fs::exists(kb_path)) {
        fs::remove_all(kb_path);
    }
}

// @Card: no_target_files_verification
// @Is python_available == true
// @Results quanta_glia_no_target_files_operational == true
void no_target_files_verification_card(const std::map<std::string, std::string>& facts) {
    std::string repo_name = facts.at("repo_prefix") + "_no_targets";
    fs::path repo_path(repo_name);

    fs::create_directory(repo_path);
    {
        std::ofstream f(repo_path / "some_other_file.txt");
        f << "This is not a target file.";
    }

    std::string command = "python3 scripts/quanta_glia.py " + repo_name + " > /dev/null 2>&1";
    std::system(command.c_str());

    fs::path kb_path = fs::path("knowledge_base") / repo_name;
    bool kb_exists = fs::exists(kb_path);

    std::cout << "quanta_glia_no_target_files_operational = " << (!kb_exists ? "true" : "false") << std::endl;

    // Cleanup
    fs::remove_all(repo_path);
    if (kb_exists) {
        fs::remove_all(kb_path);
    }
}

// @Card: repo_cloning_verification
// @Is python_available == true
// @Results quanta_glia_clone_repo_operational == true
void repo_cloning_verification_card(const std::map<std::string, std::string>& facts) {
    std::string src_repo = facts.at("clone_src");
    std::string cache_dir = facts.at("cache_dir");
    fs::create_directory(src_repo);
    { std::ofstream f(fs::path(src_repo) / "a.txt"); f << "test"; }

    fs::create_directory(cache_dir);

    std::string python_cmd = "python3 -c \"from scripts import quanta_glia; from pathlib import Path; quanta_glia.clone_repo('" + src_repo + "', Path('" + cache_dir + "'))\"";
    std::system(python_cmd.c_str());

    bool operational = fs::exists(fs::path(cache_dir) / src_repo / "a.txt");

    std::cout << "quanta_glia_clone_repo_operational = " << (operational ? "true" : "false") << std::endl;

    // Cleanup
    fs::remove_all(src_repo);
    fs::remove_all(cache_dir);
}

// @Card: cache_pruning_verification
// @Is python_available == true
// @Results quanta_glia_prune_cache_operational == true
void cache_pruning_verification_card(const std::map<std::string, std::string>& facts) {
    std::string cache_dir = facts.at("cache_dir") + "_prune";
    fs::create_directory(cache_dir);
    fs::create_directory(fs::path(cache_dir) / "to_delete");
    { std::ofstream f(fs::path(cache_dir) / "keep.txt"); f << "keep"; }

    std::string python_cmd = "python3 -c \"from scripts import quanta_glia; from pathlib import Path; quanta_glia.prune_cache(Path('" + cache_dir + "'))\"";
    std::system(python_cmd.c_str());

    bool dir_deleted = !fs::exists(fs::path(cache_dir) / "to_delete");
    bool file_kept = fs::exists(fs::path(cache_dir) / "keep.txt");

    std::cout << "quanta_glia_prune_cache_operational = " << (dir_deleted && file_kept ? "true" : "false") << std::endl;

    // Cleanup
    fs::remove_all(cache_dir);
}

int main(int argc, char* argv[]) {
    auto facts = FactReader::readFacts("tests/cdd/facts/quanta_glia.facts");
    if (facts.empty()) {
        std::cerr << "Error: Could not read facts from tests/cdd/facts/quanta_glia.facts" << std::endl;
        return 1;
    }

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "max_repos") {
            max_repos_limit_verification_card(facts);
        } else if (arg == "custom_config") {
            custom_config_verification_card(facts);
        } else if (arg == "no_targets") {
            no_target_files_verification_card(facts);
        } else if (arg == "clone") {
            repo_cloning_verification_card(facts);
        } else if (arg == "prune") {
            cache_pruning_verification_card(facts);
        } else {
            quanta_glia_extraction_card(facts);
        }
    } else {
        quanta_glia_extraction_card(facts);
    }
    return 0;
}
