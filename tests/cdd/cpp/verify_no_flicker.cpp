#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

bool check_no_cout(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    std::string line;
    bool inComment = false;
    while (std::getline(file, line)) {
        // Simple comment skipping
        size_t commentStart = line.find("//");
        if (commentStart != std::string::npos) line = line.substr(0, commentStart);

        if (line.find("std::cout") != std::string::npos || line.find("cout <<") != std::string::npos) {
            // Allow cout in init or shutdown if specifically marked, but not in render
            if (line.find("DEBUG") == std::string::npos && line.find("Logger") == std::string::npos) {
                std::cerr << "Found forbidden cout in " << filepath << ": " << line << std::endl;
                return false;
            }
        }
    }
    return true;
}

int main() {
    bool ok = true;
    ok &= check_no_cout("src/render/console_renderer.cpp");
    // We expect some cout in viewer_logic for prompt handling, but not in the main loop.
    // This is a simplified check.

    if (ok) {
        std::cout << "CDD Card PASSED: No direct cout in rendering core." << std::endl;
        return 0;
    } else {
        std::cerr << "CDD Card FAILED: Direct cout found in rendering core, potential flickering." << std::endl;
        return 1;
    }
}
