#include "gtest/gtest.h"
#include "OStr.h"
#define REGEX_TAB_FILTER_IMPLEMENTATION
#include "Regex_tabFilter.h"

#define MEM_SIZE 1000*1024

TEST(Regex_tabFilter, do) {
        char *src = (char*)calloc(MEM_SIZE, 1);
        char *snk = (char*)calloc(MEM_SIZE, 1);
        const char *a = "The quick brown fox jumps over the lazy dog.\t\t\tIs there something.  \t Hello Sailor! Where\tare you coming frome.  Ok!";
        const char *b = "The quick brown fox jumps over the lazy dog. Is there something.    Hello Sailor! Where are you coming frome.  Ok!";
        strcpy(src, a);
        OStr source = {
                .capacity = MEM_SIZE,
                .size = (uint32_t)strlen(a),
                .at = src
        };
        OStr sink = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };

        Regex_tabFilter_t regex = {.state = RTF_CHAR};
        for (uint32_t i = 0; i < source.size; i++) {
                OptionalChar_t o = Regex_tabFilter(&regex, source.at[i]);
                if (o.ok) {
                        OStr_append_chr(&sink, o.chr);
                }
        }
        EXPECT_EQ(sink.size, (size_t)114);
        EXPECT_EQ(strlen(b), sink.size);
        EXPECT_TRUE(strncmp(b, sink.at, sink.size) == 0);
}
