#include <gtest/gtest.h>
#include "graphDB/GraphDB.h"
#include "GraphDBTestFixture.h"

using namespace graph_db;

TEST_F(GraphDBTestFixture, integrationTest) {
    auto expected = "?X\t?Z\n"
                    "<Brian>\t<Brian>\n"
                    "<Lois>\t<Peter>\n"
                    "<Lois>\t<Lois>\n"
                    "<Peter>\t<Peter>\n"
                    "<Peter>\t<Lois>\n";
    ASSERT_QUERY("SELECT ?X ?Z WHERE { ?X <hasAge> ?Y . ?Z <hasAge> ?Y . }", 5, expected);
}

TEST_F(GraphDBTestFixture, modelAnswerTests) {
    auto expected = "?X\t?Y\n"
                    "<Peter>\t<Chris>\n"
                    "<Peter>\t<Stewie>\n";
    ASSERT_QUERY("SELECT ?X ?Y WHERE { ?X <hasSon> ?Y . }", 2, expected);

    expected = "?X\n"
               "<Peter>\n"
               "<Peter>\n";
    ASSERT_QUERY("SELECT ?X WHERE { ?X <hasSon> ?Y . }", 2, expected);

    expected = "?Y\n"
               "<Stewie>\n"
               "<Chris>\n";
    ASSERT_QUERY("SELECT ?Y WHERE { <Peter> <hasSon> ?Y . }", 2, expected);

    expected = "?X\n"
               "<Brian>\n";
    ASSERT_QUERY("SELECT ?X WHERE { ?X <hasAge> \"10\" . }", 1, expected);

    expected = "?X\n";
    ASSERT_QUERY("SELECT ?X WHERE { ?X <hasAge> \"12\" . }", 0, expected);

    expected = "?X\t?Y\t?Z\n"
               "<Stewie>\t<Brian>\t<Lois>\n"
               "<Stewie>\t<Stewie>\t<Stewie>\n"
               "<Stewie>\t<Stewie>\t<Brian>\n";
    ASSERT_QUERY("SELECT ?X ?Y ?Z WHERE { ?X <loves> ?Y . ?Y <loves> ?Z . }", 3, expected);

    expected = "?X\t?Y\n"
               "<Stewie>\t<Stewie>\n"
               "<Stewie>\t<Brian>\n";
    ASSERT_QUERY("SELECT ?X ?Y WHERE { ?X <loves> ?X . ?X <loves> ?Y . }", 2, expected);

    expected = "?X\t?Y\t?Z\n"
               "<Brian>\t<loves>\t<Lois>\n"
               "<Peter>\t<marriedTo>\t<Lois>\n";
    ASSERT_QUERY("SELECT ?X ?Y ?Z WHERE { ?X ?Y ?Z . ?Z <hasAge> \"42\" . }", 2, expected);

    expected = "?X\t?Y\t?Z\n"
               "<Peter>\t<Brian>\t<Lois>\n";
    ASSERT_QUERY("SELECT ?X ?Y ?Z WHERE {\n"
                        "?X <hasPet> ?Y . ?Y <loves> ?Z . ?X <marriedTo> ?Z .\n"
                        "}", 1, expected);

    expected = "?X\t?Y\t?Z\n";
    ASSERT_QUERY("SELECT ?X ?Y ?Z WHERE {\n"
                 "?X <hasPet> ?Y . ?Y <hasDaughter> ?Z . ?X <marriedTo> ?Z .\n"
                 "}", 0, expected);
}

/**
 * This test checks the answers produced for the LUBM-001-mat.ttl file are correct.
 * Correct answers obtained from: http://swat.cse.lehigh.edu/projects/lubm/answers.htm
 */
TEST_F(GraphDBTestFixture, LUBMTests) {
    LoadLubmData();
    auto queries = GetLubmQueries();

    auto expected = "?X\n"
                    "<http://www.Department0.University0.edu/GraduateStudent142>\n"
                    "<http://www.Department0.University0.edu/GraduateStudent124>\n"
                    "<http://www.Department0.University0.edu/GraduateStudent101>\n"
                    "<http://www.Department0.University0.edu/GraduateStudent44>\n";
    ASSERT_QUERY(queries["q01.txt"], 4, expected);

    ASSERT_QUERY(queries["q02.txt"], 0, "?X\t?Y\t?Z\n");

    expected = "?X\n"
               "<http://www.Department0.University0.edu/AssistantProfessor0/Publication5>\n"
               "<http://www.Department0.University0.edu/AssistantProfessor0/Publication4>\n"
               "<http://www.Department0.University0.edu/AssistantProfessor0/Publication3>\n"
               "<http://www.Department0.University0.edu/AssistantProfessor0/Publication2>\n"
               "<http://www.Department0.University0.edu/AssistantProfessor0/Publication1>\n"
               "<http://www.Department0.University0.edu/AssistantProfessor0/Publication0>\n";
    ASSERT_QUERY(queries["q03.txt"], 6, expected);

    ASSERT_QUERY_COUNT(queries["q04.txt"], 34);
    ASSERT_QUERY_COUNT(queries["q05.txt"], 719);
    ASSERT_QUERY_COUNT(queries["q06.txt"], 7790);
    ASSERT_QUERY_COUNT(queries["q07.txt"], 67);
    ASSERT_QUERY_COUNT(queries["q08.txt"], 7790);
    ASSERT_QUERY_COUNT(queries["q09.txt"], 208);

    expected = "?X\n"
                    "<http://www.Department0.University0.edu/GraduateStudent142>\n"
                    "<http://www.Department0.University0.edu/GraduateStudent124>\n"
                    "<http://www.Department0.University0.edu/GraduateStudent101>\n"
                    "<http://www.Department0.University0.edu/GraduateStudent44>\n";
    ASSERT_QUERY(queries["q10.txt"], 4, expected);

    ASSERT_QUERY_COUNT(queries["q11.txt"], 224);
    ASSERT_QUERY_COUNT(queries["q12.txt"], 15);

    expected = "?X\n<http://www.Department0.University0.edu/AssistantProfessor2>\n";
    ASSERT_QUERY(queries["q13.txt"], 1, expected);

    ASSERT_QUERY_COUNT(queries["q14.txt"], 5916);
}