#include "QueryOptimiser.h"
#include <algorithm>
#include <cassert>

namespace graph_db::queryOptimiser {

    Query QueryOptimiser::OptimiseQuery(const Query& query) {
        std::vector<TriplePattern> plan;
        plan.reserve(query.triplePatterns.size());
        std::unordered_set<std::string> boundVariables;
        std::unordered_set<TriplePattern, TriplePattern::HashFunction> remainingPatterns;
        remainingPatterns.reserve(query.triplePatterns.size());
        for (const TriplePattern& pattern : query.triplePatterns) {
            remainingPatterns.insert(pattern);
        }

        while (!remainingPatterns.empty()) {
            const TriplePattern* bestPattern;
            unsigned bestSelectivity = 0;
            for (const TriplePattern& pattern : remainingPatterns) {
                unsigned selectivity = ComputeSelectivity(pattern, boundVariables);
                // TODO: explain modification to the algorithm. provide example query.
                if (pattern.GetVariables().empty() || ContainsBoundVariables(pattern, boundVariables)) {
                    selectivity += 10;
                }
                if (selectivity >= bestSelectivity) {
                    bestPattern = &pattern;
                    bestSelectivity = selectivity;
                }
            }
            plan.push_back(*bestPattern);
            for (const auto& var : bestPattern->GetVariables()) {
                boundVariables.insert(var);
            }
            remainingPatterns.erase(*bestPattern);
        }
        return Query{query.projectionVariables, std::move(plan)};
    }

    unsigned QueryOptimiser::ComputeSelectivity(const TriplePattern& pattern,
                                                const std::unordered_set<std::string>& boundVariables) {
        bool sVariable = pattern.SisVariable() && boundVariables.find(pattern.GetSString()) == boundVariables.end();
        bool pVariable = pattern.PisVariable() && boundVariables.find(pattern.GetPString()) == boundVariables.end();
        bool oVariable = pattern.OisVariable() && boundVariables.find(pattern.GetOString()) == boundVariables.end();
        return SelectivityRelation(sVariable, pVariable, oVariable);
    }

    unsigned QueryOptimiser::SelectivityRelation(bool sv, bool pv, bool ov) {
        if (!sv && !pv && !ov) {
            return 7;
        }
        if (!sv && pv && !ov) {
            return 6;
        }
        if (sv && !pv && !ov) {
            return 5;
        }
        if (!sv && !pv && ov) {
            return 4;
        }
        if (sv && pv && !ov) {
            return 3;
        }
        if (!sv && pv && ov) {
            return 2;
        }
        if (sv && !pv && ov) {
            return 1;
        }
        if (sv && pv && ov) {
            return 0;
        }
        assert(false);
    }

    bool QueryOptimiser::ContainsBoundVariables(const TriplePattern& pattern,
                                                const std::unordered_set<std::string>& boundVariables) {
        const auto& variables = pattern.GetVariables();
        return std::any_of(variables.begin(), variables.end(), [&](const std::string& var) {
            return boundVariables.find(var) != boundVariables.end();
        });
    }
}