#include "Pars.h"
#include "tree_sitter/api.h"
#include <assert.h>
#define OARENA_IMPLEMENTATION
#include "OArena.h"

OArena *arena = NULL;

void *arena_malloc(size_t size) {
        void *p = OArena_malloc(arena, size);
        return p;
}

void *arena_calloc(size_t nitems, size_t size) {
        void *p = OArena_calloc(arena, nitems, size);
        return p;
}

void *arena_realloc(void *buffer, size_t size) {
        void *p = OArena_realloc(arena, buffer, size);
        return p;
}

void arena_free(void *buffer) {
        OArena_free(arena, buffer);
}

// Declare the `tree_sitter_json` function, which is
// implemented by the `tree-sitter-c` library.
TSLanguage *tree_sitter_c(void);

void Pars_init(void) {
        arena = OArena_make(10 * 1024 * 1024);
        ts_set_allocator(arena_malloc, arena_calloc, arena_realloc, arena_free);
}

RootNode_t Pars_getTree(char const *string, uint32_t size) {
        assert(arena != NULL);
        RootNode_t root = {0};
        root.idx = arena->size;
        TSParser *parser = ts_parser_new();
        ts_parser_set_language(parser, tree_sitter_c());
        TSTree *tree = ts_parser_parse_string(
                parser,
                NULL,
                string,
                size
        );
        root.node = ts_tree_root_node(tree);
        return root;
}

void Pars_freeTree(RootNode_t root) {
        arena->size = root.idx;
}
