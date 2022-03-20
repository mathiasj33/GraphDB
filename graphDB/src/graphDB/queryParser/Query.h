#pragma once

#include <string>
#include <vector>
#include "TriplePattern.h"

namespace graph_db::queryParser {

    struct Query {
        std::vector<std::string> projectionVariables;
        std::vector<TriplePattern> triplePatterns;
    };

}
