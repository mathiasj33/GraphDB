#pragma once
#include "graphDB/hash/SpookyHash.h"

using namespace graph_db::hash;

namespace graph_db::index {
    struct ThreeHashKey {
        unsigned a;
        unsigned b;
        unsigned c;

        bool operator==(const ThreeHashKey& rhs) const {
            return a == rhs.a &&
                   b == rhs.b &&
                   c == rhs.c;
        }

        bool operator!=(const ThreeHashKey& rhs) const {
            return !(rhs == *this);
        }

        struct HashFunction {
            std::size_t operator()(const ThreeHashKey& key) const {
                std::size_t res = 17;
                res = res * 31 + std::hash<unsigned>()(key.a);
                res = res * 31 + std::hash<unsigned>()(key.b);
                res = res * 31 + std::hash<unsigned>()(key.c);
                return res;
            }
        };

        struct SpookyHashFunction {
            std::size_t operator()(const ThreeHashKey& key) const {
                return SpookyHash::Hash32(&key, sizeof(ThreeHashKey), 17);
            }
        };
    };
}