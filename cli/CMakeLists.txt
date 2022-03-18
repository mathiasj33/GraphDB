SET(CLI_SOURCES
        src/cli/main.cpp)

add_executable(cli ${CLI_SOURCES})
target_link_libraries(cli PRIVATE graphDB)