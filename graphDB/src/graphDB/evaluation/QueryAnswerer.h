#pragma once

#include "graphDB/queryParser/Query.h"
#include "graphDB/index/IndexedTripleTable.h"

namespace graph_db::evaluation {

    using namespace queryParser;
    using namespace index;

    /**
     * The main component for answering SPARQL queries.
     */
    class QueryAnswerer {
    public:
        explicit QueryAnswerer(const IndexedTripleTable& table, const Dictionary& dictionary)
                : table(table), dictionary(dictionary) {}

        void PrintQueryAnswers(const Query& query);
        unsigned CountQueryAnswers(const Query& query);

    private:
        const IndexedTripleTable& table;
        const Dictionary& dictionary;

        unsigned ComputeQueryAnswers(const Query& query, bool print);
        void ComputeQueryAnswers(const Query& query, bool print, unsigned long index,
                                     std::unordered_map<std::string, unsigned>& assignment,
                                     unsigned& count);
        /**
         * Returns a new TriplePattern where all variables that have been assigned in {@param assignment} are
         * replaced with their respective resource ids.
         */
        static TriplePattern UpdateTriplePattern(const TriplePattern& triplePattern,
                                          const std::unordered_map<std::string, unsigned>& assignment);
        static std::variant<unsigned, std::string> GetUpdatedVariant(std::variant<unsigned, std::string> variant,
                                                                     const std::unordered_map<std::string, unsigned>& assignment);
        void PrintAnswer(const Query& query, const std::unordered_map<std::string, unsigned>& assignment);
        std::unique_ptr<Scan> GetScan(const TriplePattern& triplePattern);
        static void UpdateAssignment(const TriplePattern& triplePattern, std::unordered_map<std::string, unsigned>& assignment,
                              unsigned s, unsigned p, unsigned o);
        static void UndoAssignmentUpdate(const TriplePattern& triplePattern,
                                  std::unordered_map<std::string, unsigned>& assignment);
    };

}
