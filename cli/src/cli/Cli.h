#pragma once

#include "graphDB/GraphDB.h"
#include <chrono>

namespace graph_db::cli {

    class Cli {
    public:
        explicit Cli(GraphDB& db) : db(db) {}

        void Start();
    private:
        GraphDB& db;
        bool quit = false;

        void ParseAndExecuteCommand(std::string command);
        void ExecuteLoadCommand(const std::string& filename);
        void ExecuteQueryCommand(const std::string& query);
        void ExecuteQuitCommand();
        static double GetElapsedTime(std::chrono::high_resolution_clock::time_point start);
    };

}
