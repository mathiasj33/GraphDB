#include "TriplePattern.h"

namespace graph_db::queryParser {
    bool TriplePattern::SisVariable() const {
        return std::holds_alternative<std::string>(s);
    }

    bool TriplePattern::PisVariable() const {
        return std::holds_alternative<std::string>(p);
    }

    bool TriplePattern::OisVariable() const {
        return std::holds_alternative<std::string>(o);
    }

    unsigned TriplePattern::GetSUnsigned() const {
        return std::get<unsigned>(s);
    }

    unsigned TriplePattern::GetPUnsigned() const {
        return std::get<unsigned>(p);
    }

    unsigned TriplePattern::GetOUnsigned() const {
        return std::get<unsigned>(o);
    }

    std::string TriplePattern::GetSString() const {
        return std::get<std::string>(s);
    }

    std::string TriplePattern::GetPString() const {
        return std::get<std::string>(p);
    }

    std::string TriplePattern::GetOString() const {
        return std::get<std::string>(o);
    }

    bool TriplePattern::operator==(const TriplePattern& rhs) const {
        return s == rhs.s &&
               p == rhs.p &&
               o == rhs.o;
    }

    bool TriplePattern::operator!=(const TriplePattern& rhs) const {
        return !(rhs == *this);
    }
}