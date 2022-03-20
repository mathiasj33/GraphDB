#pragma once

#include <gtest/gtest.h>
#include "graphDB/index/IndexedTripleTable.h"
#include "graphDB/evaluation/Scan.h"

using namespace graph_db::index;
using namespace graph_db::evaluation;

class ScanTestFixture: public ::testing::Test {
public:
    IndexedTripleTable table;

    void SetUp() override {
        table.Add(1, 2, 3);
        table.Add(1, 4, 5);
        table.Add(1, 4, 6);
        table.Add(1, 7, 8);
        table.Add(1, 9, 10);
        table.Add(5, 11, 5);
        table.Add(5, 11, 10);
        table.Add(10, 11, 3);
        table.Add(1, 12, 13);
        table.Add(3, 12, 13);
        table.Add(10, 12, 14);
        table.Add(20, 20, 20);
        table.Add(5, 13, 10);
    }

    void ASSERT_SCAN_EQ(Scan& scan, unsigned* triples, int size) {
        for(int i = 0; i < size; i++) {
            ASSERT_FALSE(scan.ReachedEnd());
            unsigned s,p,o;
            scan.GetNext(s, p, o);
            ASSERT_EQ(s, triples[i * 3]);
            ASSERT_EQ(p, triples[i * 3 + 1]);
            ASSERT_EQ(o, triples[i * 3 + 2]);
        }
        ASSERT_TRUE(scan.ReachedEnd());
    }
};