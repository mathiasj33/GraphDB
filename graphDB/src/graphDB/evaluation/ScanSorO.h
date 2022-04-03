#pragma once

#include "Scan.h"

namespace graph_db::evaluation {

    class ScanSorO : public Scan {
    public:
        enum Column {
            S,
            O
        };

        ScanSorO(const IndexedTripleTable& table, unsigned first, Column outputColumn, unsigned p);

        ~ScanSorO() override = default;
        
    protected:
        bool Matches(const Triple& triple) override;
        void IncreaseCurrent() override;

    private:
        Column outputColumn;
        unsigned p;
    };

}
