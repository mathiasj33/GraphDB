#pragma once

#include <string_view>
#include "ResultsTable.h"

namespace graph_db::benchmark {

    class SummaryPrinter {
    public:
        void PrintSummary(const ResultsTable& results) const;

    private:
        int mainWidth = 15;
        int firstWidth = 8;

        void PrintHeader() const;
        void PrintLoadingTimes(const ResultsTable& results) const;
        void PrintMainTable(const ResultsTable& results) const;
        void PrintMainCell(std::string_view content) const;
        void PrintFirstCell(std::string_view content) const;
        void PrintTimeCell(double time) const;
        void PrintQueryCell(unsigned query) const;
        void PrintDelimiter(char delimiter) const;
    };

}
