#include <gtest/gtest.h>
#include "ScanTestFixture.h"
#include "graphDB/index/IndexedTripleTable.h"

using namespace graph_db::evaluation;

TEST_F(ScanTestFixture, testEvaluateS) {
    std::unique_ptr<Scan> scan = table.EvaluateS(12, 13);
    unsigned result[] = {1, 12, 13,
                         3, 12, 13};
    ASSERT_SCAN_EQ(*scan, result, 2);

    scan = table.EvaluateS(2, 3);
    unsigned result2[] = {1, 2, 3};
    ASSERT_SCAN_EQ(*scan, result2, 1);

    scan = table.EvaluateS(123, 1);
    ASSERT_SCAN_EQ(*scan, result2, 0);
}

TEST_F(ScanTestFixture, testEvaluateP) {
    std::unique_ptr<Scan> scan = table.EvaluateP(5, 10);
    unsigned result[] = {5, 13, 10,
                         5, 11, 10};
    ASSERT_SCAN_EQ(*scan, result, 2);

    scan = table.EvaluateP(123, 1);
    ASSERT_SCAN_EQ(*scan, result, 0);
}

TEST_F(ScanTestFixture, testEvaluateO) {
    std::unique_ptr<Scan> scan = table.EvaluateO(1, 4);
    unsigned result[] = {1, 4, 5,
                         1, 4, 6};
    ASSERT_SCAN_EQ(*scan, result, 2);

    scan = table.EvaluateO(123, 1);
    ASSERT_SCAN_EQ(*scan, result, 0);
}

TEST_F(ScanTestFixture, testEvaluateSP) {
    std::unique_ptr<Scan> scan = table.EvaluateSP(13, false);
    unsigned result[] = {1, 12, 13,
                         3, 12, 13};
    ASSERT_SCAN_EQ(*scan, result, 2);

    scan = table.EvaluateSP(123, false);
    ASSERT_SCAN_EQ(*scan, result, 0);
}

TEST_F(ScanTestFixture, testEvaluateSO) {
    std::unique_ptr<Scan> scan = table.EvaluateSO(11, false);
    unsigned result[] = {10, 11, 3,
                         5, 11, 10,
                         5, 11, 5};
    ASSERT_SCAN_EQ(*scan, result, 3);

    scan = table.EvaluateSO(11, true);
    unsigned result1[] = {5, 11, 5};
    ASSERT_SCAN_EQ(*scan, result1, 1);

    scan = table.EvaluateSO(123, false);
    ASSERT_SCAN_EQ(*scan, result, 0);
}

TEST_F(ScanTestFixture, testEvaluatePO) {
    std::unique_ptr<Scan> scan = table.EvaluatePO(5, false);
    unsigned result[] = {5, 13, 10,
                         5, 11, 5,
                         5, 11, 10};
    ASSERT_SCAN_EQ(*scan, result, 3);

    scan = table.EvaluatePO(123, false);
    ASSERT_SCAN_EQ(*scan, result, 0);
}

TEST_F(ScanTestFixture, testEvaluateSPO) {
    std::unique_ptr<Scan> scan = table.EvaluateSPO(false, false, false);
    unsigned result[] = {1, 2, 3,
                         1, 4, 5,
                         1, 4, 6,
                         1, 7, 8,
                         1, 9, 10,
                         5, 11, 5,
                         5, 11, 10,
                         10, 11, 3,
                         1, 12, 13,
                         3, 12, 13,
                         10, 12, 14,
                         20, 20, 20,
                         5, 13, 10};
    ASSERT_SCAN_EQ(*scan, result, 13);

    scan = table.EvaluateSPO(false, false, true);
    unsigned result2[] = {5, 11, 5,
                          20, 20, 20};
    ASSERT_SCAN_EQ(*scan, result2, 2);
}
