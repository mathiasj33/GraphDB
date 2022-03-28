#pragma once

#include <vector>
#include <cassert>
#include <cstring>

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
        };

        explicit LinearProbingIndex(int initialSize = 16, float maxLoadFactor = 0.5f) : numBuckets(initialSize),
                                                                                        maxLoadFactor(maxLoadFactor),
                                                                                        hashFunction(Hash()),
                                                                                        buckets(new Bucket[numBuckets]),
                                                                                        used(numBuckets, false) {
            assert(maxLoadFactor < 1);
        }

        LinearProbingIndex(const LinearProbingIndex& other) : numBuckets(other.numBuckets),
                                                              maxLoadFactor(other.maxLoadFactor),
                                                              hashFunction(other.hashFunction),
                                                              numEntries(other.numEntries),
                                                              buckets(new Bucket[numBuckets]),
                                                              used(other.used) {
            std::memcpy(buckets, other.buckets, numBuckets);
        }

        LinearProbingIndex(LinearProbingIndex&& other) noexcept
                : numBuckets(other.numBuckets), maxLoadFactor(other.maxLoadFactor), hashFunction(other.hashFunction),
                  numEntries(other.numEntries), buckets(other.buckets), used(other.used) {
            other.numBuckets = 0;
            other.numEntries = 0;
            other.buckets = nullptr;
        }

        ~LinearProbingIndex() {
            delete[] buckets;
        }

        LinearProbingIndex& operator=(const LinearProbingIndex& other) {
            if (this == &other) {
                return *this;
            }
            numEntries = other.numEntries;
            maxLoadFactor = other.maxLoadFactor;
            hashFunction = other.hashFunction;
            used = std::move(other.used);
            if (numBuckets != other.numBuckets) {
                delete[] buckets;
                numBuckets = other.numBuckets;
                buckets = new Bucket[numBuckets];
            }
            std::memcpy(buckets, other.buckets, numBuckets);
            return *this;
        }

        LinearProbingIndex& operator=(LinearProbingIndex&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            numBuckets = other.numBuckets;
            maxLoadFactor = other.maxLoadFactor;
            hashFunction = other.hashFunction;
            numEntries = other.numEntries;
            used = std::move(other.used);

            delete[] buckets;
            buckets = other.buckets;

            other.numBuckets = 0;
            other.numEntries = 0;
            other.buckets = nullptr;

            return *this;
        }

        V& operator[](const K& k) {
            K copy = k;
            return (*this)[std::move(copy)];
        }

        V& operator[](K&& k) {
            unsigned index = ProbeBucket(k);
            Bucket& bucket = buckets[index];
            if (used[index]) {
                return bucket.second;
            }
            // insert
            if (GetLoadFactor(numEntries + 1) > maxLoadFactor) {
                Rehash();
                return (*this)[k];
            }
            ++numEntries;
            bucket.first = std::move(k);
            used[index] = true;
            return bucket.second;
        }

        const Bucket* find(const K& k) const {
            unsigned index = ProbeBucket(k);
            if (!used[index]) {
                return nullptr;
            }
            return &buckets[index];
        }

        const Bucket* end() const {
            return nullptr;
        }

        [[nodiscard]] unsigned size() const {
            return numEntries;
        }

        [[nodiscard]] float loadFactor() const {
            return static_cast<float>(numEntries) / numBuckets;
        }

    private:
        unsigned numBuckets;
        float maxLoadFactor;
        Hash hashFunction;
        unsigned numEntries = 0;
        Bucket* buckets;
        std::vector<bool> used;

        unsigned ComputeHash(const K& k) const {
            return hashFunction(k) % numBuckets;
        }

        unsigned ProbeBucket(const K& k) const {
            unsigned bucketIndex = ComputeHash(k);
            while (used[bucketIndex] && buckets[bucketIndex].first != k) {
                bucketIndex = (bucketIndex + 1) % numBuckets;
            }
            return bucketIndex;
        }

        [[nodiscard]] float GetLoadFactor(unsigned numEntries) const {
            return static_cast<float>(numEntries) / numBuckets;
        }

        void Rehash() {
            Bucket* oldBuckets = buckets;
            auto oldUsed = std::move(used);
            auto oldNumEntries = numEntries;
            auto oldNumBuckets = numBuckets;
            numBuckets *= 2;
            buckets = new Bucket[numBuckets];
            used = std::vector<bool>(numBuckets, false);
            for (int i = 0; i < oldNumBuckets; ++i) {
                Bucket& bucket = oldBuckets[i];
                if (oldUsed[i]) {
                    (*this)[std::move(bucket.first)] = std::move(bucket.second);
                }
            }
            numEntries = oldNumEntries;
            delete[] oldBuckets;
        }
    };

}
