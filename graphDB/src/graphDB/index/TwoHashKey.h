#pragma once

namespace graph_db::index {
    struct TwoHashKey {
        unsigned a;
        unsigned b;

        bool operator==(const TwoHashKey& rhs) const {
            return a == rhs.a &&
                   b == rhs.b;
        }

        bool operator!=(const TwoHashKey& rhs) const {
            return !(rhs == *this);
        }

        struct HashFunction {
            std::size_t operator()(const TwoHashKey& key) const {
                std::size_t res = 17;
                res = res * 31 + std::hash<unsigned>()(key.a);
                res = res * 31 + std::hash<unsigned>()(key.b);
                return res;
            }
        };
    };
}