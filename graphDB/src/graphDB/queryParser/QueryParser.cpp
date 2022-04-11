#include "QueryParser.h"
#include <algorithm>

namespace graph_db::query_parser {
    Query QueryParser::ParseQuery(std::string_view queryString) {
        lexer = std::make_unique<QueryLexer>(std::string(queryString));
        return ParseQuery();
    }

    Query QueryParser::ParseQuery() {
        Query query;
        query.print = PeekToken(TokenType::SELECT);
        ExpectOneOf({TokenType::SELECT, TokenType::COUNT});
        while (PeekToken(TokenType::VARIABLE)) {
            query.projectionVariables.push_back(lexer->ConsumeToken().value);
        }
        ExpectToken(TokenType::WHERE);
        ExpectToken(TokenType::LBRACE);
        while(PeekToken(TokenType::VARIABLE) || PeekToken(TokenType::RESOURCE)) {
            query.triplePatterns.emplace_back(ParseTriplePattern());
        }
        ExpectToken(TokenType::RBRACE);
        ExpectToken(TokenType::EOQ);
        return query;
    }

    TriplePattern QueryParser::ParseTriplePattern() {
        TriplePattern pattern;
        pattern.s = ParseVariableOrResource();
        pattern.p = ParseVariableOrResource();
        pattern.o = ParseVariableOrResource();
        if (PeekToken(TokenType::RBRACE)) {
            return pattern;  // make the last dot optional
        }
        ExpectToken(TokenType::DOT);
        return pattern;
    }

    std::variant<unsigned, std::string> QueryParser::ParseVariableOrResource() {
        if(PeekToken(TokenType::VARIABLE)) {
            return lexer->ConsumeToken().value;
        }
        if (PeekToken(TokenType::RESOURCE)) {
            return dictionary.GetId(lexer->ConsumeToken().value);
        }
        throw std::runtime_error("Error while parsing query.");
    }

    void QueryParser::ExpectToken(TokenType type) {
        if (lexer->ConsumeToken().type != type) {
            throw std::runtime_error("Error while parsing query.");
        }
    }

    void QueryParser::ExpectOneOf(const std::vector<TokenType>& types) {
        if (std::find(types.begin(), types.end(), lexer->ConsumeToken().type) == types.end()) {
            throw std::runtime_error("Error while parsing query.");
        }
    }

    bool QueryParser::PeekToken(TokenType type) {
        return lexer->PeekToken().type == type;
    }

}