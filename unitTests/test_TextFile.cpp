#include "gtest/gtest.h"
#include <cstdlib>
#include <filesystem>
#include "TextFile.h"

TEST(TextVector, load) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        size_t const SIZE = 10000;
        char *buffer = (char*) malloc(SIZE);
        TextVector txt;
        txt.setMemory(buffer, SIZE);
        EXPECT_EQ(txt.size(), 0);
        char zero = 0;
        for (size_t i = 0; i < SIZE; i++) {
                zero |= txt[i];
        }
        EXPECT_EQ(zero, 0);
        bool error = false;
        error = txt.load(path.c_str(), error);
        ASSERT_FALSE(error);
        EXPECT_EQ(txt.size(), 87);
        EXPECT_EQ(txt[txt.size()], 0);
        zero = 0;
        for (size_t i = 0; i < SIZE; i++) {
                zero |= txt[i];
        }
        EXPECT_NE(zero, 0);
        txt.clear();
        EXPECT_EQ(txt.size(), 0);
        zero = 0;
        for (size_t i = 0; i < SIZE; i++) {
                zero |= txt[i];
        }
        EXPECT_EQ(zero, 0);
}

TEST(TextVector, buffer_SIZE_error) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        size_t const SIZE = 1;
        char *buffer = (char*) malloc(SIZE);
        TextVector txt;
        txt.setMemory(buffer, SIZE);
        bool error = false;
        error = txt.load(path.c_str(), error);
        EXPECT_TRUE(error);
        EXPECT_EQ(txt.size(), 0);
}


//////////////////////////////////////////////////////////////////////////////

TEST(TextFile, construct) {
        size_t capacity = 10000;
        char buffer[capacity];
        TextFile txt;
        txt.setMemory(buffer, capacity);
        EXPECT_EQ(txt.numOfLines(), 0);
}

TEST(TextFile, load0) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        size_t capacity = 10000;
        char buffer[capacity];
        TextFile txt;
        txt.setMemory(buffer, capacity);
        bool error = false;
        error = txt.load(path.c_str(), error);
        txt.setLineSlice();
        ASSERT_FALSE(error);
        EXPECT_EQ(txt.size(), 87);
        EXPECT_EQ(txt.numOfLines(), 7);
        EXPECT_EQ(txt.line(0).front, 0);
        EXPECT_EQ(txt.line(0).size, 18);
        EXPECT_EQ(txt.line(1).front, 19);
        EXPECT_EQ(txt.line(1).size, 0);
        EXPECT_EQ(txt.line(txt.numOfLines() - 1).front, 85);
        EXPECT_EQ(txt.line(txt.numOfLines() - 1).size, 1);
        EXPECT_EQ(txt.line(2)[1], 'h');
        EXPECT_EQ(txt[txt.size()], 0);
}

TEST(TextFile, load_error) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        size_t s = 10000;
        char buffer[s];
        TextFile txt;
        txt.setMemory(buffer, s);
        bool error = true;
        error = txt.load(path.c_str(), error);
        txt.setLineSlice();
        EXPECT_TRUE(error);
        EXPECT_EQ(txt.size(), 0);
        EXPECT_EQ(txt.numOfLines(), 0);
}
TEST(TextFile, append_character) {
        size_t s = 10000;
        char buffer[s];
        TextFile txt;
        txt.setMemory(buffer, s);
        EXPECT_EQ(txt.numOfLines(), 0);
        txt.append('o');
        EXPECT_EQ(txt.size(), 1);
        txt.append('o');
        EXPECT_EQ(txt.numOfLines(), 0);
        txt.append('o');
        txt.append('o');
        txt.append('o');
        txt.append('o');
        EXPECT_EQ(txt.size(), 6);
        EXPECT_EQ(txt.numOfLines(), 0);
        txt.setLineSlice();
        EXPECT_EQ(txt.numOfLines(), 1);
        txt.append('\n');
        EXPECT_EQ(txt.size(), 7);
        txt.setLineSlice();
        EXPECT_EQ(txt.numOfLines(), 2);
}

TEST(TextFile, example_copy) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        std::string path = std::filesystem::current_path();
        path.append("/../../unitTests/Example.txt");
        size_t const s0 = 10000;
        char buffer0[s0];
        TextFile txt;
        txt.setMemory(buffer0, s0);
        bool error = false;
        error = txt.load(path.c_str(), error);
        txt.setLineSlice();
        size_t const s1 = 10000;
        char buffer1[s1];
        TextFile copy;
        copy.setMemory(buffer1, s1);
        for (size_t idx = 0; idx < txt.size(); idx++) {
                copy.append(txt[idx]);
        }
        copy.setLineSlice();
        for (size_t idx = 0; idx < txt.size(); idx++) {
                EXPECT_EQ(copy[idx], txt[idx]);
                EXPECT_EQ(copy.size(), txt.size());
        }
        EXPECT_EQ(txt.numOfLines(), 7);
        for (size_t idx = 0; idx < txt.numOfLines(); idx++) {
                EXPECT_EQ(copy.line(idx).front,txt.line(idx).front);
                EXPECT_EQ(copy.line(idx).size,txt.line(idx).size);
                EXPECT_EQ(copy.line(idx).size,txt.line(idx).size);
                if (copy.line(idx).size > 0) {
                        EXPECT_EQ(copy.line(idx).at[0], txt.line(idx).at[0]);
                }
        }
        EXPECT_EQ(txt.line(2).at[1], 'h');
        EXPECT_EQ(copy.line(2).at[1], 'h');
}
