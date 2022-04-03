#include <iostream>
#include "QueryAnswerer.h"
#include <cassert>
#include "Scan.h"

namespace graph_db::evaluation {
    unsigned QueryAnswerer::ComputeQueryAnswers(const Query& query) {
        std::unordered_map<std::string, unsigned> assignment;
        unsigned count = 0;
        if (query.print) {
            std::cout << "----------" << std::endl;
            PrintVariableNames(query);
        }
        ComputeQueryAnswers(query, 0, assignment, count);
        if (query.print) {
            std::cout << "----------" << std::endl;
        }
        return count;
    }

    void QueryAnswerer::PrintVariableNames(const Query& query) {
        for (unsigned long i = 0; i < query.projectionVariables.size(); ++i) {
            std::cout << '?' << query.projectionVariables[i];
            if (i == query.projectionVariables.size() - 1) {
                std::cout << std::endl;
            } else {
                std::cout << '\t';
            }
        }
    }

    void QueryAnswerer::ComputeQueryAnswers(const Query& query, unsigned long index,
                                            std::unordered_map<std::string, unsigned>& assignment,
                                            unsigned& count) {
        if (index >= query.triplePatterns.size()) {
            if (query.print) {
                PrintAnswer(query, assignment);
            }
            ++count;
        } else {
            const TriplePattern& pattern = query.triplePatterns[index];
            ScanInformation scanInfo = GetScanInformation(pattern, assignment);
            std::unique_ptr<Scan> scan = GetScan(scanInfo);
            while (!scan->ReachedEnd()) {
                unsigned s, p, o;
                scan->GetNext(s, p, o);
                UpdateAssignment(pattern, scanInfo, s, p, o, assignment);
                ComputeQueryAnswers(query, index + 1, assignment, count);
                UndoAssignmentUpdate(pattern, scanInfo, assignment);
            }
        }
    }

    QueryAnswerer::ScanInformation QueryAnswerer::GetScanInformation(const TriplePattern& triplePattern,
                                                                     const std::unordered_map<std::string, unsigned>& assignment) {
        ScanInformation scanInfo;
        scanInfo.spEqual = triplePattern.s == triplePattern.p;
        scanInfo.poEqual = triplePattern.p == triplePattern.o;
        scanInfo.soEqual = triplePattern.s == triplePattern.o;
        UpdateScanInformation(triplePattern.s, scanInfo.sVariable, scanInfo.s, assignment);
        UpdateScanInformation(triplePattern.p, scanInfo.pVariable, scanInfo.p, assignment);
        UpdateScanInformation(triplePattern.o, scanInfo.oVariable, scanInfo.o, assignment);
        return scanInfo;
    }

    void QueryAnswerer::UpdateScanInformation(const std::variant<unsigned, std::string>& variant, bool& varOut,
                                              unsigned& vOut,
                                              const std::unordered_map<std::string, unsigned>& assignment) {
        if (std::holds_alternative<std::string>(variant)) {
            auto search = assignment.find(std::get<std::string>(variant));
            if (search != assignment.end()) {
                varOut = false;
                vOut = search->second;
            } else {
                varOut = true;
            }
        } else {
            varOut = false;
            vOut = std::get<unsigned>(variant);
        }
    }

    void
    QueryAnswerer::PrintAnswer(const Query& query, const std::unordered_map<std::string, unsigned>& assignment) {
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

    std::unique_ptr<Scan> QueryAnswerer::GetScan(const ScanInformation& scanInfo) {
        bool sv = scanInfo.sVariable;
        bool pv = scanInfo.pVariable;
        bool ov = scanInfo.oVariable;

        if (sv && pv && ov) {
            return table.EvaluateSPO(scanInfo.spEqual, scanInfo.poEqual, scanInfo.soEqual);
        } else if (sv && pv && !ov) {
            return table.EvaluateSP(scanInfo.o, scanInfo.spEqual);
        } else if (sv && !pv && ov) {
            return table.EvaluateSO(scanInfo.p, scanInfo.soEqual);
        } else if (sv && !pv && !ov) {
            return table.EvaluateS(scanInfo.p, scanInfo.o);
        } else if (!sv && pv && ov) {
            return table.EvaluatePO(scanInfo.s, scanInfo.poEqual);
        } else if (!sv && pv && !ov) {
            return table.EvaluateP(scanInfo.s, scanInfo.o);
        } else if (!sv && !pv && ov) {
            return table.EvaluateO(scanInfo.s, scanInfo.p);
        } else if (!sv && !pv && !ov) {
            return table.Contains(scanInfo.s, scanInfo.p, scanInfo.o);
        }
        assert(false);
    }

    void QueryAnswerer::UpdateAssignment(const TriplePattern& triplePattern,
                                         const ScanInformation& scanInfo,
                                         unsigned s, unsigned p, unsigned o,
                                         std::unordered_map<std::string, unsigned>& assignment) {
        if (scanInfo.sVariable) {
            assignment[triplePattern.GetSString()] = s;
        }
        if (scanInfo.pVariable) {
            assignment[triplePattern.GetPString()] = p;
        }
        if (scanInfo.oVariable) {
            assignment[triplePattern.GetOString()] = o;
        }
    }

    void QueryAnswerer::UndoAssignmentUpdate(const TriplePattern& triplePattern,
                                             const ScanInformation& scanInfo,
                                             std::unordered_map<std::string, unsigned>& assignment) {
        if (scanInfo.sVariable) {
            assignment.erase(triplePattern.GetSString());
        }
        if (scanInfo.pVariable) {
            assignment.erase(triplePattern.GetPString());
        }
        if (scanInfo.oVariable) {
            assignment.erase(triplePattern.GetOString());
        }
    }
}