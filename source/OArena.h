#ifndef OARENA_H
#define OARENA_H

#include <stdint.h>

typedef struct {
        uint8_t *at;
        size_t const SIZE;
        size_t size;
} OArena;

OArena *OArena_make(size_t const SIZE);
void *OArena_malloc(OArena *arena, size_t size);
void *OArena_calloc(OArena *arena, size_t nitems, size_t size);

 #define OARENA_IMPLEMENTATION
#ifdef OARENA_IMPLEMENTATION

#include <stddef.h>
#include <assert.h>
// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
OArena *OArena_make(size_t const SIZE) {
        OArena *arena = malloc(sizeof(OArena));
        OArena init = {.at = 0, .SIZE = SIZE, .size = 0};
        memcpy(arena, &init, sizeof(OArena));
        arena->at = malloc(SIZE);
        assert(arena->at != NULL && "buy more memory");
        return arena;
}

void *OArena_malloc(OArena *arena, size_t size) {
        assert(arena->size + size <= arena->SIZE && "Arena need more memory");
        uint8_t *p = arena->at + arena->size;
        arena->size += size;
        return (void *)p;
}

void *OArena_calloc(OArena *arena, size_t nitems, size_t size) {
        uint8_t *p = OArena_malloc(arena, nitems * size);
        for (size_t i = 0; i < nitems * size; i++) {
                p[i] = 0;
        }
        return (void *)p;
}

#endif
#endif
