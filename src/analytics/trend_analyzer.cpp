#include "trend_analyzer.h"
#include <numeric>
#include <algorithm>

namespace analytics {

LogisticParams TrendAnalyzer::fitGrowthModel(const std::vector<double>& series) {
    LogisticParams params;
    if (series.size() < 3) return params;

    double maxVal = *std::max_element(series.begin(), series.end());
    if (maxVal == 0) return params;

    std::vector<double> y(series.size());
    for (size_t i = 0; i < series.size(); ++i) y[i] = series[i] / maxVal;

    // Simple heuristic for logistic parameters
    params.L = 1.0;
    params.x0 = static_cast<double>(series.size()) / 2.0;

    // Estimating k using a simple slope at midpoint
    size_t mid = series.size() / 2;
    if (mid > 0 && mid < series.size() - 1) {
        params.k = (y[mid + 1] - y[mid - 1]) / 2.0;
    } else {
        params.k = 0.1;
    }

    params.valid = true;
    return params;
}

} // namespace analytics
