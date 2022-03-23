#include "QueryLexer.h"
#include <cassert>
#include <algorithm>

namespace graph_db::queryParser {
    Token QueryLexer::PeekToken() {
        if (!nextToken) {
            nextToken = std::make_unique<Token>(ConsumeToken());
        }
        return *nextToken;
    }

    Token QueryLexer::ConsumeToken() {
        if(nextToken) {
            Token t = *nextToken;
            nextToken = nullptr;
            return t;
        }
        SkipWhitespace();
        if(IsAtEnd()) {
            return Token{TokenType::EOQ};
        }
        char c = Peek();
        switch (c) {
            case '{':
                ++pos;
                return Token{TokenType::LBRACE};
            case '}':
                ++pos;
                return Token{TokenType::RBRACE};
            case '.':
                ++pos;
                return Token{TokenType::DOT};
            case '?':
                return LexVariable();
            case '<':
            case '\"':
                return LexResource();
            default:
                return LexKeyword();
        }
    }

    Token QueryLexer::LexVariable() {
        ++pos;
        unsigned start = pos;
        while (!IsAtEnd() && IsValidVariableChar(Peek())) {
            ++pos;
        }
        return Token{TokenType::VARIABLE, query.substr(start, pos - start)};
    }

    Token QueryLexer::LexResource() {
        unsigned start = pos;
        char endChar = Peek() == '<' ? '>' : '"';
        ++pos;
        while(!IsAtEnd() && Peek() != endChar) {  // assume there's no nesting in resources
            ++pos;
        }
        ++pos;
        return Token{TokenType::RESOURCE, query.substr(start, pos - start)};
    }

    Token QueryLexer::LexKeyword() {
        unsigned start = pos;
        while(!IsAtEnd() && IsValidKeywordChar(Peek())) {
            ++pos;
        }
        std::string s = query.substr(start, pos - start);
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        auto search = keywords.find(s);
        if(search != keywords.end()) {
            return Token{search->second};
        }
        return Token{TokenType::ERROR};
    }

    char QueryLexer::Peek() {
        assert(!IsAtEnd());
        return query[pos];
    }

    bool QueryLexer::IsAtEnd() {
        return pos >= query.size();
    }

    bool QueryLexer::IsAtWhitespace() {
        return !IsAtEnd() && (Peek() == ' ' || Peek() == '\t' || Peek() == '\n');
    }

    void QueryLexer::SkipWhitespace() {
        while (IsAtWhitespace()) {
            pos++;
        }
    }

    bool QueryLexer::IsValidVariableChar(char c) {
        return IsValidKeywordChar(c) || ('0' <= c && c <= '9');
    }

    bool QueryLexer::IsValidKeywordChar(char c) {
        return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
    }
}