#include "Cli.h"
#include <iostream>
#include <algorithm>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

namespace graph_db::cli {
    void Cli::Start() {
        std::cout.precision(2);
        std::cout << std::fixed;
        std::cout << "Welcome to GraphDB v1.0!" << std::endl;
        while (!quit) {
            std::cout << "> ";
            std::string commandString;
            std::getline(std::cin, commandString);
            ParseAndExecuteCommand(std::move(commandString));
        }
    }

    void Cli::ParseAndExecuteCommand(std::string command) {
        int spacePosition = command.find(' ');
        if (spacePosition > 0) {
            std::string firstWord = command.substr(0, spacePosition);
            std::transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::tolower);
            try {
                if (firstWord == "load") {
                    ExecuteLoadCommand(command.substr(spacePosition + 1, command.size()));
                    return;
                } else if (firstWord == "select" || firstWord == "count") {
                    ExecuteQueryCommand(command);
                    return;
                }
            } catch (const std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                return;
            }
        } else {
            std::transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "quit") {
                ExecuteQuitCommand();
                return;
            }
        }
        std::cout << "Unrecognised command." << std::endl;
    }

    void Cli::ExecuteLoadCommand(const std::string& filename) {
        auto start = high_resolution_clock::now();
        unsigned count = db.LoadFile(filename);
        double elapsedTime = GetElapsedTime(start);
        std::cout << count << " triples loaded in " << elapsedTime << "ms." << std::endl;
    }

    void Cli::ExecuteQueryCommand(const std::string& query) {
        auto start = high_resolution_clock::now();
        unsigned count = db.ComputeQueryAnswers(query);
        double elapsedTime = GetElapsedTime(start);
        std::cout << count << " results returned in " << elapsedTime << "ms." << std::endl;
    }

    void Cli::ExecuteQuitCommand() {
        std::cout << "Shutting down." << std::endl;
        quit = true;
    }

    double Cli::GetElapsedTime(std::chrono::high_resolution_clock::time_point start) {
        auto end = high_resolution_clock::now();
        duration<double, std::milli> timeElapsed = end - start;
        return timeElapsed.count();
    }
}