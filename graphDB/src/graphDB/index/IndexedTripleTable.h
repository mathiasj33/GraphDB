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

        bool Contains(unsigned s, unsigned p, unsigned o) const;
        std::unique_ptr<evaluation::Scan> EvaluateS(unsigned p, unsigned o) const;
        std::unique_ptr<evaluation::Scan> EvaluateP(unsigned s, unsigned o) const;
        std::unique_ptr<evaluation::Scan> EvaluateO(unsigned s, unsigned p) const;
        std::unique_ptr<evaluation::Scan> EvaluateSP(unsigned o, bool spEqual) const;
        std::unique_ptr<evaluation::Scan> EvaluateSO(unsigned p, bool soEqual) const;
        std::unique_ptr<evaluation::Scan> EvaluatePO(unsigned s, bool poEqual) const;
        std::unique_ptr<evaluation::Scan> EvaluateSPO(bool spEqual, bool poEqual, bool soEqual) const;

        const Triple& operator[](unsigned index) const;
        unsigned size() const;

    private:
        std::vector<Triple> table;
        std::unordered_set<ThreeHashKey, ThreeHashKey::HashFunction> indexSPO;
        /**
         * Maps to the head of the s-list (first element) and keeps track of its size (second element).
         */
        VectorIndex<std::pair<unsigned, unsigned>> indexS;
        std::unordered_map<TwoHashKey, unsigned, TwoHashKey::HashFunction> indexSP;
        /**
         * Maps to the head of the p-list (first element) and keeps track of its size (second element).
         */
        VectorIndex<std::pair<unsigned, unsigned>> indexO;
        std::unordered_map<TwoHashKey, unsigned, TwoHashKey::HashFunction> indexOP;
        VectorIndex<unsigned> indexP;

        void UpdateSPIndex(Triple& triple, unsigned triplePtr);
        void UpdateOPIndex(Triple& triple, unsigned triplePtr);
        void UpdatePIndex(Triple& triple, unsigned triplePtr);
    };

}
