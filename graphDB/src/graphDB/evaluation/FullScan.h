#pragma once

#include <cassert>
#include "Scan.h"

namespace graph_db::evaluation {

    /**
     * A full table scan (with three variables in the query).
     */
    class FullScan : public Scan {
    public:
        FullScan(const IndexedTripleTable& table, unsigned int first, bool spEqual, bool poEqual, bool soEqual);

        ~FullScan() override = default;
        
    protected:
        bool Matches(const Triple& triple) override;
        void IncreaseCurrent() override;

    private:
        bool spEqual;
        bool poEqual;
        bool soEqual;
    };

}
