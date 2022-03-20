#pragma once

#include "Scan.h"

namespace graph_db::evaluation {

    /**
     * A scan with two variables in the query (fully scans either the s-, p-, or o-list).
     */
    class ScanTwoVariables : public Scan {
    public:
        enum ScanList {
            S,
            P,
            O
        };

        /**
         * @param equal: denotes whether the two variables in the query are equal, i.e. <s, ?X, ?X>.
         * @param scanList: the list over which to iterate
         */
        ScanTwoVariables(const IndexedTripleTable& table, unsigned int first, bool equal, ScanList scanList);

        ~ScanTwoVariables() override = default;

    protected:
        bool Matches(const Triple& triple) override;
        void IncreaseCurrent() override;

    private:
        bool equal;
        ScanList scanList;
    };

}
