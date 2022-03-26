#pragma once

#include <gtest/gtest.h>
#include "graphDB/GraphDB.h"
#include <filesystem>
#include <iostream>
#include <fstream>

using namespace graph_db;

class GraphDBTestFixture : public ::testing::Test {
public:
    GraphDB db;

    void ASSERT_QUERY(std::string_view query, unsigned expectedCount, const std::string& expectedOutput) {
        std::stringstream stream;
        auto old_buf = std::cout.rdbuf(stream.rdbuf());  // redirect std::cout to stream
        unsigned count = db.ComputeQueryAnswers(query);
        std::cout.rdbuf(old_buf); // reset std::cout

        ASSERT_EQ(expectedCount, count);
        ASSERT_EQ("----------\n" + expectedOutput + "----------\n", stream.str());
    }

    void ASSERT_QUERY_COUNT(std::string_view query, unsigned expectedCount) {
        ASSERT_EQ(expectedCount, db.ComputeQueryAnswers(query, true));
    }

    void SetUp() override {
        db.LoadFile("data/familyGuy.ttl");
    }

    void LoadLubmData() {
        db.LoadFile("data/LUBM-001-mat.ttl");
    };

    static std::unordered_map<std::string, std::string> GetLubmQueries() {
        std::unordered_map<std::string, std::string> queries;
        for (const auto& entry : std::filesystem::directory_iterator("data/queries")) {
            queries[entry.path().filename()] = ReadFile(entry.path());
        }
        return queries;
    }

private:
    static std::string ReadFile(const std::string& path) {
        std::ifstream input(path);
        std::stringstream string;
        while (input >> string.rdbuf());
        return string.str();
    }
};