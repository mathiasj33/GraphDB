#include "FileParser.h"

namespace graph_db::fileParser {
    bool FileParser::ParseLine(const char* line, unsigned n, unsigned int& s, unsigned int& p, unsigned int& o) {
        current = 0;
        this->line = line;
        length = n;
        SkipWhitespace();
        bool result = ParseResource(s) && ParseResource(p) && ParseResource(o);
        if(!result || line[current] != '.') {
            return false;
        }
        return true;
    }

    bool FileParser::ParseResource(unsigned int& result) {
        std::string string = GetResourceString();
        if(string.empty()) {
            return false;
        }
        result = dictionary.GetId(std::move(string));
        SkipWhitespace();
        return true;
    }

    std::string FileParser::GetResourceString() {
        unsigned begin = current;
        if(begin >= length || (line[begin] != '<' && line[begin] != '"')) {
            return "";
        }
        char end = line[begin] == '<' ? '>' : '"';  // TODO: assumption: no nesting
        ++current;
        while (current < length - 1) {
            ++current;
            if(line[current] == end) {
                return std::string{line + begin, (++current) - begin};
            }
            if(line[current] == '\n' || line[current] == '\0') {
                return "";
            }
        }
        return "";
    }

    void FileParser::SkipWhitespace() {
        while(current < length && (line[current] == ' ' || line[current] == '\t')) {
            ++current;
        }
    }
}