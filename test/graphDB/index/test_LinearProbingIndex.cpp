#include <gtest/gtest.h>
#include "graphDB/index/Dictionary.h"
#include "graphDB/index/LinearProbingIndex.h"
#include "graphDB/index/TwoHashKey.h"

using namespace graph_db::index;

TEST(GraphDBTest, testLinearProbingIndex) {
    LinearProbingIndex<TwoHashKey, unsigned, TwoHashKey::HashFunction> index{10, .5};
    index[TwoHashKey{1,1}] = 11;
    ASSERT_EQ(1, index.size());
    ASSERT_EQ(11, index[(TwoHashKey{1,1})]);
    ASSERT_EQ(11, index.find((TwoHashKey{1, 1}))->second);
    index[TwoHashKey{1, 2}] = 12;
    index[TwoHashKey{1, 3}] = 13;
    index[TwoHashKey{1, 4}] = 14;
    index[TwoHashKey{1,5}] = 15;
    ASSERT_FLOAT_EQ(.5f, index.loadFactor());
    ASSERT_EQ(12, index[(TwoHashKey{1,2})]);
    ASSERT_EQ(13, index[(TwoHashKey{1,3})]);
    ASSERT_EQ(14, index[(TwoHashKey{1,4})]);
    ASSERT_EQ(15, index[(TwoHashKey{1,5})]);
    index[TwoHashKey{1,1}] = 111;
    index[TwoHashKey{1,3}] = 133;
    ASSERT_EQ(111, index[(TwoHashKey{1,1})]);
    ASSERT_EQ(133, index[(TwoHashKey{1,3})]);
    ASSERT_TRUE(index.find((TwoHashKey{1, 123})) == index.end());

    index[TwoHashKey{1,6}] = 16;
    ASSERT_FLOAT_EQ(.3f, index.loadFactor());
    ASSERT_EQ(16, index[(TwoHashKey{1, 6})]);
    ASSERT_EQ(6, index.size());
}

TEST(GraphDBTest, testLinearProbingIndexMany) {
    LinearProbingIndex<TwoHashKey, unsigned, TwoHashKey::HashFunction> index;
    unsigned numTests = 10000;
    for (unsigned i = 0; i < numTests; ++i) {
        index[TwoHashKey{i, i + 1}] = i * 2;
    }
    for (unsigned i = 0; i < numTests; ++i) {
        auto search = index.find({TwoHashKey{i, i + 1}});
        ASSERT_TRUE(search != index.end());
        ASSERT_EQ(i * 2, search->second);
    }
}