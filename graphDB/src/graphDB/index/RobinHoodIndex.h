#pragma once

#include <vector>
#include <cassert>
#include <cstring>

namespace graph_db::index {

    /**
     * An index mapping Ks to Vs.
     */
    template<class K, class V, class Hash>
    class RobinHoodIndex {
    public:
        struct Bucket {
            K first{};
            V second{};
        };

        explicit RobinHoodIndex(int initialSize = 16, float maxLoadFactor = 0.95f) : numBuckets(initialSize),
                                                                                    maxLoadFactor(maxLoadFactor),
                                                                                    hashFunction(Hash()),
                                                                                    buckets(new Bucket[numBuckets]),
                                                                                    hashValues(new unsigned[numBuckets]) {
            assert(maxLoadFactor < 1);
            std::fill(hashValues, hashValues + numBuckets, 0);
        }

        RobinHoodIndex(const RobinHoodIndex& other) : numBuckets(other.numBuckets),
                                                      maxLoadFactor(other.maxLoadFactor),
                                                      hashFunction(other.hashFunction),
                                                      numEntries(other.numEntries),
                                                      buckets(new Bucket[numBuckets]),
                                                      hashValues(new unsigned[numBuckets]) {
            std::memcpy(buckets, other.buckets, numBuckets);
            std::memcpy(hashValues, other.hashValues, numBuckets);
        }

        RobinHoodIndex(RobinHoodIndex&& other) noexcept
                : numBuckets(other.numBuckets), maxLoadFactor(other.maxLoadFactor), hashFunction(other.hashFunction),
                  numEntries(other.numEntries), buckets(other.buckets), hashValues(other.hashValues) {
            other.numBuckets = 0;
            other.numEntries = 0;
            other.buckets = nullptr;
            other.hashValues = nullptr;
        }

        ~RobinHoodIndex() {
            delete[] buckets;
            delete[] hashValues;
        }

        RobinHoodIndex& operator=(const RobinHoodIndex& other) {
            if (this == &other) {
                return *this;
            }
            numEntries = other.numEntries;
            maxLoadFactor = other.maxLoadFactor;
            hashFunction = other.hashFunction;
            if (numBuckets != other.numBuckets) {
                delete[] buckets;
                delete[] hashValues;
                numBuckets = other.numBuckets;
                buckets = new Bucket[numBuckets];
                hashValues = new Bucket[numBuckets];
            }
            std::memcpy(buckets, other.buckets, numBuckets);
            std::memcpy(hashValues, other.hashValues, numBuckets);
            return *this;
        }

        RobinHoodIndex& operator=(RobinHoodIndex&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            numBuckets = other.numBuckets;
            maxLoadFactor = other.maxLoadFactor;
            hashFunction = other.hashFunction;
            numEntries = other.numEntries;

            delete[] buckets;
            delete[] hashValues;
            buckets = other.buckets;
            hashValues = other.hashValues;

            other.numBuckets = 0;
            other.numEntries = 0;
            other.buckets = nullptr;
            other.hashValues = nullptr;

            return *this;
        }

        V& operator[](const K& k) {
            K copy = k;
            return (*this)[std::move(copy)];
        }

        V& operator[](K&& k) {
            unsigned hash = ModifiedHashFunction(k);
            unsigned current = hash % numBuckets;
            unsigned distance = 0;
            while(true) {
                unsigned currentHash = hashValues[current];
                if(currentHash == hash && buckets[current].first == k) {
                    return buckets[current].second;
                }
                if(currentHash == 0) {
                    if (GetLoadFactor(numEntries + 1) > maxLoadFactor) {
                        Rehash();
                        return (*this)[k];
                    }
                    ++numEntries;
                    buckets[current].first = std::move(k);
                    hashValues[current] = hash;
                    return buckets[current].second;
                }
                unsigned existingDistance = ComputeProbeDistance(current, currentHash % numBuckets);
                if (existingDistance < distance) {  // insert
                    if (GetLoadFactor(numEntries + 1) > maxLoadFactor) {
                        Rehash();
                        return (*this)[k];
                    }
                    ++numEntries;
                    Bucket& bucket = buckets[current];
                    std::swap(hash, hashValues[current]);
                    std::swap(k, bucket.first);
                    V v{};
                    std::swap(v, bucket.second);
                    UpdatePosition(std::move(k), std::move(v), current, hash, existingDistance);
                    return bucket.second;
                }
                current = (current + 1) % numBuckets;
                ++distance;
            }
        }

        const Bucket* find(const K& k) const {
            unsigned hash = ModifiedHashFunction(k);
            unsigned current = hash % numBuckets;
            unsigned distance = 0;
            while(true) {
                unsigned currentHash = hashValues[current];
                if(currentHash == 0) {
                    return nullptr;
                }
                if(distance > ComputeProbeDistance(current, currentHash % numBuckets)) {
                    return nullptr;
                }
                if(currentHash == hash && buckets[current].first == k) {
                    return &buckets[current];
                }
                current = (current + 1) % numBuckets;
                ++distance;
            }
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
        unsigned* hashValues;

        unsigned ModifiedHashFunction(const K& k) const {
            auto hash = hashFunction(k);
            if(hash == 0) {
                return 1;
            }
            return hash;
        }

        [[nodiscard]] unsigned ComputeProbeDistance(unsigned pos, unsigned desiredPos) const {
            if(desiredPos < pos) {
                return pos - desiredPos;
            }
            return numBuckets - desiredPos + pos;
        }

        void UpdatePosition(K&& k, V&& v, unsigned originalPos, unsigned hash, unsigned distance) {
            unsigned current = (originalPos + 1) % numBuckets;
            ++distance;
            while(true) {
                unsigned currentHash = hashValues[current];
                if(currentHash == 0) {
                    buckets[current].first = std::move(k);
                    buckets[current].second = std::move(v);
                    hashValues[current] = hash;
                    return;
                }
                unsigned existingDistance = ComputeProbeDistance(current, currentHash % numBuckets);
                if (existingDistance < distance) {
                    Bucket& bucket = buckets[current];
                    std::swap(hash, hashValues[current]);
                    std::swap(k, bucket.first);
                    std::swap(v, bucket.second);
                    distance = existingDistance;
                }
                current = (current + 1) % numBuckets;
                ++distance;
            }
        }

        [[nodiscard]] float GetLoadFactor(unsigned numEntries) const {
            return static_cast<float>(numEntries) / numBuckets;
        }

        void Rehash() {
            Bucket* oldBuckets = buckets;
            unsigned* oldHashes = hashValues;
            auto oldNumEntries = numEntries;
            auto oldNumBuckets = numBuckets;
            numBuckets *= 2;
            buckets = new Bucket[numBuckets];
            hashValues = new unsigned[numBuckets];
            std::fill(hashValues, hashValues + numBuckets, 0);
            for (int i = 0; i < oldNumBuckets; ++i) {
                if (oldHashes[i] != 0) {
                    Bucket& bucket = oldBuckets[i];
                    (*this)[std::move(bucket.first)] = std::move(bucket.second);
                }
            }
            numEntries = oldNumEntries;
            delete[] oldBuckets;
            delete[] oldHashes;
        }
    };

}
