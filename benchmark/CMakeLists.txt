SET(BENCHMARK_SOURCES
        src/benchmark/main.cpp src/benchmark/LubmBenchmark.cpp src/benchmark/LubmBenchmark.h src/benchmark/SummaryPrinter.cpp src/benchmark/SummaryPrinter.h src/benchmark/ResultsTable.h)

add_executable(benchmark ${BENCHMARK_SOURCES})
target_link_libraries(benchmark PRIVATE graphDB)