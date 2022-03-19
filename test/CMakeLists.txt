SET(TEST_SOURCES
        graphDB/test_GraphDB.cpp
        graphDB/index/test_Dictionary.cpp
        graphDB/index/test_IndexedTripleTable.cpp

        Tester.cpp)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(tester PUBLIC
        graphDB
        GTest::GTest)