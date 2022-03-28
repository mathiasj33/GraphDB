#pragma once

#include <unordered_map>
#include <string>
#include <chrono>
#include "graphDB/GraphDB.h"

namespace graph_db::benchmark {

    class LubmBenchmark {
    public:
        enum Size {
            SMALL,
            MEDIUM,
            LARGE
        };
        explicit LubmBenchmark(Size size);

        void Run();
        void RunQuery(int i);
        double GetLoadingTime() const;
        const std::vector<std::pair<double, unsigned>>& GetResults() const;

    private:
        GraphDB db;
        double loadingTime = 0;
        std::vector<std::pair<double, unsigned>> results;

        void RunQuery(int i, const std::unordered_map<std::string, std::string>& queries);
        void LoadData(Size size);
        static std::string GetFilename(Size size);
        static std::unordered_map<std::string, std::string> GetQueries();
        static std::string ReadFile(const std::string& path);
        static double GetElapsedTime(std::chrono::high_resolution_clock::time_point start);
    };

}
