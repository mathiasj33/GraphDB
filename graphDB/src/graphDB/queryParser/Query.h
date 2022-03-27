#pragma once

#include <string>
#include <vector>
#include "TriplePattern.h"

namespace graph_db::query_parser {

    struct Query {
        bool print;
        std::vector<std::string> projectionVariables;
        std::vector<TriplePattern> triplePatterns;
    };

}
