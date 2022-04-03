#pragma once

#include "Scan.h"

namespace graph_db::evaluation {

    /**
     * A scan returning only a single value (or potentially no values). Needed to conform to the interface when
     * evaluating queries without any variables.
     */
    class ScanSingleValue : public Scan {
    public:
        ScanSingleValue(const IndexedTripleTable& table, unsigned first) : Scan(table, first) {}

        ~ScanSingleValue() override = default;
    protected:
        bool Matches(const Triple& triple) override;
        void IncreaseCurrent() override;
    };

}
