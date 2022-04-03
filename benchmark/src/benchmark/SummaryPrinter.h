#pragma once

#include <string_view>
#include "ResultsTable.h"

namespace graph_db::benchmark {

    class SummaryPrinter {
    public:
        static void PrintSummary(const ResultsTable& results);

    private:
        const static int MAIN_WIDTH = 15;
        const static int FIRST_WIDTH = 8;

        static void PrintHeader();
        static void PrintLoadingTimes(const ResultsTable& results);
        static void PrintMainTable(const ResultsTable& results);
        static void PrintMainCell(std::string_view content);
        static void PrintFirstCell(std::string_view content);
        static void PrintTimeCell(double time);
        static void PrintQueryCell(unsigned query);
        static void PrintDelimiter(char delimiter);
    };

}
