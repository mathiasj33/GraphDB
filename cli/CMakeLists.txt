SET(CLI_SOURCES
        src/cli/main.cpp src/cli/Cli.cpp src/cli/Cli.h)

add_executable(cli ${CLI_SOURCES})
target_link_libraries(cli PRIVATE graphDB)