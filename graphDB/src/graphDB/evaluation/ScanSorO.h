#pragma once

#include "Scan.h"

namespace graph_db::evaluation {

    class ScanSorO : public Scan {
    public:
        enum Column {
            S,
            O
        };

        ScanSorO(const IndexedTripleTable& table, unsigned int first, Column outputColumn, unsigned int p);

        ~ScanSorO() override = default;
        
    protected:
        bool Matches(const Triple& triple) override;
        void IncreaseCurrent() override;

    private:
        Column outputColumn;
        unsigned p;
    };

}
