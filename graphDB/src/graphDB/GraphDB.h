#pragma once

#include "graphDB/index/Dictionary.h"
#include "graphDB/index/IndexedTripleTable.h"
#include "graphDB/fileParser/FileParser.h"
#include "graphDB/evaluation/QueryAnswerer.h"
#include "graphDB/queryParser/QueryParser.h"

using namespace graph_db::index;
using namespace graph_db::evaluation;
using namespace graph_db::queryParser;
using namespace graph_db::fileParser;

namespace graph_db {

    class GraphDB {
    public:
        GraphDB() : fileParser(dictionary), queryAnswerer(table, dictionary), queryParser(dictionary) {}

        unsigned LoadFile(const std::string& filename);
        unsigned ComputeQueryAnswers(std::string_view query, bool countOnly = false);

    private:
        Dictionary dictionary;
        IndexedTripleTable table;
        FileParser fileParser;
        QueryAnswerer queryAnswerer;
        QueryParser queryParser;
    };

}
