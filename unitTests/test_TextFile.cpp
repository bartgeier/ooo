#include "gtest/gtest.h"
#include <filesystem>
#include "TextFile.h"


TEST(TextFile, load) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt;
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
}

TEST(TextFile, load_error) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt;
        bool error = true;
        error = txt.load(path.c_str(), error);
        EXPECT_TRUE(error);
        EXPECT_EQ(txt.size(), 0);
        EXPECT_EQ(txt.numOfLines(), 0);
}

TEST(TextFile, append_character) {
        TextFile txt;
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
