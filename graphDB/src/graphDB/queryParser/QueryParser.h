#pragma once

#include "Query.h"
#include "QueryLexer.h"
#include "graphDB/index/Dictionary.h"

using namespace graph_db::index;

namespace graph_db::queryParser {

    class QueryParser {
    public:
        explicit QueryParser(Dictionary& dictionary) : dictionary(dictionary) {}
        Query ParseQuery(std::string_view query);

    private:
        Dictionary& dictionary;
        std::unique_ptr<QueryLexer> lexer;

        Query ParseQuery();
        TriplePattern ParseTriplePattern();
        std::variant<unsigned, std::string> ParseVariableOrResource();
        void ExpectToken(TokenType type);
        bool PeekToken(TokenType type);
    };

}
