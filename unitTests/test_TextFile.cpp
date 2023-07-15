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
        EXPECT_EQ(txt.file.size(), 71);
        EXPECT_EQ(txt.lines.size(), 5);
        EXPECT_EQ(txt.lines[0].start, 0);
        EXPECT_EQ(txt.lines[0].end, 18);
        EXPECT_EQ(txt.lines[1].start, 19);
        EXPECT_EQ(txt.lines[1].end, 19);
        EXPECT_EQ(txt.lines[txt.lines.size() - 1].start, 52);
        EXPECT_EQ(txt.lines[txt.lines.size() - 1].end, 70);
}

TEST(TextFile, error) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt;
        bool error = true;
        error = txt.load(path.c_str(), error);
        EXPECT_TRUE(error);
        EXPECT_EQ(txt.file.size(), 0);
        EXPECT_EQ(txt.lines.size(), 0);
}
