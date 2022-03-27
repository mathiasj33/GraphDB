#include <vector>
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

    std::vector<std::string> TriplePattern::GetVariables() const {
        std::vector<std::string> variables;
        if(SisVariable()) {
            variables.emplace_back(GetSString());
        }
        if(PisVariable()) {
            variables.emplace_back(GetPString());
        }
        if(OisVariable()) {
            variables.emplace_back(GetOString());
        }
        return variables;
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