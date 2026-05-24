#ifndef TREND_ANALYZER_H
#define TREND_ANALYZER_H

#include <vector>
#include <cmath>
#include <string>

namespace analytics {

struct LogisticParams {
    double L = 0;
    double k = 0;
    double x0 = 0;
    bool valid = false;
};

class TrendAnalyzer {
public:
    static void loadConfig(const std::string& filepath);
    static LogisticParams fitGrowthModel(const std::vector<double>& series);
};

} // namespace analytics

#endif // TREND_ANALYZER_H
