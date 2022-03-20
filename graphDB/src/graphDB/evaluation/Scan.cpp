#include "Scan.h"

namespace graph_db::evaluation {

    void Scan::GetNext(unsigned int& sOut, unsigned int& pOut, unsigned int& oOut) {
        assert(!ReachedEnd() && Matches(table[current]));

        const Triple& t = table[current];
        sOut = t.s;
        pOut = t.p;
        oOut = t.o;
        IncreaseCurrent();
        MoveToNextMatchingElement();
    }

    void Scan::MoveToNextMatchingElement() {
        while (!ReachedEnd() && !Matches(table[current])) {
            IncreaseCurrent();
        }
    }

    bool Scan::ReachedEnd() const {
        return current > table.size() ||  // note that indexing starts with 1 (> instead of >=)
               current == Dictionary::INVALID_ID ||  // if the end of a list has been reached
               done;
    }

}