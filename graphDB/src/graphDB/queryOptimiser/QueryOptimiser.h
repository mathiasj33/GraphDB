#pragma once

#include <unordered_set>
#include "graphDB/queryParser/Query.h"
#include <memory>

using namespace graph_db::queryParser;

namespace graph_db::queryOptimiser {

    class QueryOptimiser {
    public:
        static Query OptimiseQuery(const Query& query);

    private:
        static unsigned
        ComputeSelectivity(const TriplePattern& pattern, const std::unordered_set<std::string>& boundVariables);
        static unsigned SelectivityRelation(bool sv, bool pv, bool ov);
        static bool
        ContainsBoundVariables(const TriplePattern& pattern, const std::unordered_set<std::string>& boundVariables);
    };

}
