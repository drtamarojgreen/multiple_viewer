#include <iostream>
#include "src/io/xml_parser.h"

int main() {
    auto root = io::XmlParser::loadFile("rules/commands.xml");
    for (const auto& child : root.children) {
        if (child.name == "Command") {
            std::cout << "Key: " << child.attributes.at("key") << " Action: " << child.attributes.at("action") << std::endl;
        }
    }
    return 0;
}
