SET(TEST_SOURCES
        graphDB/test_GraphDB.cpp
        graphDB/index/test_Dictionary.cpp
        graphDB/index/test_IndexedTripleTable.cpp

        Tester.cpp
        graphDB/evaluation/test_Scan.cpp
        graphDB/evaluation/ScanTestFixture.h
        graphDB/evaluation/test_QueryAnswerer.cpp
        graphDB/queryParser/test_QueryLexer.cpp
        graphDB/queryParser/test_QueryParser.cpp
        graphDB/fileParser/test_FileParser.cpp
        graphDB/fileParser/FileParserTestFixture.h
        graphDB/queryOptimiser/test_QueryOptimiser.cpp
        graphDB/GraphDBTestFixture.h)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(tester PUBLIC
        graphDB
        GTest::GTest)