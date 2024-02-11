#include "gtest/gtest.h"
#include <stdlib.h>
#define OStr_IMPLEMENTAION
#include "OStr.h"
#if 0
typedef struct {
        size_t capacity;
        size_t size;
        size_t idx;
        char *at;
} OStr;

typedef struct {
        size_t row;
        size_t column;
        size_t idx;
} OStrCursor;

| ((m->row == p.row) & (m->column < p.column));

#endif

#define MEM_SIZE 1000*1024
TEST(OStr, move_cursor) {
        char *snk = (char*)malloc(MEM_SIZE);
        const char *s = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        for (size_t i = 0; i < strlen(s); i++) {
                snk[i] = s[i];
        } 
        snk[strlen(s)] = 0;
//The quick brown fox jumps over the lazy dog.\nI
        OStr str = {
                .capacity = MEM_SIZE,
                .size = strlen(s),
                .at = snk
        };
        OStrCursor cursor = { .row = 0, .column = 0, .idx = 0};
        TSPoint point = {.row = 1, .column = 0};
        size_t x = OStrCursor_move_to_point(&cursor, &str, point);
        EXPECT_EQ(cursor.idx, (size_t)45);
        EXPECT_EQ(x, (size_t)45);
}
