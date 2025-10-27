#include "cmd_line_parser.h"

CmdLineParser::CmdLineParser(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        tokens.push_back(argv[i]);
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].rfind("--", 0) == 0) {
            std::string option = tokens[i].substr(2);
            if (i + 1 < tokens.size() && tokens[i + 1].rfind("--", 0) != 0) {
                options[option] = tokens[i + 1];
                i++;
            } else {
                options[option] = "";
            }
        }
    }
}

bool CmdLineParser::hasOption(const std::string& option) const {
    return options.count(option);
}

std::string CmdLineParser::getOption(const std::string& option) const {
    if (options.count(option)) {
        return options.at(option);
    }
    return "";
}
