#include "xml_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace io {

XmlNode XmlParser::loadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return {};
    std::stringstream ss;
    ss << file.rdbuf();
    return parse(ss.str());
}

XmlNode XmlParser::parse(const std::string& xml) {
    XmlNode root;
    root.name = "Root";

    size_t pos = 0;
    while ((pos = xml.find("<", pos)) != std::string::npos) {
        if (xml[pos + 1] == '?' || xml[pos + 1] == '!') {
            pos = xml.find(">", pos);
            continue;
        }

        size_t tagEnd = xml.find(">", pos);
        if (tagEnd == std::string::npos) break;

        std::string fullTag = xml.substr(pos + 1, tagEnd - pos - 1);

        // Handle closing tags
        if (fullTag[0] == '/') {
            pos = tagEnd + 1;
            continue;
        }

        XmlNode node;
        size_t spacePos = fullTag.find(" ");
        if (spacePos == std::string::npos) {
            node.name = fullTag;
        } else {
            node.name = fullTag.substr(0, spacePos);
            std::string attrs = fullTag.substr(spacePos + 1);

            // Basic attribute parsing
            size_t attrPos = 0;
            while ((attrPos = attrs.find("=", attrPos)) != std::string::npos) {
                size_t keyStart = attrs.find_last_of(" ", attrPos);
                if (keyStart == std::string::npos) keyStart = 0;
                else keyStart++;

                std::string key = attrs.substr(keyStart, attrPos - keyStart);
                size_t valStart = attrs.find("\"", attrPos);
                if (valStart == std::string::npos) break;
                valStart++;
                size_t valEnd = attrs.find("\"", valStart);
                if (valEnd == std::string::npos) break;

                node.attributes[key] = attrs.substr(valStart, valEnd - valStart);
                attrPos = valEnd + 1;
            }
        }

        root.children.push_back(node);
        pos = tagEnd + 1;
    }

    return root;
}

} // namespace io
