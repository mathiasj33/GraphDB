SET(GRAPHDB_SOURCES
        src/graphDB/index/Dictionary.cpp
        src/graphDB/index/Dictionary.h
        src/graphDB/index/IndexedTripleTable.cpp
        src/graphDB/index/IndexedTripleTable.h
        src/graphDB/evaluation/Scan.cpp
        src/graphDB/evaluation/Scan.h
        src/graphDB/index/ThreeHashKey.h
        src/graphDB/index/TwoHashKey.h
        src/graphDB/index/VectorIndex.h)

add_library(graphDB STATIC ${GRAPHDB_SOURCES})
target_include_directories(graphDB PUBLIC src/)