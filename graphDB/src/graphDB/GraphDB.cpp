#include "GraphDB.h"
#include <cstdio>
#include <iostream>
#include "graphDB/queryOptimiser/QueryOptimiser.h"

using namespace graph_db::query_optimiser;

namespace graph_db {
    unsigned GraphDB::LoadFile(const std::string& filename) {
        unsigned count = 0;
        FILE* file = fopen(filename.c_str(), "r");
        if(file == nullptr) {
            std::cout << "Error opening file." << std::endl;
            return 0;
        }
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];  // TODO: assumption: no lines longer than 1024 characters
        while(fgets(buffer, BUFFER_SIZE, file) != nullptr) {
            unsigned s, p, o;
            if(!fileParser.ParseLine(buffer, BUFFER_SIZE, s, p, o)) {
                std::cout << "Error parsing input file." << std::endl;
                break;
            }
            if(table.Add(s, p, o)) {
                ++count;
            }
        }
        fclose(file);
        return count;
    }

    unsigned GraphDB::ComputeQueryAnswers(std::string_view queryString, bool countOnly) {
        Query query = queryParser.ParseQuery(queryString);
        if(countOnly) {
            query.print = false;
        }
        Query optimisedQuery = QueryOptimiser::OptimiseQuery(query);
        return queryAnswerer.ComputeQueryAnswers(optimisedQuery);
    }
}