#ifndef XML_EXTRACTOR_H
#define XML_EXTRACTOR_H

#include <string>
#include <vector>
#include <set>

namespace io {

class XmlExtractor {
public:
    static std::set<std::string> extractMeshTerms(const std::string& xml) {
        std::set<std::string> terms;
        std::string tagStart = "<DescriptorName";
        std::string tagEnd = "</DescriptorName>";

        size_t pos = 0;
        while ((pos = xml.find(tagStart, pos)) != std::string::npos) {
            size_t contentStart = xml.find('>', pos);
            if (contentStart == std::string::npos) break;
            contentStart++;

            size_t contentEnd = xml.find(tagEnd, contentStart);
            if (contentEnd == std::string::npos) break;

            std::string term = xml.substr(contentStart, contentEnd - contentStart);
            // Basic trim
            size_t first = term.find_first_not_of(" \t\r\n");
            if (first != std::string::npos) {
                size_t last = term.find_last_not_of(" \t\r\n");
                terms.insert(term.substr(first, (last - first + 1)));
            }

            pos = contentEnd + tagEnd.size();
        }
        return terms;
    }
};

} // namespace io

#endif // XML_EXTRACTOR_H
