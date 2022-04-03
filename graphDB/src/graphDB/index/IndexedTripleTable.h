#pragma once

#include <vector>
#include <unordered_set>
#include <memory>
#include <unordered_map>
#include "ThreeHashKey.h"
#include "TwoHashKey.h"
#include "VectorIndex.h"
#include "Dictionary.h"
#include "Triple.h"
#include "LinearProbingIndex.h"
#include "RobinHoodIndex.h"

namespace graph_db::evaluation {
    class Scan;
}

namespace graph_db::index {

    /**
     * The main data structure in which triples are stored, following the paper by Motik et al.
     */
    class IndexedTripleTable {
    public:
        IndexedTripleTable() : indexS(std::make_pair(Dictionary::INVALID_ID, Dictionary::INVALID_ID)),
                               indexO(std::make_pair(Dictionary::INVALID_ID, Dictionary::INVALID_ID)),
                               indexP(Dictionary::INVALID_ID) {}

        void Add(unsigned s, unsigned p, unsigned o);

        [[nodiscard]] std::unique_ptr<evaluation::Scan> Contains(unsigned s, unsigned p, unsigned o) const;
        [[nodiscard]] std::unique_ptr<evaluation::Scan> EvaluateS(unsigned p, unsigned o) const;
        [[nodiscard]] std::unique_ptr<evaluation::Scan> EvaluateP(unsigned s, unsigned o) const;
        [[nodiscard]] std::unique_ptr<evaluation::Scan> EvaluateO(unsigned s, unsigned p) const;
        [[nodiscard]] std::unique_ptr<evaluation::Scan> EvaluateSP(unsigned o, bool spEqual) const;
        [[nodiscard]] std::unique_ptr<evaluation::Scan> EvaluateSO(unsigned p, bool soEqual) const;
        [[nodiscard]] std::unique_ptr<evaluation::Scan> EvaluatePO(unsigned s, bool poEqual) const;
        [[nodiscard]] std::unique_ptr<evaluation::Scan> EvaluateSPO(bool spEqual, bool poEqual, bool soEqual) const;

        const Triple& operator[](unsigned index) const;
        unsigned size() const;

    private:
        std::vector<Triple> table;
        LinearProbingIndex<ThreeHashKey, unsigned, ThreeHashKey::SpookyHashFunction> indexSPO;
        /**
         * Maps to the head of the s-list (first element) and keeps track of its size (second element).
         */
        VectorIndex<std::pair<unsigned, unsigned>> indexS;
        LinearProbingIndex<TwoHashKey, unsigned, TwoHashKey::SpookyHashFunction> indexSP;
        /**
         * Maps to the head of the p-list (first element) and keeps track of its size (second element).
         */
        VectorIndex<std::pair<unsigned, unsigned>> indexO;
        LinearProbingIndex<TwoHashKey, unsigned, TwoHashKey::SpookyHashFunction> indexOP;
        VectorIndex<unsigned> indexP;

        void UpdateSPIndex(Triple& triple, unsigned triplePtr);
        void UpdateOPIndex(Triple& triple, unsigned triplePtr);
        void UpdatePIndex(Triple& triple, unsigned triplePtr);
    };

}
