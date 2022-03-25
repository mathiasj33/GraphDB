#include <gtest/gtest.h>
#include "graphDB/GraphDB.h"

using namespace graph_db;

TEST(GraphDBTest, integrationTest) {  // TODO: add more tests (look at model answers)
    std::stringstream stream;
    auto old_buf = std::cout.rdbuf(stream.rdbuf());  // redirect std::cout to stream

    GraphDB db;
    db.LoadFile("data/familyGuy.ttl");
    unsigned count = db.ComputeQueryAnswers("SELECT ?X ?Z WHERE { ?X <hasAge> ?Y . ?Z <hasAge> ?Y . }");
    std::cout.rdbuf(old_buf); // reset std::cout

    ASSERT_EQ(5, count);
    ASSERT_EQ("----------\n"
              "?X\t?Z\n"
              "<Brian>\t<Brian>\n"
              "<Lois>\t<Peter>\n"
              "<Lois>\t<Lois>\n"
              "<Peter>\t<Peter>\n"
              "<Peter>\t<Lois>\n"
              "----------\n", stream.str());
}
