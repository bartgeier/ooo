#ifndef OARENA_H
#define OARENA_H

#include <stdint.h>

typedef struct {
        size_t const SIZE;
        size_t size;
        uint8_t *at;
} OArena;

OArena *OArena_make(size_t const SIZE);
void *OArena_malloc(OArena *arena, size_t const size);
void *OArena_calloc(OArena *arena, size_t const nitems, size_t const size);
void *OArena_realloc(OArena *arena, void *buffer, size_t const size);
void OArena_free(OArena *arena, void *buffer);

#endif

//#define OARENA_IMPLEMENTATION
#ifdef OARENA_IMPLEMENTATION

#include <stddef.h>
#include <assert.h>
// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

#define HEAD_TYPE uint64_t
typedef struct {
        HEAD_TYPE size;
        uint8_t at[1];  // uint8_t at[]; flexible array don't work under C++ gtest
} Memory;

#define SIZE_OF_HEAD sizeof(HEAD_TYPE)


OArena *OArena_make(size_t const SIZE) {
        OArena *arena = (OArena *)malloc(sizeof(OArena));
        OArena init = {.SIZE = SIZE, .size = 0, .at = 0};
        memcpy((void *)arena, &init, sizeof(OArena));
        arena->at = (uint8_t *)malloc(SIZE);
        assert(arena->at != NULL && "buy more memory");
        return arena;
}

void *OArena_malloc(OArena *arena, size_t const num_of_bytes) {
        if (num_of_bytes == 0) return NULL;
        size_t padding = SIZE_OF_HEAD * ((num_of_bytes + SIZE_OF_HEAD - 1) / SIZE_OF_HEAD);
        assert(arena->size + SIZE_OF_HEAD + padding <= arena->SIZE && "OArena_malloc need more memory");
        Memory *mem = (Memory *)(arena->at + arena->size);
        mem->size = padding;
        arena->size = arena->size + SIZE_OF_HEAD + padding;
        return (void *)mem->at;
}

void *OArena_calloc(OArena *arena, size_t const nitems, size_t const size) {
        size_t num_of_bytes = nitems * size;
        uint8_t *p = (uint8_t *)OArena_malloc(arena, num_of_bytes);
        if (p == NULL) return NULL;
        memset(p, 0, num_of_bytes);
        return (void *)p;
}

void *OArena_realloc(OArena *arena, void *buffer, size_t const size) {
        if (buffer == NULL) return OArena_malloc(arena, size);
        Memory *mem = (Memory *)((uint8_t *)buffer - SIZE_OF_HEAD);
        void *arena_end = (arena->at + arena->size);
        void *buffer_end = (uint8_t *)buffer + mem->size;
        if (buffer_end == arena_end) {
                arena->size = arena->size - SIZE_OF_HEAD - mem->size;
                return OArena_malloc(arena, size);
        }
        if (mem->size >= size) {
                return buffer;
        }
        void *new_buffer = OArena_malloc(arena, size);
        memcpy(new_buffer, buffer, mem->size);
        return new_buffer;
}

void OArena_free(OArena *arena, void *buffer) {
        Memory *mem = (Memory *)((uint8_t *)buffer - SIZE_OF_HEAD);
        void *arena_end = (arena->at + arena->size);
        void *buffer_end = (uint8_t *)buffer + mem->size;
        if (buffer_end == arena_end) {
                arena->size -= (mem->size + SIZE_OF_HEAD);
        }
}

#endif
