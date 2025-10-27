#ifndef CMD_LINE_PARSER_H
#define CMD_LINE_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>

class CmdLineParser {
public:
    CmdLineParser(int argc, char* argv[]);
    bool hasOption(const std::string& option) const;
    std::string getOption(const std::string& option) const;

private:
    std::vector<std::string> tokens;
    std::unordered_map<std::string, std::string> options;
};

#endif // CMD_LINE_PARSER_H
