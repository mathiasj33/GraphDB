SET(TEST_SOURCES
        graphDB/test_GraphDB.cpp
        graphDB/dictionary/test_Dictionary.cpp

        Tester.cpp)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(tester PUBLIC
        graphDB
        GTest::GTest)