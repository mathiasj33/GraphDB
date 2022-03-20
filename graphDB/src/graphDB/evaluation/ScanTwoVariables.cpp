#include "ScanTwoVariables.h"

namespace graph_db::evaluation {
    ScanTwoVariables::ScanTwoVariables(const IndexedTripleTable& table, unsigned int first, bool equal,
                                       ScanTwoVariables::ScanList scanList)
            : Scan(table, first), equal(equal), scanList(scanList) {
        MoveToNextMatchingElement();
    }

    bool ScanTwoVariables::Matches(const Triple& triple) {
        if (!equal) {
            return true;
        }
        switch (scanList) {
            case S:
                return triple.p == triple.o;
            case P:
                return triple.s == triple.o;
            case O:
                return triple.s == triple.p;
            default:
                return false;
        }
    }

    void ScanTwoVariables::IncreaseCurrent() {
        switch (scanList) {
            case S:
                current = table[current].nextSP;
                break;
            case P:
                current = table[current].nextP;
                break;
            case O:
                current = table[current].nextOP;
                break;
        }
    }
}