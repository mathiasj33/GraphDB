#pragma once

#include "Scan.h"

namespace graph_db::evaluation {

    class ScanP : public Scan {
    public:
        enum ScanList {
            S,
            O
        };

        ScanP(const IndexedTripleTable& table, unsigned int first, ScanList scanList, unsigned int s, unsigned int o);

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
