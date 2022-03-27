#include <gtest/gtest.h>
#include "graphDB/queryParser/QueryLexer.h"

using namespace graph_db::query_parser;

std::vector<Token> GetAllTokens(QueryLexer& lexer) {
    std::vector<Token> tokens;
    Token token = lexer.ConsumeToken();
    while (token.type != TokenType::EOQ && token.type != TokenType::ERROR) {
        tokens.push_back(token);
        token = lexer.ConsumeToken();
    }
    return tokens;
}

template<class T>
void ASSERT_VECTOR_EQ(std::vector<T>& v1, std::vector<T>& v2) {
    ASSERT_EQ(v1.size(), v2.size());
    for (unsigned long i = 0; i < v1.size(); ++i) {
        ASSERT_EQ(v1[i], v2[i]);
    }
}

TEST(GraphDBTest, testQueryLexer) {
    std::string query = "SELECT ?X ?Y WHERE { ?X <hasAge> ?Y . ?Z . <hasAge> \"42\" . ?myVariable ?Z <Object>.}";
    QueryLexer lexer{query};
    auto tokens = GetAllTokens(lexer);
    std::vector<Token> expected = {
            Token{TokenType::SELECT},
            Token{TokenType::VARIABLE, "X"},
            Token{TokenType::VARIABLE, "Y"},
            Token{TokenType::WHERE},
            Token{TokenType::LBRACE},
            Token{TokenType::VARIABLE, "X"},
            Token{TokenType::RESOURCE, "<hasAge>"},
            Token{TokenType::VARIABLE, "Y"},
            Token{TokenType::DOT},
            Token{TokenType::VARIABLE, "Z"},
            Token{TokenType::DOT},
            Token{TokenType::RESOURCE, "<hasAge>"},
            Token{TokenType::RESOURCE, "\"42\""},
            Token{TokenType::DOT},
            Token{TokenType::VARIABLE, "myVariable"},
            Token{TokenType::VARIABLE, "Z"},
            Token{TokenType::RESOURCE, "<Object>"},
            Token{TokenType::DOT},
            Token{TokenType::RBRACE},
    };

    ASSERT_VECTOR_EQ(expected, tokens);
}