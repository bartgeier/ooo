#include "Pars.h"
#include "tree_sitter/api.h"
#include <assert.h>

// Declare the `tree_sitter_json` function, which is
// implemented by the `tree-sitter-json` library.
TSLanguage *tree_sitter_c();
OArena *arena = NULL;

void Pars_setArena(OArena *a) {
        arena = a;
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
        //ts_tree_delete(tree);
}

void Pars_freeTree(RootNode_t root) {
        arena->size = root.idx;
}
