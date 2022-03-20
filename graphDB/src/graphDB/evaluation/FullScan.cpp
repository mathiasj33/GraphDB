#include "FullScan.h"

namespace graph_db::evaluation {
    FullScan::FullScan(const IndexedTripleTable& table, unsigned first, bool spEqual, bool poEqual, bool soEqual) :
            Scan(table, first), spEqual(spEqual), poEqual(poEqual), soEqual(soEqual) {
        MoveToNextMatchingElement();
    }

    bool FullScan::Matches(const Triple& triple) {
        if (spEqual && triple.s != triple.p) return false;
        if (poEqual && triple.p != triple.o) return false;
        if (soEqual && triple.s != triple.o) return false;
        return true;
    }

    void FullScan::IncreaseCurrent() {
        ++current;
    }
}