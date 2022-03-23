#pragma once

#include <variant>
#include <string>

namespace graph_db::queryParser {

    struct TriplePattern {
        std::variant<unsigned, std::string> s;
        std::variant<unsigned, std::string> p;
        std::variant<unsigned, std::string> o;

        [[nodiscard]] bool SisVariable() const;
        [[nodiscard]] bool PisVariable() const;
        [[nodiscard]] bool OisVariable() const;
        [[nodiscard]] unsigned GetSUnsigned() const;
        [[nodiscard]] unsigned GetPUnsigned() const;
        [[nodiscard]] unsigned GetOUnsigned() const;
        [[nodiscard]] std::string GetSString() const;
        [[nodiscard]] std::string GetPString() const;
        [[nodiscard]] std::string GetOString() const;
        bool operator==(const TriplePattern& rhs) const;
        bool operator!=(const TriplePattern& rhs) const;
    };

}