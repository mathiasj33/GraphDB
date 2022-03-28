SET(GRAPHDB_SOURCES
        src/graphDB/index/Dictionary.cpp
        src/graphDB/index/Dictionary.h
        src/graphDB/index/IndexedTripleTable.cpp
        src/graphDB/index/IndexedTripleTable.h
        src/graphDB/evaluation/Scan.h
        src/graphDB/index/ThreeHashKey.h
        src/graphDB/index/TwoHashKey.h
        src/graphDB/index/VectorIndex.h
        src/graphDB/index/Triple.h
        src/graphDB/evaluation/Scan.cpp
        src/graphDB/evaluation/FullScan.h
        src/graphDB/evaluation/FullScan.cpp
        src/graphDB/evaluation/ScanTwoVariables.cpp
        src/graphDB/evaluation/ScanTwoVariables.h
        src/graphDB/evaluation/ScanSorO.h
        src/graphDB/evaluation/ScanSorO.cpp
        src/graphDB/evaluation/ScanP.cpp
        src/graphDB/evaluation/ScanP.h
        src/graphDB/queryParser/Query.h
        src/graphDB/queryParser/TriplePattern.h
        src/graphDB/queryParser/QueryParser.cpp
        src/graphDB/queryParser/QueryParser.h
        src/graphDB/evaluation/QueryAnswerer.cpp
        src/graphDB/evaluation/QueryAnswerer.h
        src/graphDB/evaluation/ScanSingleValue.cpp
        src/graphDB/evaluation/ScanSingleValue.h
        src/graphDB/queryParser/TriplePattern.cpp
        src/graphDB/queryParser/QueryLexer.cpp
        src/graphDB/queryParser/QueryLexer.h
        src/graphDB/GraphDB.cpp
        src/graphDB/GraphDB.h
        src/graphDB/fileParser/FileParser.cpp
        src/graphDB/fileParser/FileParser.h
        src/graphDB/queryOptimiser/QueryOptimiser.cpp
        src/graphDB/queryOptimiser/QueryOptimiser.h src/graphDB/index/LinearProbingIndex.h)

add_library(graphDB STATIC ${GRAPHDB_SOURCES})
target_include_directories(graphDB PUBLIC src/)