#include <stdio.h>
#include <string.h>
#include "tree_sitter/api.h"
#define OSTR_IMPLEMENTAION
#include "OStr.h"

TSLanguage *tree_sitter_c();

typedef struct {
        OStrCursor cursor;
        OStr sink;
        OStr source;
} Job;

/* return true => successful */ 
bool read_txt_file(OStr *source, char const *path) {
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                return false;
        }
        source->size = 0;
        source->at[0] = 0;
        char chr;
        do {
                chr = getc(file);
                source->at[source->size] = chr;
                source->size++; 
        } while (chr != EOF);
        source->size--; 
        source->at[source->size] = 0;
        fclose(file);
        return true;
}

/* return true => successful */
bool write_txt_file(OStr const *source, char const *path) {
        if (strcmp(path,"-") == 0) { 
                printf("source_code:\n%s", source->at);
                return true;
        } else {
                FILE *file = fopen(path, "w");
                if (file == NULL) {
                        return false;
                }
                fprintf(file, "%s", source->at);
                fclose(file);
                return true;
        }
}

typedef enum {
        ENUM_SPECIFIER,
        TYPE_IDENTIFIER,
        UNKNOWN
} Symbol;

Symbol symbol(TSNode node) {
        if (ts_node_is_null(node)) return UNKNOWN;
        if (strcmp(ts_node_type(node), "type_identifier") == 0) return TYPE_IDENTIFIER;
        if (strcmp(ts_node_type(node), "enum_specifier") == 0) return ENUM_SPECIFIER;  
        return UNKNOWN;
}

void append_slice(OStr *snk, OStr const *src, size_t const begin, size_t const end) {
        for (size_t i = begin; i < end; i++) {
                snk->at[snk->size++] = src->at[i];
        }
}

bool equal_slice(char const *refstr, OStr const *str, size_t const begin, size_t const end){
        if (strlen(refstr) != end - begin) return false;
        for (size_t i = 0; i < end - begin; i++) {
                if (refstr[i] != str->at[i + begin]) return false;
        }
        return true;
}

void tree_runner(
        TSNode node,
        Job *job
) {
        TSNode parent_node = ts_node_parent(node);
        Symbol me = symbol(node); 
        Symbol parent = symbol(parent_node);  

        if (me == TYPE_IDENTIFIER & parent == ENUM_SPECIFIER) {
                size_t start_idx = OStrCursor_move_to_point(
                        &job->cursor, 
                        &job->source, 
                        ts_node_start_point(node)
                );
                size_t end_idx = OStrCursor_move_to_point(
                        &job->cursor, 
                        &job->source, 
                        ts_node_end_point(node)
                );
                if (equal_slice("ts_symbol_identifiers", &job->source, start_idx, end_idx)) {
                        QStr_append_cstring(&job->sink, "#ifndef OOO_TREESITTER_SYMBOL_IDS_H\n");
                        QStr_append_cstring(&job->sink, "#define OOO_TREESITTER_SYMBOL_IDS_H\n\n");
                        QStr_append_cstring(&job->sink, "/* This is an automatically generated file! */\n");
                        QStr_append_cstring(&job->sink, "/* Copyed from treesitter-c/src/parser.c    */\n");
                        QStr_append_cstring(&job->sink, "/* with source/copy_treesitter_symbols.c    */\n\n");
                        append_slice(
                                &job->sink,
                                &job->source,
                                OStrCursor_move_to_point( // -> start idx
                                        &job->cursor,
                                        &job->source, 
                                        ts_node_start_point(parent_node)
                                ),
                                OStrCursor_move_to_point( // -> end idx
                                        &job->cursor, 
                                        &job->source, 
                                        ts_node_end_point(parent_node)
                                )
                        );
                        QStr_append_cstring(&job->sink, ";\n");
                        QStr_append_cstring(&job->sink, "#endif\n");
                }
        }
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                tree_runner(child, job);
        }
}

#define MEM_SIZE 10000*1024
int main(int argc, char **argv) {
        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        Job job = {
                .cursor = {0},
                .sink =  {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source =  {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                }
        };
        if (!read_txt_file(&job.source, "./tree-sitter-c/src/parser.c")) return 1;
        OStr_remove_indentation(&job.sink, &job.source);
        OStr_move(&job.source, &job.sink);
        TSParser *parser = ts_parser_new();
        ts_parser_set_language(parser, tree_sitter_c());
        TSTree *tree = ts_parser_parse_string(
                parser,
                NULL,
                job.source.at,
                job.source.size
        );
        tree_runner(
                ts_tree_root_node(tree),
                &job
        );
        if (write_txt_file(&job.sink, "tree-sitter-c/ooo_treesitter_symbol_ids.h")) return 0;
        return 1;
}
