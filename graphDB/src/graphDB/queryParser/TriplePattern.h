#pragma once

#include <variant>
#include <string>

namespace graph_db::query_parser {

    struct TriplePattern {
        std::variant<unsigned, std::string> s;
        std::variant<unsigned, std::string> p;
        std::variant<unsigned, std::string> o;

        [[nodiscard]] bool SisVariable() const;
        [[nodiscard]] bool PisVariable() const;
        [[nodiscard]] bool OisVariable() const;
        [[nodiscard]] std::vector<std::string> GetVariables() const;
        [[nodiscard]] std::string GetSString() const;
        [[nodiscard]] std::string GetPString() const;
        [[nodiscard]] std::string GetOString() const;
        bool operator==(const TriplePattern& rhs) const;
        bool operator!=(const TriplePattern& rhs) const;

        struct HashFunction {
            std::size_t operator()(const TriplePattern& key) const {
                std::size_t res = 17;
                res = res * 31 + std::hash<std::variant<unsigned, std::string>>()(key.s);
                res = res * 31 + std::hash<std::variant<unsigned, std::string>>()(key.p);
                res = res * 31 + std::hash<std::variant<unsigned, std::string>>()(key.o);
                return res;
            }
        };
    };

}