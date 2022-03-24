#pragma once

#include "graphDB/index/Dictionary.h"

using namespace graph_db::index;

namespace graph_db::fileParser {

    class FileParser {
    public:
        explicit FileParser(Dictionary& dictionary) : dictionary(dictionary) {}

        /**
         * Parses the given line and saves the matching s, p, and o indices into the respective output parameters.
         * Returns false if a parsing error occurred.
         */
         bool ParseLine(const char* line, unsigned n, unsigned& s, unsigned& p, unsigned& o);

    private:
        Dictionary& dictionary;
        const char* line = nullptr;
        unsigned current = 0;
        unsigned length = 0;

        /**
         * Parses one resource from the current position in the line and saves the matching id into {@param result}.
         * Returns false if no resource can be parsed.
         */
        bool ParseResource(unsigned& result);
        std::string GetResourceString();
        void SkipWhitespace();
    };

}
