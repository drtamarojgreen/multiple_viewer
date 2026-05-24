#include "trend_analyzer.h"
#include "../io/yaml_parser.h"
#include <numeric>
#include <algorithm>

namespace analytics {

static double g_initialL = 1.0;
static double g_initialK = 0.1;
static double g_midpointDivisor = 2.0;

void TrendAnalyzer::loadConfig(const std::string& filepath) {
    auto cfg = io::YamlParser::loadSimpleYaml(filepath);
    g_initialL = io::YamlParser::getFloat(cfg, "initial_l", g_initialL);
    g_initialK = io::YamlParser::getFloat(cfg, "initial_k", g_initialK);
    g_midpointDivisor = io::YamlParser::getFloat(cfg, "midpoint_divisor", g_midpointDivisor);
}

LogisticParams TrendAnalyzer::fitGrowthModel(const std::vector<double>& series) {
    LogisticParams params;
    if (series.size() < 3) return params;

    double maxVal = *std::max_element(series.begin(), series.end());
    if (maxVal == 0) return params;

    std::vector<double> y(series.size());
    for (size_t i = 0; i < series.size(); ++i) y[i] = series[i] / maxVal;

    // Simple heuristic for logistic parameters
    params.L = g_initialL;
    params.x0 = static_cast<double>(series.size()) / g_midpointDivisor;

    // Estimating k using a simple slope at midpoint
    size_t mid = series.size() / 2;
    if (mid > 0 && mid < series.size() - 1) {
        params.k = (y[mid + 1] - y[mid - 1]) / 2.0;
    } else {
        params.k = g_initialK;
    }

    params.valid = true;
    return params;
}

} // namespace analytics
