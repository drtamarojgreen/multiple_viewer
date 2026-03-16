#include <iostream>
#include <string>
#include <fstream>
#include <filesystem> // Required for C++17 filesystem operations

namespace fs = std::filesystem;

// Function to find the 'cdd' directory by looking upwards from the current executable's path
fs::path find_cdd_directory() {
    fs::path current_path = fs::current_path();
    while (current_path.has_parent_path()) {
        if (fs::exists(current_path / "cdd") && fs::is_directory(current_path / "cdd")) {
            return current_path / "cdd";
        }
        if (current_path == current_path.parent_path()) { // Reached root
            break;
        }
        current_path = current_path.parent_path();
    }
    return {}; // Return empty path if not found
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "sip") {
        std::cout << "SIP RULES" << std::endl;
        std::cout << std::endl;
        std::cout << "1. Implement one capability" << std::endl;
        std::cout << "2. Write minimal code" << std::endl;
        std::cout << "3. Provide compile instructions" << std::endl;
        std::cout << "4. Stop after increment" << std::endl;
    } else if (argc > 1 && std::string(argv[1]) == "facts") {
        std::cout << "CHAI Facts" << std::endl;
        std::cout << std::endl;
        // This command no longer uses hardcoded paths for this sip
        std::cout << "(Fact file discovery not yet implemented, use 'discover cdd')" << std::endl;
    } else if (argc > 2 && std::string(argv[1]) == "discover") {
        if (std::string(argv[2]) == "cdd") {
            fs::path cdd_dir = find_cdd_directory();
            if (!cdd_dir.empty()) {
                std::cout << "cdd_directory_found = true" << std::endl;
                std::cout << "cdd_directory_path = " << cdd_dir << std::endl;
            } else {
                std::cout << "cdd_directory_found = false" << std::endl;
                std::cout << "cdd_directory_path = (not found)" << std::endl;
            }
        } else if (std::string(argv[2]) == "facts") { // New logic for 'discover facts'
            fs::path cdd_dir = find_cdd_directory();
            if (!cdd_dir.empty()) {
                fs::path facts_dir_path = cdd_dir / "facts";
                if (fs::exists(facts_dir_path) && fs::is_directory(facts_dir_path)) {
                    std::cout << "facts_directory_found = true" << std::endl;
                    std::cout << "facts_directory_path = " << facts_dir_path << std::endl;
                } else {
                    std::cout << "facts_directory_found = false" << std::endl;
                    std::cout << "facts_directory_path = (not found)" << std::endl;
                }
            } else {
                std::cout << "cdd_directory_found = false" << std::endl;
                std::cout << "cdd_directory_path = (not found - cannot check for facts directory)" << std::endl;
                std::cout << "facts_directory_found = false" << std::endl;
                std::cout << "facts_directory_path = (not found)" << std::endl;
            }
        } else { // Handle unknown discover target
            std::cout << "Unknown discover target: " << argv[2] << std::endl;
        }
    } else {
        std::cout << "CHAI CLI" << std::endl;
    }
    return 0;
}
