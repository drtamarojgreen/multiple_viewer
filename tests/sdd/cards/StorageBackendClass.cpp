#include <iostream>
#include <string>
#include <filesystem>
#include "io/io_manager.h"
#include "../cpp/util/fact_utils.h"

namespace fs = std::filesystem;
using namespace Sorrel::Sdd::Util;

// Mock Backend for testing
class MockBackend : public io::StorageBackend {
public:
    bool read(const std::string& path, std::string& outData) override {
        if (files.count(path)) {
            outData = files.at(path);
            return true;
        }
        return false;
    }
    bool write(const std::string& path, const std::string& inData) override {
        files[path] = inData;
        return true;
    }
    std::map<std::string, std::string> files;
};

// @Card: storage_backend_verification
// @Results storage_backend_operational == true
void storage_backend_card(const std::map<std::string, std::string>& facts) {
    MockBackend mock;
    io::IOManager::setBackend(&mock);

    std::string test_path = "mock://test";
    std::string test_data = facts.at("test_data");
    if (!test_data.empty() && test_data.front() == '"') test_data = test_data.substr(1, test_data.size()-2);

    bool written = mock.write(test_path, test_data);

    std::string read_data;
    bool read = mock.read(test_path, read_data);

    bool operational = written && read && (read_data == test_data);

    // Test IOManager using backend
    Graph g;
    mock.write("mock://json", "{\"nodes\": [{\"label\": \"Mock\", \"index\": 0}]}");
    bool json_loaded = io::IOManager::loadJSON(g, "mock://json");
    bool node_exists = g.nodeExists(0);
    operational = operational && json_loaded && node_exists;

    std::cout << "storage_backend_operational = " << (operational ? "true" : "false") << std::endl;

    io::IOManager::setBackend(nullptr);
}

int main() {
    auto facts = FactReader::readFacts("storage_backend.facts");
    if (facts.empty()) return 1;
    storage_backend_card(facts);
    return 0;
}
