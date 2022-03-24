#include <gtest/gtest.h>
#include "graphDB/GraphDB.h"

using namespace graph_db;

TEST(GraphDBTest, integrationTest) {  // TODO: add more tests (look at model answers)
    std::stringstream stream;
    auto old_buf = std::cout.rdbuf(stream.rdbuf());  // redirect std::cout to stream

    GraphDB db;
    db.LoadFile("data/familyGuy.ttl");
    db.PrintQueryAnswers("SELECT ?X ?Z WHERE { ?X <hasAge> ?Y . ?Z <hasAge> ?Y . }");
    ASSERT_EQ("<Brian>\t<Brian>\n"
              "<Lois>\t<Peter>\n"
              "<Lois>\t<Lois>\n"
              "<Peter>\t<Peter>\n"
              "<Peter>\t<Lois>\n", stream.str());

    std::cout.rdbuf(old_buf); // reset std::cout
}
