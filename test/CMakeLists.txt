SET(TEST_SOURCES
        graphDB/test_GraphDB.cpp

        Tester.cpp)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(tester PUBLIC
        graphDB
        GTest::GTest)