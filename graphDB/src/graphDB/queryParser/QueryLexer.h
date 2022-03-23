#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <ostream>

namespace graph_db::queryParser {

    enum TokenType {
        SELECT,
        WHERE,
        VARIABLE,
        RESOURCE,
        LBRACE,
        RBRACE,
        DOT,
        EOQ,  // end of query
        ERROR
    };

    struct Token {
        explicit Token(TokenType type) : type(type) {}

        Token(TokenType type, std::string value) : type(type), value(std::move(value)) {}

        TokenType type;
        std::string value;

        bool operator==(const Token& rhs) const {
            return type == rhs.type &&
                   value == rhs.value;
        }

        bool operator!=(const Token& rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            os << "Token{" << token.type << ", " << token.value << "}";
            return os;
        }
    };

    class QueryLexer {
    public:
        explicit QueryLexer(std::string query) : query(std::move(query)) {}

        Token PeekToken();
        Token ConsumeToken();

    private:
        std::string query;
        unsigned pos = 0;
        std::unique_ptr<Token> nextToken = nullptr;
        std::unordered_map<std::string, TokenType> keywords = {{"select", TokenType::SELECT},
                                                               {"where",  TokenType::WHERE}};

        Token LexVariable();
        Token LexResource();
        Token LexKeyword();

        char Peek();
        bool IsAtEnd();
        bool IsAtWhitespace();
        void SkipWhitespace();
        static bool IsValidVariableChar(char c);
        static bool IsValidKeywordChar(char c);
    };

}
