#include "IndexedTripleTable.h"
#include "Dictionary.h"
#include "graphDB/evaluation/Scan.h"
#include "graphDB/evaluation/ScanTwoVariables.h"
#include "graphDB/evaluation/FullScan.h"
#include "graphDB/evaluation/ScanSorO.h"
#include "graphDB/evaluation/ScanP.h"
#include <cassert>

namespace graph_db::index {

    using namespace evaluation;

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

    const Triple& IndexedTripleTable::operator[](unsigned int index) const {
        return table[index - 1];  // account for indexing starting with 1
    }

    unsigned IndexedTripleTable::size() const {
        return table.size();
    }

    bool IndexedTripleTable::Contains(unsigned int s, unsigned int p, unsigned int o) const {
        return indexSPO.find(ThreeHashKey{s, p, o}) != indexSPO.end();
    }

    std::unique_ptr<evaluation::Scan> IndexedTripleTable::EvaluateS(unsigned int p, unsigned int o) const {
        auto search = indexOP.find(TwoHashKey{o, p});
        unsigned first = search == indexOP.end() ? Dictionary::INVALID_ID : search->second;
        return std::make_unique<ScanSorO>(*this, first, ScanSorO::Column::S, p);
    }

    std::unique_ptr<evaluation::Scan> IndexedTripleTable::EvaluateP(unsigned int s, unsigned int o) const {
        if(!indexS.Contains(s) || !indexO.Contains(o)) {
            return std::make_unique<ScanP>(*this, Dictionary::INVALID_ID, ScanP::ScanList::S, s, o);
        }
        auto[headS, lengthS] = indexS[s];
        auto[headO, lengthO] = indexO[o];
        unsigned head = lengthS < lengthO ? headS : headO;
        auto scanList = head == headS ? ScanP::ScanList::S : ScanP::ScanList::O;
        return std::make_unique<ScanP>(*this, head, scanList, s, o);
    }

    std::unique_ptr<evaluation::Scan> IndexedTripleTable::EvaluateO(unsigned int s, unsigned int p) const {
        auto search = indexSP.find(TwoHashKey{s, p});
        unsigned first = search == indexSP.end() ? Dictionary::INVALID_ID : search->second;
        return std::make_unique<ScanSorO>(*this, first, ScanSorO::Column::O, p);
    }

    std::unique_ptr<evaluation::Scan> IndexedTripleTable::EvaluateSP(unsigned int o, bool spEqual) const {
        unsigned first = indexO.Contains(o) ? indexO[o].first : Dictionary::INVALID_ID;
        return std::make_unique<ScanTwoVariables>(*this, first, spEqual, ScanTwoVariables::ScanList::O);
    }

    std::unique_ptr<evaluation::Scan> IndexedTripleTable::EvaluateSO(unsigned int p, bool soEqual) const {
        unsigned first = indexP.Contains(p) ? indexP[p] : Dictionary::INVALID_ID;
        return std::make_unique<ScanTwoVariables>(*this, first, soEqual, ScanTwoVariables::ScanList::P);
    }

    std::unique_ptr<evaluation::Scan> IndexedTripleTable::EvaluatePO(unsigned int s, bool poEqual) const {
        unsigned first = indexS.Contains(s) ? indexS[s].first : Dictionary::INVALID_ID;
        return std::make_unique<ScanTwoVariables>(*this, first, poEqual, ScanTwoVariables::ScanList::S);
    }

    std::unique_ptr<evaluation::Scan> IndexedTripleTable::EvaluateSPO(bool spEqual, bool poEqual, bool soEqual) const {
        return std::make_unique<FullScan>(*this, 1, spEqual, poEqual, soEqual);
    }
}