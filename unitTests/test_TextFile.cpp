#include "gtest/gtest.h"
#include <filesystem>
#include "TextFile.h"

TEST(TextFile, construct) {
        TextFile txt(10000);
        EXPECT_EQ(txt.numOfLines(), 1);
        /* one empty Line */
        EXPECT_EQ(txt.line(0).start, 0);
        EXPECT_EQ(txt.line(0).end, 0);
}

TEST(TextFile, load) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt(10000);
        bool error = false;
        error = txt.load(path.c_str(), error);
        ASSERT_FALSE(error);
        EXPECT_EQ(txt.size(), 87);
        EXPECT_EQ(txt.numOfLines(), 7);
        EXPECT_EQ(txt.line(0).start, 0);
        EXPECT_EQ(txt.line(0).end, 18);
        EXPECT_EQ(txt.line(1).start, 19);
        EXPECT_EQ(txt.line(1).end, 19);
        EXPECT_EQ(txt.line(txt.numOfLines() - 1).start, 85);
        EXPECT_EQ(txt.line(txt.numOfLines() - 1).end, 86);
        EXPECT_EQ(txt.line(2).at[1], 'h');
}

TEST(TextFile, load_error) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt(10000);
        bool error = true;
        error = txt.load(path.c_str(), error);
        EXPECT_TRUE(error);
        EXPECT_EQ(txt.size(), 0);
        EXPECT_EQ(txt.numOfLines(), 1);
}

TEST(TextFile, append_character) {
        TextFile txt(10000);
        EXPECT_EQ(txt.numOfLines(), 1);
        txt.append('o');
        EXPECT_EQ(txt.size(), 1);
        txt.append('o');
        EXPECT_EQ(txt.numOfLines(), 1);
        txt.append('o');
        txt.append('o');
        txt.append('o');
        txt.append('o');
        txt.append('\n');
        EXPECT_EQ(txt.size(), 7);
        EXPECT_EQ(txt.numOfLines(), 2);
}

TEST(TextFile, example_copy) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt(10000);
        bool error = false;
        error = txt.load(path.c_str(), error);
        TextFile copy(10000);
        for (size_t idx = 0; idx < txt.size(); idx++) {
                copy.append(txt[idx]);
        }
        for (size_t idx = 0; idx < txt.size(); idx++) {
                EXPECT_EQ(copy[idx], txt[idx]);
                EXPECT_EQ(copy.size(), txt.size());
        }
        EXPECT_EQ(txt.numOfLines(), 7);
        for (size_t idx = 0; idx < txt.numOfLines()-1; idx++) {
                EXPECT_EQ(copy.line(idx).start,txt.line(idx).start);
                EXPECT_EQ(copy.line(idx).end,txt.line(idx).end);
                EXPECT_EQ(copy.line(idx).size,txt.line(idx).size);
                if (copy.line(idx).size > 0) {
                        EXPECT_EQ(copy.line(idx).at[0], txt.line(idx).at[0]);
                }
        }
        EXPECT_EQ(txt.line(2).at[1], 'h');
        EXPECT_EQ(copy.line(2).at[1], 'h');
}
