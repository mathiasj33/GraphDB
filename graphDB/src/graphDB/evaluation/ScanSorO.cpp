#include "ScanSorO.h"

namespace graph_db::evaluation {
    ScanSorO::ScanSorO(const IndexedTripleTable& table, unsigned int first, ScanSorO::Column outputColumn,
                       unsigned int p) : Scan(table, first), outputColumn(outputColumn), p(p) {
        MoveToNextMatchingElement();
    }

    bool ScanSorO::Matches(const Triple& triple) {
        if(triple.p != p) {
            done = true;  // traverse S- or O-list until we encounter triple where p doesn't match
            return false;
        }
        return true;
    }

    void ScanSorO::IncreaseCurrent() {
        switch (outputColumn) {
            case S:
                current = table[current].nextOP;
                break;
            case O:
                current = table[current].nextSP;
                break;
        }
    }
}