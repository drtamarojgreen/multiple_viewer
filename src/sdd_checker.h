#ifndef SDD_CHECKER_H
#define SDD_CHECKER_H

#include <string>
#include <vector>
#include <filesystem>

struct SddCheckResult {
    bool passed;
    std::string message;
    std::vector<std::string> details;
};

class SddChecker {
public:
    SddCheckResult checkRepository(const std::string& pathOrUrl);

private:
    std::string cloneRepository(const std::string& url);
    bool isUrl(const std::string& str);

    SddCheckResult performChecks(const std::filesystem::path& repoPath);
    bool checkDirectoryStructure(const std::filesystem::path& repoPath, std::vector<std::string>& details);
    bool checkMandatoryFiles(const std::filesystem::path& repoPath, std::vector<std::string>& details);
    bool checkFactsFormat(const std::filesystem::path& repoPath, std::vector<std::string>& details);
    bool checkCardsFormat(const std::filesystem::path& repoPath, std::vector<std::string>& details);
    bool checkCheckoutArtifacts(const std::filesystem::path& repoPath, std::vector<std::string>& details);
};

#endif // SDD_CHECKER_H
