#include <gtest/gtest.h>
#include "graphDB/queryParser/QueryParser.h"

using namespace graph_db::queryParser;

TEST(GraphDBTest, testQueryParser) {
    std::string query = "SELECT ?X ?Y WHERE { ?X <hasAge> ?Y . ?Z <hasAge> \"42\" . ?myVariable ?Z <Object>.}";
    Dictionary dictionary;
    QueryParser parser{dictionary};

    auto result = parser.ParseQuery(query);
    std::vector<std::string> expectedVariables = {"X", "Y"};
    std::vector<TriplePattern> expectedPatterns = {
            TriplePattern{"X", 1, "Y"},
            TriplePattern{"Z", 1, 2},
            TriplePattern{"myVariable", "Z", 3},
    };
    ASSERT_TRUE(result.print);
    ASSERT_EQ(expectedVariables, result.projectionVariables);
    ASSERT_EQ(expectedPatterns, result.triplePatterns);

    ASSERT_EQ("<hasAge>", dictionary.GetResource(dictionary.GetId("<hasAge>")));
    ASSERT_EQ("\"42\"", dictionary.GetResource(dictionary.GetId("\"42\"")));

    result = parser.ParseQuery("COUNT ?X ?Y WHERE { ?X <hasAge> ?Y . ?Z <hasAge> \"42\" . ?myVariable ?Z <Object>.}");
    ASSERT_FALSE(result.print);

    try {
        parser.ParseQuery("SELECT ?X WHERE ?X <hasAge> ?Y .");  // missing braces
        FAIL();
    } catch (std::runtime_error& e) {
    }
    try {
        parser.ParseQuery("SELECT ?X WHERE {?X <hasAge> ?Y .} ?X");
        FAIL();
    } catch (std::runtime_error& e) {
    }
    try {
        parser.ParseQuery("SELECT COUNT ?X WHERE { ?X <hasAge> ?Y .}");
        FAIL();
    } catch (std::runtime_error& error) {
    }
}