#include "ScanP.h"

namespace graph_db::evaluation {
    ScanP::ScanP(const IndexedTripleTable& table, unsigned first, ScanP::ScanList scanList, unsigned s,
                 unsigned o) : Scan(table, first), scanList(scanList), s(s), o(o) {
        MoveToNextMatchingElement();
    }

    bool ScanP::Matches(const Triple& triple) {
        return triple.s == s && triple.o == o;
    }

    void ScanP::IncreaseCurrent() {
        switch (scanList) {
            case S:
                current = table[current].nextSP;
                break;
            case O:
                current = table[current].nextOP;
                break;
        }
    }
}