#include "IndexedTripleTable.h"
#include "Dictionary.h"
#include <cassert>

namespace graph_db::index {

    void IndexedTripleTable::Add(unsigned s, unsigned p, unsigned o) {
        ThreeHashKey key{s, p, o};
        if (indexSPO.find(key) != indexSPO.end()) {
            return;
        }
        indexSPO.insert(key);

        Triple triple{s, p, o, Dictionary::INVALID_ID, Dictionary::INVALID_ID, Dictionary::INVALID_ID};

        // points to the new triple (once inserted)
        // note that we start indexing with 1, so that 0 can be used to indicate the end of a list
        unsigned triplePtr = table.size() + 1;
        UpdateSPIndex(triple, triplePtr);
        UpdateOPIndex(triple, triplePtr);
        UpdatePIndex(triple, triplePtr);

        table.push_back(triple);
    }

    void IndexedTripleTable::UpdateSPIndex(Triple& triple, unsigned triplePtr) {
        unsigned s = triple.s;
        unsigned p = triple.p;
        TwoHashKey spKey{s, p};
        auto search = indexSP.find(spKey);
        if (search != indexSP.end()) {
            Triple& before = table[search->second - 1];  // if we find a match, insert the current triple afterwards
            triple.nextSP = before.nextSP;
            before.nextSP = triplePtr;
            assert(indexS.Contains(s));
            indexS[s].second++;
        } else {
            indexSP[spKey] = triplePtr;
            if (indexS.Contains(s)) {
                auto&[headS, length] = indexS[s];  // if indexS is not empty, insert as the new head
                triple.nextSP = headS;
                headS = triplePtr;
                ++length;
            } else {
                indexS[s] = std::make_pair(triplePtr, 1);
            }
        }
    }

    void IndexedTripleTable::UpdateOPIndex(Triple& triple, unsigned int triplePtr) {
        unsigned o = triple.o;
        unsigned p = triple.p;
        TwoHashKey opKey{o, p};
        auto search = indexOP.find(opKey);
        if (search != indexOP.end()) {
            Triple& before = table[search->second - 1];
            triple.nextOP = before.nextOP;
            before.nextOP = triplePtr;
            assert(indexO.Contains(o));
            indexO[o].second++;
        } else {
            indexOP[opKey] = triplePtr;
            if (indexO.Contains(o)) {
                auto&[headO, length] = indexO[o];
                triple.nextOP = headO;
                headO = triplePtr;
                ++length;
            } else {
                indexO[o] = std::make_pair(triplePtr, 1);
            }
        }
    }

    void IndexedTripleTable::UpdatePIndex(Triple& triple, unsigned int triplePtr) {
        unsigned p = triple.p;
        if (indexP.Contains(p)) {
            triple.nextP = indexP[p];
        }
        indexP[p] = triplePtr;
    }

    const IndexedTripleTable::Triple& IndexedTripleTable::operator[](unsigned int index) const {
        return table[index];
    }
}