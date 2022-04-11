#pragma once

#include <string>
#include <vector>
#include "TriplePattern.h"

namespace graph_db::query_parser {

    struct Query {
        bool print = true;
        std::vector<std::string> projectionVariables;
        std::vector<TriplePattern> triplePatterns;
    };

}
