#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <vector>
#include <map>

namespace io {

struct XmlNode {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::vector<XmlNode> children;
    std::string text;
};

class XmlParser {
public:
    static XmlNode parse(const std::string& xml);
    static XmlNode loadFile(const std::string& filepath);

private:
    static std::string getAttribute(const std::string& tag, const std::string& attr);
};

} // namespace io

#endif // XML_PARSER_H
