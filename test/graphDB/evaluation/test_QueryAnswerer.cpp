#include <gtest/gtest.h>
#include "graphDB/index/IndexedTripleTable.h"
#include "graphDB/queryParser/TriplePattern.h"
#include "graphDB/queryParser/Query.h"
#include "graphDB/evaluation/QueryAnswerer.h"

using namespace graph_db::index;
using namespace graph_db::evaluation;
using namespace graph_db::query_parser;

TEST(GraphDBTest, testQueryAnswerer) {
    std::stringstream stream;
    auto old_buf = std::cout.rdbuf(stream.rdbuf());  // redirect std::cout to stream

    Dictionary dictionary;
    unsigned a = dictionary.GetId("<a>");
    unsigned b = dictionary.GetId("<b>");
    unsigned c = dictionary.GetId("<c>");
    unsigned d = dictionary.GetId("<d>");
    unsigned r = dictionary.GetId("<r>");
    unsigned s = dictionary.GetId("<s>");
    unsigned one = dictionary.GetId("\"1\"");
    unsigned two = dictionary.GetId("\"2\"");

    IndexedTripleTable table;
    table.Add(a, r, b);
    table.Add(b, r, c);
    table.Add(c, r, d);
    table.Add(d, s, one);
    table.Add(a, s, two);
    table.Add(b, s, one);
    table.Add(c, s, c);

    QueryAnswerer answerer{table, dictionary};

    {
        Query query;
        query.print = true;
        query.projectionVariables.emplace_back("X");
        query.projectionVariables.emplace_back("Y");
        query.projectionVariables.emplace_back("Z");
        query.projectionVariables.emplace_back("W");
        query.triplePatterns.emplace_back(TriplePattern{"X", r, "Y"});
        query.triplePatterns.emplace_back(TriplePattern{"Y", r, "Z"});
        query.triplePatterns.emplace_back(TriplePattern{"Z", r, "W"});
        ASSERT_EQ(1, answerer.ComputeQueryAnswers(query));
    }

    {
        Query query;
        query.print = true;
        query.projectionVariables.emplace_back("X");
        query.triplePatterns.emplace_back(TriplePattern{"X", r, "Y"});
        query.triplePatterns.emplace_back(TriplePattern{"X", s, "Z"});
        ASSERT_EQ(3, answerer.ComputeQueryAnswers(query));
    }

    {
        Query query;
        query.print = true;
        query.projectionVariables.emplace_back("Y");
        query.projectionVariables.emplace_back("X");
        query.triplePatterns.emplace_back(TriplePattern{"X", s, "X"});
        query.triplePatterns.emplace_back(TriplePattern{"X", r, "Y"});
        query.triplePatterns.emplace_back(TriplePattern{"Y", s, one});
        ASSERT_EQ(1, answerer.ComputeQueryAnswers(query));
    }

    {
        Query query;
        query.print = true;
        query.projectionVariables.emplace_back("Y");
        query.projectionVariables.emplace_back("X");
        query.triplePatterns.emplace_back(TriplePattern{"X", r, "X"});
        query.triplePatterns.emplace_back(TriplePattern{"X", "Y", "Z"});
        ASSERT_EQ(0, answerer.ComputeQueryAnswers(query));
    }

    std::cout.rdbuf(old_buf); // reset std::cout

    ASSERT_EQ("----------\n"
              "?X\t?Y\t?Z\t?W\n"
              "<a>\t<b>\t<c>\t<d>\n"
              "----------\n"
              "----------\n"
              "?X\n"
              "<c>\n<b>\n<a>\n"
              "----------\n"
              "----------\n"
              "?Y\t?X\n"
              "<d>\t<c>\n"
              "----------\n"
              "----------\n"
              "?Y\t?X\n"
              "----------\n", stream.str());
}