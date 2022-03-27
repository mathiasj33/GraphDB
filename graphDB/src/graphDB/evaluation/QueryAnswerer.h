#pragma once

#include "graphDB/queryParser/Query.h"
#include "graphDB/index/IndexedTripleTable.h"

namespace graph_db::evaluation {

    using namespace query_parser;
    using namespace index;

    /**
     * The main component for answering SPARQL queries.
     */
    class QueryAnswerer {
    public:
        explicit QueryAnswerer(const IndexedTripleTable& table, const Dictionary& dictionary)
                : table(table), dictionary(dictionary) {}

        unsigned ComputeQueryAnswers(const Query& query);

    private:
        const IndexedTripleTable& table;
        const Dictionary& dictionary;

        struct ScanInformation {
            bool sVariable = false;
            bool pVariable = false;
            bool oVariable = false;
            bool spEqual = false;
            bool poEqual = false;
            bool soEqual = false;
            unsigned s = 0;
            unsigned p = 0;
            unsigned o = 0;
        };

        static void PrintVariableNames(const Query& query);

        void ComputeQueryAnswers(const Query& query, unsigned long index,
                                 std::unordered_map<std::string, unsigned>& assignment,
                                 unsigned& count);
        void PrintAnswer(const Query& query, const std::unordered_map<std::string, unsigned>& assignment);
        static ScanInformation GetScanInformation(const TriplePattern& triplePattern,
                                                  const std::unordered_map<std::string, unsigned>& assignment);
        static void
        UpdateScanInformation(const std::variant<unsigned, std::string>& variant, bool& varOut, unsigned& vOut,
                              const std::unordered_map<std::string, unsigned>& assignment);
        std::unique_ptr<Scan> GetScan(const ScanInformation& scanInfo);
        static void
        UpdateAssignment(const TriplePattern& triplePattern, const ScanInformation& scanInfo, unsigned s, unsigned p,
                         unsigned o, std::unordered_map<std::string, unsigned>& assignment);
        static void UndoAssignmentUpdate(const TriplePattern& triplePattern, const ScanInformation& scanInfo,
                                         std::unordered_map<std::string, unsigned>& assignment);
    };

}
