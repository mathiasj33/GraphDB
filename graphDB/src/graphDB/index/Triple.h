#pragma once

namespace graph_db::index {

    /**
     * An entry in the table, containing dictionary encodings of s, s, p, as well as the next pointers for
     * the linked lists S, P, and O.
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
}
