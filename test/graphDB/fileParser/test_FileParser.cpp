#include <gtest/gtest.h>
#include "graphDB/fileParser/FileParser.h"
#include "FileParserTestFixture.h"

using namespace graph_db::file_parser;

TEST_F(FileParserTestFixture, testFileParser) {
    ASSERT_PARSE_LINE("<a> <b> <c> .", 1, 2, 3);
    ASSERT_PARSE_LINE("\"test\" <b>         <d> .", 4, 2, 5);
    ASSERT_PARSE_LINE("     <https:://test.org/resource1>  <d>  \"test\"", 6, 5, 4);
    ASSERT_EQ("<https:://test.org/resource1>", dictionary->GetResource(6));
    ASSERT_EQ("\"test\"", dictionary->GetResource(4));
    ASSERT_EQ("<b>", dictionary->GetResource(2));
    ASSERT_ERROR("<a> <b <c>");
    ASSERT_ERROR("a> <b <c>");
    ASSERT_ERROR("<a> <b> c>");
    ASSERT_ERROR("<a> <b> <c>");
}