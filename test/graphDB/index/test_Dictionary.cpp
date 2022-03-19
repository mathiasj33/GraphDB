#include <gtest/gtest.h>
#include "graphDB/index/Dictionary.h"

using namespace graph_db::index;

TEST(GraphDBTest, testDictionary) {
    Dictionary dict;
    unsigned id1 = dict.GetId("<a>");
    unsigned id2 = dict.GetId("<b>");
    unsigned id3 = dict.GetId("\"a\"");
    ASSERT_EQ("<a>", dict.GetResource(id1));
    ASSERT_NE("<a>", dict.GetResource(id2));
    ASSERT_EQ("<b>", dict.GetResource(id2));
    ASSERT_EQ("\"a\"", dict.GetResource(id3));
    ASSERT_NE(dict.GetResource(id1), dict.GetResource(id3));
}
