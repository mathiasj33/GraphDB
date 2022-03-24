#pragma once

#include <gtest/gtest.h>
#include "graphDB/fileParser/FileParser.h"

using namespace graph_db::fileParser;

class FileParserTestFixture: public ::testing::Test {
public:
    std::unique_ptr<Dictionary> dictionary;
    std::unique_ptr<FileParser> parser;
    const static int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    void SetUp() override {
        dictionary = std::make_unique<Dictionary>();
        parser = std::make_unique<FileParser>(*dictionary);
    }

    void ASSERT_PARSE_LINE(const std::string& line, unsigned s, unsigned p, unsigned o) {
        strcpy(buffer, line.c_str());
        unsigned s_, p_, o_;
        parser->ParseLine(buffer, BUFFER_SIZE, s_, p_, o_);
        ASSERT_EQ(s, s_);
        ASSERT_EQ(p, p_);
        ASSERT_EQ(o, o_);
    }

    void ASSERT_ERROR(const std::string& line) {
        strcpy(buffer, line.c_str());
        unsigned s_, p_, o_;
        ASSERT_FALSE(parser->ParseLine(buffer, BUFFER_SIZE, s_, p_, o_));
    }
};