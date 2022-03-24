#include "GraphDB.h"
#include <cstdio>
#include <iostream>

namespace graph_db {
    void GraphDB::LoadFile(const std::string& filename) {
        FILE* file = fopen(filename.c_str(), "r");
        if(file == nullptr) {
            std::cout << "Error opening file." << std::endl;
            return;
        }
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];  // TODO: assumption: no lines longer than 1024 characters
        while(fgets(buffer, BUFFER_SIZE, file) != nullptr) {
            unsigned s, p, o;
            if(!fileParser.ParseLine(buffer, BUFFER_SIZE, s, p, o)) {
                std::cout << "Error parsing input file." << std::endl;
                break;
            }
            table.Add(s, p, o);
        }
        fclose(file);
    }

    void GraphDB::PrintQueryAnswers(std::string_view queryString) {
        Query query = queryParser.ParseQuery(queryString);
        queryAnswerer.PrintQueryAnswers(query);
    }
}