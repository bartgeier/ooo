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

static void buffer_set_string(uint8_t *buffer, char const *str, HEAD_TYPE size) {
        for (HEAD_TYPE i = 0; i < size; i++) {
                buffer[i] = str[i];
        }
}

static bool buffer_eq_string(uint8_t const *buffer, char const *str, HEAD_TYPE size) {
        bool result = true;
        for (HEAD_TYPE i = 0; i < size; i++) {
                result &= buffer[i] == str[i];
        }
        return result;
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

TEST(OArena, malloc_zero) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_malloc(arena, 42);
        EXPECT_EQ(arena->SIZE, (size_t)10000);
        EXPECT_EQ(arena->size, (size_t)SIZE_OF_HEAD + 42);
        EXPECT_EQ(size_of_buffer(buffer_0), (HEAD_TYPE)42);
        EXPECT_EQ(arena->size, (size_t)1*SIZE_OF_HEAD + 42);

        uint8_t *const buffer_1 = (uint8_t *)OArena_malloc(arena, 0);
        EXPECT_EQ(buffer_1, nullptr);
        EXPECT_EQ(arena->size, (size_t)1*SIZE_OF_HEAD + 42);
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

TEST(OArena, calloc_zero) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_calloc(arena, 1, 42);
        EXPECT_EQ(arena->SIZE, (size_t)10000);
        EXPECT_EQ(arena->size, (size_t)SIZE_OF_HEAD + 42);
        EXPECT_EQ(size_of_buffer(buffer_0), (HEAD_TYPE)42);
        EXPECT_TRUE(every_byte_is_zero(buffer_0, size_of_buffer(buffer_0)));

        uint8_t *const buffer_1 = (uint8_t *)OArena_calloc(arena, 0, 0);
        EXPECT_EQ(buffer_1, nullptr);
        EXPECT_EQ(arena->size, (size_t)SIZE_OF_HEAD + 42);
}
///////////////////////////////////////////////////////////////////////////////

TEST(OArena, realloc_the_same_at_last_allocation) {
        OArena *const arena = OArena_make(10000);
        OArena_malloc(arena, 13);
        uint8_t *const buffer_1 = (uint8_t *)OArena_malloc(arena, 3);
        buffer_set_string(buffer_1, "aBc", 3);
        uint8_t *const buffer_2 = (uint8_t *)OArena_realloc(arena, buffer_1, 3);
        EXPECT_EQ(buffer_2, buffer_1);
        EXPECT_EQ(size_of_buffer(buffer_1), (HEAD_TYPE)3);
        EXPECT_EQ(size_of_buffer(buffer_2), (HEAD_TYPE)3);
        EXPECT_TRUE(buffer_eq_string(buffer_2, "aBc", 3));
        EXPECT_EQ(arena->size, (size_t)13 + 3 + 2*SIZE_OF_HEAD);
}

TEST(OArena, realloc_increase_last_allocation) {
        OArena *const arena = OArena_make(10000);
        OArena_malloc(arena, 13);
        uint8_t *const buffer_1 = (uint8_t *)OArena_malloc(arena, 3);
        buffer_set_string(buffer_1, "aBc", 3);
        uint8_t *const buffer_2 = (uint8_t *)OArena_realloc(arena, buffer_1, 5);
        EXPECT_EQ(buffer_2, buffer_1);
        EXPECT_EQ(size_of_buffer(buffer_1), (HEAD_TYPE)5);
        EXPECT_EQ(size_of_buffer(buffer_2), (HEAD_TYPE)5);
        EXPECT_TRUE(buffer_eq_string(buffer_2, "aBc", 3));
        EXPECT_EQ(arena->size, (size_t)13 + 5 + 2*SIZE_OF_HEAD);
}

