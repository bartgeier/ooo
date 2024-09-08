#ifndef OARENA_H
#define OARENA_H

#include <stdint.h>

typedef struct {
        size_t const SIZE;
        size_t size;
        uint8_t *at;
} OArena;

OArena *OArena_make(size_t const SIZE);
void *OArena_malloc(OArena *arena, size_t size);
void *OArena_calloc(OArena *arena, size_t nitems, size_t size);
void *OArena_realloc(OArena *arena, void *buffer, size_t size);

#endif

//#define OARENA_IMPLEMENTATION
#ifdef OARENA_IMPLEMENTATION

#include <stddef.h>
#include <assert.h>
// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

#define HEAD_TYPE uint32_t
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

void *OArena_malloc(OArena *arena, size_t num_of_bytes) {
        assert(arena->size + SIZE_OF_HEAD + num_of_bytes <= arena->SIZE && "OArena_malloc need more memory");
        Memory *mem = (Memory *)(arena->at + arena->size);
        mem->size = num_of_bytes;
        arena->size = arena->size + SIZE_OF_HEAD + num_of_bytes;
        return (void *)mem->at;
}

void *OArena_calloc(OArena *arena, size_t nitems, size_t size) {
        uint8_t num_of_bytes = nitems * size;
        uint8_t *p = (uint8_t *)OArena_malloc(arena, num_of_bytes);
        memset(p, 0, num_of_bytes);
        return (void *)p;
}

void *OArena_realloc(OArena *arena, void *buffer, size_t size) {
        Memory *mem = (Memory *)((uint8_t *)buffer - SIZE_OF_HEAD);
        void *arena_end = (arena->at + arena->size);
        void *buffer_end = (uint8_t *)buffer + mem->size;
        if (buffer_end == arena_end) {
                if (mem->size >= size) {
                        arena->size -= (mem->size - size);
                } else {
                        arena->size += (size - mem->size);
                }
                mem->size = size;
                return buffer;
        }
        if (mem->size >= size) {
                return buffer;
        }
        void *new_buffer = OArena_malloc(arena, size);
        memcpy(new_buffer, buffer, mem->size);
        return new_buffer;
}

// #ifdef __cplusplus
// }
// #endif

#endif
