#include "gtest/gtest.h"
#define OARENA_IMPLEMENTATION
#include "OArena.h"

static HEAD_TYPE size_of_buffer(void const *buffer) {
        Memory *head = (Memory *)((uint8_t *)buffer - SIZE_OF_HEAD);
        return head->size;
}

static bool every_byte_is_zero(void const *buffer, HEAD_TYPE const size) {
        uint8_t *bytes = (uint8_t *)buffer;
        bool b = true;
        for (HEAD_TYPE i = 0; i < size; i++) {
                b &= (bytes[i] == 0);
        }
        return b;
}

TEST(OArena, make) {
        OArena *const arena = OArena_make(1000);
        EXPECT_EQ(arena->SIZE, (size_t)1000);
        EXPECT_EQ(arena->size, (size_t)0);
        EXPECT_EQ(sizeof(Memory), (size_t)8);  // <--- don't use this
        EXPECT_EQ(SIZE_OF_HEAD, (HEAD_TYPE)4); // <--- use this instead
}

TEST(OArena, malloc) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_malloc(arena, 42);
        EXPECT_EQ(arena->SIZE, (size_t)10000);
        EXPECT_EQ(arena->size, (size_t)SIZE_OF_HEAD + 42);
        EXPECT_EQ(size_of_buffer(buffer_0), (HEAD_TYPE)42);

        uint8_t *const buffer_1 = (uint8_t *)OArena_malloc(arena, 58);
        EXPECT_EQ(arena->size, (size_t)2*SIZE_OF_HEAD + 42 + 58);
        EXPECT_EQ(size_of_buffer(buffer_1), (HEAD_TYPE)58);
}

TEST(OArena, calloc) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_calloc(arena, 1, 42);
        EXPECT_EQ(arena->SIZE, (size_t)10000);
        EXPECT_EQ(arena->size, (size_t)SIZE_OF_HEAD + 42);
        EXPECT_EQ(size_of_buffer(buffer_0), (HEAD_TYPE)42);
        EXPECT_TRUE(every_byte_is_zero(buffer_0, size_of_buffer(buffer_0)));

        uint8_t *const buffer_1 = (uint8_t *)OArena_calloc(arena, 2, 50);
        EXPECT_EQ(arena->size, (size_t)2*SIZE_OF_HEAD + 42 + 100);
        EXPECT_EQ(size_of_buffer(buffer_1), (HEAD_TYPE)100);
        EXPECT_TRUE(every_byte_is_zero(buffer_1, size_of_buffer(buffer_1)));
}
