#include "ScanSingleValue.h"

namespace graph_db::evaluation {

    bool ScanSingleValue::Matches(const Triple& triple) {
        return true;
    }

    void ScanSingleValue::IncreaseCurrent() {
        done = true;
    }

}