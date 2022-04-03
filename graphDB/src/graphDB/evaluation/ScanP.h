#pragma once

#include "Scan.h"

namespace graph_db::evaluation {

    class ScanP : public Scan {
    public:
        enum ScanList {
            S,
            O
        };

        ScanP(const IndexedTripleTable& table, unsigned first, ScanList scanList, unsigned s, unsigned o);

        ~ScanP() override = default;

    protected:
        bool Matches(const Triple& triple) override;
        void IncreaseCurrent() override;

    private:
        ScanList scanList;
        unsigned s;
        unsigned o;
    };

}