TEST(OArena, realloc_reduce_last_allocation) {
        OArena *const arena = OArena_make(10000);
        OArena_malloc(arena, 13);
        uint8_t *const buffer_1 = (uint8_t *)OArena_malloc(arena, 5);
        buffer_set_string(buffer_1, "hello", 3);
        uint8_t *const buffer_2 = (uint8_t *)OArena_realloc(arena, buffer_1, 3);
        EXPECT_EQ(buffer_2, buffer_1);
        EXPECT_EQ(size_of_buffer(buffer_1), (HEAD_TYPE)3);
        EXPECT_EQ(size_of_buffer(buffer_2), (HEAD_TYPE)3);
        EXPECT_TRUE(buffer_eq_string(buffer_2, "hel", 3));
        EXPECT_EQ(arena->size, (size_t)13 + 3 + 2*SIZE_OF_HEAD);
}

///////////////////////////////////////////////////////////////////////////////

TEST(OArena, realloc_the_same_at_first_allocation) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_malloc(arena, 13);
        buffer_set_string(buffer_0, "hello World!!", 13);
        OArena_malloc(arena, 3);
        uint8_t *const buffer_2 = (uint8_t *)OArena_realloc(arena, buffer_0, 13);
        EXPECT_EQ(buffer_2, buffer_0);
        EXPECT_EQ(size_of_buffer(buffer_0), (HEAD_TYPE)13);
        EXPECT_EQ(size_of_buffer(buffer_2), (HEAD_TYPE)13);
        EXPECT_TRUE(buffer_eq_string(buffer_2, "hello World!!", 13));
        EXPECT_EQ(arena->size, (size_t)13 + 3 + 2*SIZE_OF_HEAD);
}

TEST(OArena, realloc_reduce_first_allocation) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_malloc(arena, 13);
        buffer_set_string(buffer_0, "hello World!!", 13);
        OArena_malloc(arena, 3);
        uint8_t *const buffer_2 = (uint8_t *)OArena_realloc(arena, buffer_0, 5);
        EXPECT_EQ(buffer_2, buffer_0);
        EXPECT_EQ(size_of_buffer(buffer_0), (HEAD_TYPE)13);
        EXPECT_EQ(size_of_buffer(buffer_2), (HEAD_TYPE)13);
        EXPECT_TRUE(buffer_eq_string(buffer_2, "hello World!!", 13));
        EXPECT_EQ(arena->size, (size_t)13 + 3 + 2*SIZE_OF_HEAD);
}

TEST(OArena, realloc_increase_first_allocation) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_malloc(arena, 13);
        buffer_set_string(buffer_0, "hello World!!", 13);
        OArena_malloc(arena, 3);
        uint8_t *const buffer_2 = (uint8_t *)OArena_realloc(arena, buffer_0, 69);
        EXPECT_NE(buffer_2, buffer_0);
        EXPECT_EQ(size_of_buffer(buffer_0), (HEAD_TYPE)13);
        EXPECT_EQ(size_of_buffer(buffer_2), (HEAD_TYPE)69);
        EXPECT_TRUE(buffer_eq_string(buffer_2, "hello World!!", 13));
        EXPECT_EQ(arena->size, (size_t)13 + 3 + 69 + 3*SIZE_OF_HEAD);
}

///////////////////////////////////////////////////////////////////////////////

TEST(OArena, free) {
        OArena *const arena = OArena_make(10000);
        uint8_t *const buffer_0 = (uint8_t *)OArena_malloc(arena, 10);
        OArena_malloc(arena, 20);
        OArena_malloc(arena, 30);
        uint8_t *const buffer_2 = (uint8_t *)OArena_malloc(arena, 69);
        EXPECT_EQ(arena->size, (size_t)10 + 20 + 30 + 69 + 4*SIZE_OF_HEAD);

        OArena_free(arena, buffer_0);
        EXPECT_EQ(arena->size, (size_t)10 + 20 + 30 + 69 + 4*SIZE_OF_HEAD);
        OArena_free(arena, buffer_2);
        EXPECT_EQ(arena->size, (size_t)10 + 20 + 30 + 3*SIZE_OF_HEAD);
}
