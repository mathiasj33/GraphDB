#include "LubmBenchmark.h"
#include <filesystem>
#include <iostream>
#include <fstream>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;
using namespace graph_db;

namespace graph_db::benchmark {
    LubmBenchmark::LubmBenchmark(LubmBenchmark::Size size) {
        std::cout.precision(2);
        std::cout << std::fixed;
        LoadData(size);
    }

    void LubmBenchmark::Run() {
        auto queries = GetQueries();
        for (long unsigned i = 0; i < queries.size(); ++i) {
            RunQuery(i + 1, queries);
        }
    }

    void LubmBenchmark::RunQuery(int i) {
        RunQuery(i, GetQueries());
    }

    void LubmBenchmark::RunQuery(int i, const std::unordered_map<std::string, std::string>& queries) {
        std::cout << "Evaluating query " << i << "." << std::endl;
        char queryFilename[20];
        sprintf(queryFilename, "q%02d.txt", i);
        auto search = queries.find(queryFilename);
        assert(search != queries.end());
        const auto& query = search->second;

        auto start = high_resolution_clock::now();
        unsigned count = db.ComputeQueryAnswers(query, true);
        double elapsedTime = GetElapsedTime(start);

        std::cout << count << " results returned in " << elapsedTime << "ms." << std::endl << std::endl;
        results.emplace_back(elapsedTime, count);
    }

    void LubmBenchmark::LoadData(LubmBenchmark::Size size) {
        std::cout << "Loading data." << std::endl;
        auto start = high_resolution_clock::now();
        unsigned count = db.LoadFile(GetFilename(size));
        loadingTime = GetElapsedTime(start);
        std::cout << count << " triples loaded in " << loadingTime << "ms." << std::endl << std::endl;
    }

    std::string LubmBenchmark::GetFilename(LubmBenchmark::Size size) {
        switch (size) {
            case SMALL:
                return "data/LUBM-001-mat.ttl";
            case MEDIUM:
                return "data/LUBM-010-mat.ttl";
            case LARGE:
                return "data/LUBM-100-mat.ttl";
        }
        assert(false);
        return "";
    }

    std::unordered_map<std::string, std::string> LubmBenchmark::GetQueries() {
        std::unordered_map<std::string, std::string> queries;
        for(const auto& entry : std::filesystem::directory_iterator("data/queries")) {
            queries[entry.path().filename()] = ReadFile(entry.path());
        }
        return queries;
    }

    std::string LubmBenchmark::ReadFile(const std::string& path) {
        std::ifstream input(path);
        std::stringstream string;
        while(input >> string.rdbuf());
        return string.str();
    }

    double LubmBenchmark::GetElapsedTime(std::chrono::high_resolution_clock::time_point start) {
        auto end = high_resolution_clock::now();
        duration<double, std::milli> timeElapsed = end - start;
        return timeElapsed.count();
    }

    double LubmBenchmark::GetLoadingTime() const {
        return loadingTime;
    }

    const std::vector<std::pair<double, unsigned>>& LubmBenchmark::GetResults() const {
        return results;
    }
}