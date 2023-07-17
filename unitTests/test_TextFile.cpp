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
        EXPECT_EQ(txt.file.size(), 87);
        EXPECT_EQ(txt.lines.size(), 7);
        EXPECT_EQ(txt.lines[0].start, 0);
        EXPECT_EQ(txt.lines[0].end, 18);
        EXPECT_EQ(txt.lines[1].start, 19);
        EXPECT_EQ(txt.lines[1].end, 19);
        EXPECT_EQ(txt.lines[txt.lines.size() - 1].start, 85);
        EXPECT_EQ(txt.lines[txt.lines.size() - 1].end, 86);
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

TEST(TextFile, first_character_in_line) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt;
        bool error = false;
        error = txt.load(path.c_str(), error);
        ASSERT_FALSE(error);
        EXPECT_TRUE(txt.first_character_in_line(txt.lines[5].start + 8));
        EXPECT_FALSE(txt.first_character_in_line(txt.lines[5].start + 9));
}

TEST(TextFile, last_character_in_line) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        TextFile txt;
        bool error = false;
        error = txt.load(path.c_str(), error);
        ASSERT_FALSE(error);
        EXPECT_FALSE(txt.last_character_in_line(txt.lines[5].start + 9));
        EXPECT_TRUE(txt.last_character_in_line(txt.lines[5].start + 10));
}
