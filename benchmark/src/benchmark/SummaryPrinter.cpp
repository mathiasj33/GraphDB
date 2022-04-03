#include "SummaryPrinter.h"
#include <iostream>
#include <iomanip>

namespace graph_db::benchmark {
    void SummaryPrinter::PrintSummary(const ResultsTable& results) {
        PrintHeader();
        PrintLoadingTimes(results);
        PrintMainTable(results);
    }

    void SummaryPrinter::PrintHeader() {
        PrintFirstCell(" ");
        PrintMainCell("LUBM-001");
        PrintMainCell("LUBM-010");
        PrintMainCell("LUBM-100");
        PrintDelimiter('=');
    }

    void SummaryPrinter::PrintLoadingTimes(const ResultsTable& results) {
        PrintFirstCell("Time");
        PrintTimeCell(results.loadingTimeSmall);
        PrintTimeCell(results.loadingTimeMedium);
        PrintTimeCell(results.loadingTimeLarge);
        PrintDelimiter('=');
    }

    void SummaryPrinter::PrintMainTable(const ResultsTable& results) {
        for (unsigned i = 0; i < results.size(); ++i) {
            PrintQueryCell(i + 1);
            PrintTimeCell(results.resultsSmall[i].first);
            PrintTimeCell(results.resultsMedium[i].first);
            PrintTimeCell(results.resultsLarge[i].first);
            std::cout << "\n";
            PrintFirstCell("Count");
            PrintMainCell(std::to_string(results.resultsSmall[i].second));
            PrintMainCell(std::to_string(results.resultsMedium[i].second));
            PrintMainCell(std::to_string(results.resultsLarge[i].second));
            PrintDelimiter('-');
        }
    }

    void SummaryPrinter::PrintMainCell(std::string_view content) {
        std::cout << std::setw(MAIN_WIDTH) << content;
    }

    void SummaryPrinter::PrintFirstCell(std::string_view content) {
        std::cout << std::setw(FIRST_WIDTH) << std::left << content << std::right;
    }

    void SummaryPrinter::PrintTimeCell(double time) {
        char c[16];
        sprintf(c, "%.2fms", time);
        PrintMainCell(c);
    }

    void SummaryPrinter::PrintQueryCell(unsigned query) {
        std::string queryString = "Query " + std::to_string(query);
        PrintFirstCell(queryString);
    }

    void SummaryPrinter::PrintDelimiter(char delimiter) {
        std::cout << '\n' << std::string(FIRST_WIDTH + 3 * MAIN_WIDTH, delimiter) << std::endl;
    }
}