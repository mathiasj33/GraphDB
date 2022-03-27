#pragma once

#include <vector>

namespace graph_db::benchmark {
    struct ResultsTable {
        std::vector<std::pair<double, unsigned>> resultsSmall;
        std::vector<std::pair<double, unsigned>> resultsMedium;
        std::vector<std::pair<double, unsigned>> resultsLarge;
        double loadingTimeSmall = 0;
        double loadingTimeMedium = 0;
        double loadingTimeLarge = 0;

        [[nodiscard]] unsigned size() const noexcept {
            return resultsSmall.size();
        }
    };
}