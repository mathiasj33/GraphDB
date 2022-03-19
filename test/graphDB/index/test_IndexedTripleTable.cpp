#include <gtest/gtest.h>
#include "graphDB/index/IndexedTripleTable.h"

using namespace graph_db::index;

TEST(GraphDBTest, testIndexedTripleTable) {
    IndexedTripleTable table;
    table.Add(1,3,0);
    table.Add(1,3,0);
    table.Add(1,3,1);
    table.Add(1,2,0);
    table.Add(2,2,0);
    table.Add(1,2,1);
    table.Add(1,3,1);
    table.Add(7,3,0);
    table.Add(7,2,1);

    ASSERT_EQ((IndexedTripleTable::Triple{1,3,0,2, 0, 6}), table[0]);
    ASSERT_EQ((IndexedTripleTable::Triple{1,3,1,0,1,0}), table[1]);
    ASSERT_EQ((IndexedTripleTable::Triple{1,2,0,5, 0, 4}), table[2]);
    ASSERT_EQ((IndexedTripleTable::Triple{2,2,0,0, 3, 1}), table[3]);
    ASSERT_EQ((IndexedTripleTable::Triple{1,2,1,1, 4, 7}), table[4]);
    ASSERT_EQ((IndexedTripleTable::Triple{7,3,0,0, 2, 0}), table[5]);
    ASSERT_EQ((IndexedTripleTable::Triple{7,2,1,6, 5, 2}), table[6]);


}
