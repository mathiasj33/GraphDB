SET(GRAPHDB_SOURCES
        src/graphDB/dictionary/Dictionary.cpp
        src/graphDB/dictionary/Dictionary.h)

add_library(graphDB STATIC ${GRAPHDB_SOURCES})
target_include_directories(graphDB PUBLIC src/)