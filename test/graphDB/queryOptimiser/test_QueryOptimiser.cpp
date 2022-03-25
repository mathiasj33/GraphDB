#include <gtest/gtest.h>
#include "graphDB/queryOptimiser/QueryOptimiser.h"
#include "graphDB/queryParser/QueryParser.h"

using namespace graph_db::queryOptimiser;
using namespace graph_db::queryParser;

TEST(GraphDBTest, testQueryOptimiser) {
    Dictionary dictionary;
    QueryParser parser{dictionary};
    Query opt = QueryOptimiser::OptimiseQuery(parser.ParseQuery("SELECT ?X WHERE { ?X ?Y ?Z . }"));
    ASSERT_EQ(1, opt.triplePatterns.size());
    ASSERT_EQ((TriplePattern{"X", "Y", "Z"}), opt.triplePatterns[0]);
    opt = QueryOptimiser::OptimiseQuery(parser.ParseQuery("SELECT ?X WHERE { ?X ?Y ?Z . ?X <hasAge> \"42\".}"));
    ASSERT_EQ(2, opt.triplePatterns.size());
    ASSERT_EQ((TriplePattern{"X", 1, 2}), opt.triplePatterns[0]);
    ASSERT_EQ((TriplePattern{"X", "Y", "Z"}), opt.triplePatterns[1]);
    opt = QueryOptimiser::OptimiseQuery(parser.ParseQuery("SELECT ?X ?A WHERE { "
                                                          "?X <loves> ?Y ."
                                                          "?Y <loves> ?Z ."
                                                          "?Y <loves> <Peter> ."
                                                          "?A ?r <Peter> ."
                                                          "<Stevie> ?r <Peter> ."
                                                          "}"));
    ASSERT_EQ(5, opt.triplePatterns.size());
    ASSERT_EQ((TriplePattern{5, "r", 4}), opt.triplePatterns[0]);
    ASSERT_EQ((TriplePattern{"A", "r", 4}), opt.triplePatterns[1]);
    ASSERT_EQ((TriplePattern{"Y", 3, 4}), opt.triplePatterns[2]);
    ASSERT_EQ((TriplePattern{"X", 3, "Y"}), opt.triplePatterns[3]);
    ASSERT_EQ((TriplePattern{"Y", 3, "Z"}), opt.triplePatterns[4]);
}