#pragma once

#include <string>
#include <vector>
#include "TriplePattern.h"

namespace graph_db::queryParser {

    struct Query {
        bool print;
        std::vector<std::string> projectionVariables;
        std::vector<TriplePattern> triplePatterns;
    };

}
