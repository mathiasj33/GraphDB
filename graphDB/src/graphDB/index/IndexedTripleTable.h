#pragma once

#include <vector>
#include <unordered_set>
#include <memory>
#include <unordered_map>
#include "graphDB/evaluation/Scan.h"
#include "ThreeHashKey.h"
#include "TwoHashKey.h"
#include "VectorIndex.h"
#include "Dictionary.h"

namespace graph_db::index {

    /**
     * The main data structure in which triples are stored, following the paper by Motik et al.
     */
    class IndexedTripleTable {
    public:
        IndexedTripleTable() : indexS(std::make_pair(Dictionary::INVALID_ID, Dictionary::INVALID_ID)),
                               indexO(std::make_pair(Dictionary::INVALID_ID, Dictionary::INVALID_ID)),
                               indexP(Dictionary::INVALID_ID) {}

        /**
         * An entry in the table, containing dictionary encodings of s, p, o, as well as the next pointers for
         * the linked lists SP, P, and OP.
         */
        struct Triple {
            unsigned s;
            unsigned p;
            unsigned o;
            unsigned nextSP;
            unsigned nextP;
            unsigned nextOP;

            bool operator==(const Triple& rhs) const {
                return s == rhs.s &&
                       p == rhs.p &&
                       o == rhs.o &&
                       nextSP == rhs.nextSP &&
                       nextP == rhs.nextP &&
                       nextOP == rhs.nextOP;
            }

            bool operator!=(const Triple& rhs) const {
                return !(rhs == *this);
            }
        };

        void Add(unsigned s, unsigned p, unsigned o);

        std::unique_ptr<Scan> EvaluateSPO(unsigned s, unsigned p, unsigned o) const;
        std::unique_ptr<Scan> EvaluatePO(unsigned p, unsigned o) const;
        std::unique_ptr<Scan> EvaluateSO(unsigned s, unsigned o) const;
        std::unique_ptr<Scan> EvaluateSP(unsigned s, unsigned p) const;
        std::unique_ptr<Scan> EvaluateO(unsigned o, bool equal) const;
        std::unique_ptr<Scan> EvaluateP(unsigned p, bool equal) const;
        std::unique_ptr<Scan> EvaluateS(unsigned s, bool equal) const;
        std::unique_ptr<Scan> Evaluate(bool spEqual, bool poEqual) const;

        const Triple& operator[](unsigned index) const;

    private:
        std::vector<Triple> table;
        std::unordered_set<ThreeHashKey, ThreeHashKey::HashFunction> indexSPO;
        /**
         * Maps to the head of the s-list (first element) and keeps track of its size (second element).
         */
        VectorIndex<std::pair<unsigned, unsigned>> indexS;
        std::unordered_map<TwoHashKey, unsigned, TwoHashKey::HashFunction> indexSP;
        /**
         * Maps to the head of the o-list (first element) and keeps track of its size (second element).
         */
        VectorIndex<std::pair<unsigned, unsigned>> indexO;
        std::unordered_map<TwoHashKey, unsigned, TwoHashKey::HashFunction> indexOP;
        VectorIndex<unsigned> indexP;

        void UpdateSPIndex(Triple& triple, unsigned triplePtr);
        void UpdateOPIndex(Triple& triple, unsigned triplePtr);
        void UpdatePIndex(Triple& triple, unsigned triplePtr);
    };

}
