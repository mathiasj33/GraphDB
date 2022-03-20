#pragma once

#include <vector>
#include <cassert>

namespace graph_db::index {

    /**
     * An index mapping ids to Vs implemented by a simple vector.
     */
    template<class V>
    class VectorIndex {
    public:
        /**
         * @param invalidValue The value that is stored in buckets that are actually empty.
         */
        explicit VectorIndex(V invalidValue) : invalidValue(invalidValue) {};
        [[nodiscard]] bool Contains(unsigned k) const {
            return k < buckets.size() && buckets[k] != invalidValue;
        }

        V& operator[](unsigned k) {
            while (k >= buckets.size()) {
                buckets.push_back(invalidValue);
            }
            return buckets[k];
        }

        const V& operator[](unsigned k) const {
            assert(k < buckets.size());
            return buckets[k];
        }

    private:
        V invalidValue;
        std::vector<V> buckets;
    };

}
