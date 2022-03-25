#include <iostream>
#include "QueryAnswerer.h"
#include <cassert>
#include "Scan.h"

namespace graph_db::evaluation {
    unsigned QueryAnswerer::ComputeQueryAnswers(const Query& query) {
        std::unordered_map<std::string, unsigned int> assignment;
        unsigned count = 0;
        if(query.print) {
            std::cout << "----------" << std::endl;
            PrintVariableNames(query);
        }
        ComputeQueryAnswers(query, 0, assignment, count);
        if(query.print) {
            std::cout << "----------" << std::endl;
        }
        return count;
    }

    void QueryAnswerer::PrintVariableNames(const Query& query) {
        for (unsigned long i = 0; i < query.projectionVariables.size(); ++i) {
            std::cout << '?' << query.projectionVariables[i];
            if(i == query.projectionVariables.size() - 1) {
                std::cout << std::endl;
            } else {
                std::cout << '\t';
            }
        }
    }

    void QueryAnswerer::ComputeQueryAnswers(const Query& query, unsigned long index,
                                            std::unordered_map<std::string, unsigned int>& assignment,
                                            unsigned& count) {
        if (index >= query.triplePatterns.size()) {
            if (query.print) {
                PrintAnswer(query, assignment);
            }
            ++count;
        } else {
            const TriplePattern& pattern = query.triplePatterns[index];
            const TriplePattern updatedPattern = UpdateTriplePattern(pattern, assignment);
            std::unique_ptr<Scan> scan = GetScan(updatedPattern);
            while (!scan->ReachedEnd()) {
                unsigned s, p, o;
                scan->GetNext(s, p, o);
                UpdateAssignment(updatedPattern, assignment, s, p, o);
                ComputeQueryAnswers(query, index + 1, assignment, count);
                UndoAssignmentUpdate(updatedPattern, assignment);
            }
        }
    }

    void
    QueryAnswerer::PrintAnswer(const Query& query, const std::unordered_map<std::string, unsigned int>& assignment) {
        for (unsigned long i = 0; i < query.projectionVariables.size(); ++i) {
            const auto& v = query.projectionVariables[i];
            auto search = assignment.find(v);
            assert(search != assignment.end());
            std::cout << dictionary.GetResource(search->second);
            if (i == query.projectionVariables.size() - 1) {
                std::cout << std::endl;
            } else {
                std::cout << '\t';
            }
        }
    }

    TriplePattern QueryAnswerer::UpdateTriplePattern(const TriplePattern& triplePattern,
                                                     const std::unordered_map<std::string, unsigned>& assignment) {
        TriplePattern updatedPattern;
        updatedPattern.s = GetUpdatedVariant(triplePattern.s, assignment);
        updatedPattern.p = GetUpdatedVariant(triplePattern.p, assignment);
        updatedPattern.o = GetUpdatedVariant(triplePattern.o, assignment);
        return updatedPattern;
    }

    std::variant<unsigned, std::string>
    QueryAnswerer::GetUpdatedVariant(std::variant<unsigned int, std::string> variant,
                                     const std::unordered_map<std::string, unsigned int>& assignment) {
        if (std::holds_alternative<std::string>(variant)) {
            auto search = assignment.find(std::get<std::string>(variant));
            if (search != assignment.end()) {
                return search->second;
            }
        }
        return variant;
    }

    std::unique_ptr<Scan> QueryAnswerer::GetScan(const TriplePattern& triplePattern) {
        bool spEqual = triplePattern.s == triplePattern.p;
        bool poEqual = triplePattern.p == triplePattern.o;
        bool soEqual = triplePattern.s == triplePattern.o;

        bool sv = triplePattern.SisVariable();
        bool pv = triplePattern.PisVariable();
        bool ov = triplePattern.OisVariable();

        if (sv && pv && ov) {
            return table.EvaluateSPO(spEqual, poEqual, soEqual);
        } else if (sv && pv && !ov) {
            return table.EvaluateSP(triplePattern.GetOUnsigned(), spEqual);
        } else if (sv && !pv && ov) {
            return table.EvaluateSO(triplePattern.GetPUnsigned(), soEqual);
        } else if (sv && !pv && !ov) {
            return table.EvaluateS(triplePattern.GetPUnsigned(), triplePattern.GetOUnsigned());
        } else if (!sv && pv && ov) {
            return table.EvaluatePO(triplePattern.GetSUnsigned(), poEqual);
        } else if (!sv && pv && !ov) {
            return table.EvaluateP(triplePattern.GetSUnsigned(), triplePattern.GetOUnsigned());
        } else if (!sv && !pv && ov) {
            return table.EvaluateO(triplePattern.GetSUnsigned(), triplePattern.GetPUnsigned());
        } else if (!sv && !pv && !ov) {
            return table.Contains(triplePattern.GetSUnsigned(), triplePattern.GetPUnsigned(),
                                  triplePattern.GetOUnsigned());
        }
        assert(false);
    }

    void QueryAnswerer::UpdateAssignment(const TriplePattern& triplePattern,
                                         std::unordered_map<std::string, unsigned int>& assignment,
                                         unsigned int s, unsigned int p, unsigned int o) {
        if (triplePattern.SisVariable()) {
            assignment[triplePattern.GetSString()] = s;
        }
        if (triplePattern.PisVariable()) {
            assignment[triplePattern.GetPString()] = p;
        }
        if (triplePattern.OisVariable()) {
            assignment[triplePattern.GetOString()] = o;
        }
    }

    void QueryAnswerer::UndoAssignmentUpdate(const TriplePattern& triplePattern,
                                             std::unordered_map<std::string, unsigned int>& assignment) {
        for (const std::string& var: triplePattern.GetVariables()) {
            assignment.erase(var);
        }
    }
}