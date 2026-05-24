#include <iostream>
#include <set>
#include <string>
#include "io/xml_extractor.h"
#include "../cpp/util/fact_utils.h"

using namespace Sorrel::Sdd::Util;

// @Card: xml_extraction_verification
// @Results xml_extraction_operational == true
void xml_extraction_card(const std::map<std::string, std::string>& facts) {
    std::string xml = "<PubmedArticleSet><PubmedArticle><DescriptorName MajorTopicYN=\"N\">Alzheimer Disease</DescriptorName><DescriptorName MajorTopicYN=\"Y\">Amyloid</DescriptorName></PubmedArticle></PubmedArticleSet>";

    std::set<std::string> terms = io::XmlExtractor::extractMeshTerms(xml);

    bool has_alzheimer = terms.count("Alzheimer Disease") > 0;
    bool has_amyloid = terms.count("Amyloid") > 0;
    bool count_ok = (terms.size() == 2);

    bool operational = has_alzheimer && has_amyloid && count_ok;
    std::cout << "xml_extraction_operational = " << (operational ? "true" : "false") << std::endl;
}

int main() {
    auto facts = FactReader::readFacts("mesh_discovery.facts");
    if (facts.empty()) return 1;
    xml_extraction_card(facts);
    return 0;
}
