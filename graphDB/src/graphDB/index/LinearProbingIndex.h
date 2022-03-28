#pragma once

#include <vector>
#include <cassert>

namespace graph_db::index {

    /**
     * An index mapping Ks to Vs.
     */
    template<class K, class V, class Hash>
    class LinearProbingIndex {
    public:
        struct Bucket {
            K first{};
            V second{};
            bool used = false;
        };

        explicit LinearProbingIndex(int initialSize = 16, float maxLoadFactor = 0.5f) : hashFunction(Hash()),
                                                                                        maxLoadFactor(maxLoadFactor) {
            assert(maxLoadFactor < 1);
            buckets.reserve(initialSize);
            for (int i = 0; i < initialSize; ++i) {
                buckets.emplace_back();
            }
        }

        V& operator[](const K& k) {
            K copy = k;
            return (*this)[std::move(copy)];
        }

        V& operator[](K&& k) {
            Bucket& bucket = buckets[ProbeBucket(k)];
            if (bucket.used) {
                return bucket.second;
            }
            // insert
            if (GetLoadFactor(numEntries + 1) > maxLoadFactor) {
                Rehash();
                return (*this)[k];
            }
            ++numEntries;
            bucket.first = std::move(k);
            bucket.used = true;
            return bucket.second;
        }

        const Bucket* find(const K& k) const {
            const Bucket* bucket = &buckets[ProbeBucket(k)];
            if (!bucket->used) {
                return nullptr;
            }
            return bucket;
        }

        const Bucket* end() const {
            return nullptr;
        }

        [[nodiscard]] unsigned size() const {
            return numEntries;
        }

        [[nodiscard]] float loadFactor() const {
            return static_cast<float>(numEntries) / buckets.size();
        }

    private:
        Hash hashFunction;
        float maxLoadFactor;
        unsigned numEntries = 0;
        std::vector<Bucket> buckets;

        unsigned ComputeHash(const K& k) const {
            return hashFunction(k) % buckets.size();
        }

        unsigned ProbeBucket(const K& k) const {
            unsigned bucketIndex = ComputeHash(k);
            while (buckets[bucketIndex].used && buckets[bucketIndex].first != k) {
                bucketIndex = (bucketIndex + 1) % buckets.size();
            }
            return bucketIndex;
        }

        [[nodiscard]] float GetLoadFactor(unsigned numEntries) const {
            return static_cast<float>(numEntries) / buckets.size();
        }

        void Rehash() {
            auto oldBuckets = std::move(buckets);
            auto oldNumEntries = numEntries;
            buckets = std::vector<Bucket>();
            buckets.reserve(oldBuckets.size() * 2);
            for (auto i = 0; i < oldBuckets.size() * 2; ++i) {
                buckets.emplace_back();
            }
            for (auto& bucket : oldBuckets) {
                if (bucket.used) {
                    (*this)[std::move(bucket.first)] = std::move(bucket.second);
                }
            }
            numEntries = oldNumEntries;
        }
    };

}
