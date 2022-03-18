SET(GRAPHDB_SOURCES
        src/graphDB/dictionary/dictionary.cpp
        src/graphDB/dictionary/dictionary.h)

add_library(graphDB STATIC ${GRAPHDB_SOURCES})
target_include_directories(graphDB PUBLIC src/)