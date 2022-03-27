#include "LubmBenchmark.h"
#include "SummaryPrinter.h"

using namespace graph_db::benchmark;

void
RunBenchmark(LubmBenchmark::Size size, double& loadingTimeOut, std::vector<std::pair<double, unsigned>>& resultOut) {
    LubmBenchmark benchmark{size};
    benchmark.Run();
    loadingTimeOut = benchmark.GetLoadingTime();
    resultOut = benchmark.GetResults();
}

int main() {
    ResultsTable results;
    RunBenchmark(LubmBenchmark::Size::SMALL, results.loadingTimeSmall, results.resultsSmall);
    RunBenchmark(LubmBenchmark::Size::MEDIUM, results.loadingTimeMedium, results.resultsMedium);
    RunBenchmark(LubmBenchmark::Size::LARGE, results.loadingTimeLarge, results.resultsLarge);

    SummaryPrinter printer;
    printer.PrintSummary(results);

    return 0;
}
