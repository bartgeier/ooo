#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include "tree_sitter/api.h"

#include "treesitter_symbol_ids.h"
#include "ooo_runner.h"
#define OStr_IMPLEMENTAION
#include "StringView.h"

// Declare the `tree_sitter_json` function, which is
// implemented by the `tree-sitter-json` library.
TSLanguage *tree_sitter_c();

/* return true => error */ 
bool read_txt_file(OStr *source, char const *path) {
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                return true;
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
        return false;
}

bool is_single_line(TSNode n) {
        TSPoint const start = ts_node_start_point(n);
        TSPoint const end = ts_node_end_point(n);
        return start.row == end.row;
}


bool curly_brace_style_for_code_blocks(
        TSNode node,
        TSNode serial_node,
        OOO_Slice slice,
        OOO_Job *job
) {
        TSSymbol parent = ooo_parent(node);
        TSSymbol me = ts_node_symbol(node);
        TSSymbol grand = ooo_grand_parent(node);
        TSSymbol prev_sibling = ooo_previous_sibling(node);

        TSSymbol serial = ts_node_symbol(serial_node);
        TSSymbol serial_parent = ooo_parent(serial_node);

        if ((me == sym_compound_statement)
        &   (parent == sym_function_definition | parent == sym_compound_statement)) {
                /* Curly brace for function K&R begin or */
                /* nested code block compound statement  */
                /* function(bool example)\n{             */
                /* '\n{'                                 */
                OStr_append_chr(&job->sink, '\n'); 
                return false;
        }

        if ((me == sym_compound_statement)
        & !(parent == sym_function_definition | parent == sym_compound_statement)) {
                /* Curly brace begin code block if not a function or */
                /* not a nested code block compound statement        */
                /* if (x == 42) {                                    */
                /* ' {'                                              */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }

        if (parent == sym_compound_statement & prev_sibling == anon_sym_LBRACE) {
                /* new line after { begin code block compound statement */
                /* '{ ' or '{\n'                                        */
                if (is_single_line(ts_node_parent(node))) {
                        OStr_append_chr(&job->sink, ' ');
                } else {
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
 
        if (me == anon_sym_RBRACE & parent == sym_compound_statement) {
                /* end of an code block compound statement  */
                /* closed curly brace on a new line         */
                /* ' }' or '\n}'                            */
                if (is_single_line(ts_node_parent(node))) {
                        OStr_append_chr(&job->sink, ' ');
                } else {
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }

        if (prev_sibling != sym_function_definition 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_compound_statement
        & me == sym_else_clause) {
                /* end of an code block compound statement  */
                /* '} else'                                 */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }

        if (prev_sibling != sym_function_definition 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_compound_statement) {
                /* end of an code block compound statement  */
                /* closed curly brace on a new line         */
                /* '}\n' or '}\n\n'                         */
                size_t n = OStr_at_least_1_not_3(
                        &job->source, 
                        slice.begin, 
                        slice.end, 
                        '\n'
                );
                OStr_append_number_of_char(&job->sink, n, '\n');
                return false;
        }

        if (prev_sibling == sym_function_definition 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_compound_statement) {
                /* end of an function code block compound statement  */
                /* closed curly brace on a new line         */
                /* '}\n\n'                                  */
                OStr_append_number_of_char(&job->sink, 2, '\n');
                return false;
        }
        return true;
}

bool curly_brace_style_field_declaration_list(
        TSNode node,
        TSNode serial_node,
        OOO_Slice slice,
        OOO_Job *job
) {
        TSSymbol parent = ooo_parent(node);
        TSSymbol me = ts_node_symbol(node);
        TSSymbol grand = ooo_grand_parent(node);
        TSSymbol prev_sibling = ooo_previous_sibling(node);

        TSSymbol serial = ts_node_symbol(serial_node);
        TSSymbol serial_parent = ooo_parent(serial_node);

        if (me == sym_field_declaration_list) {
                /* struct Foo {  */
                /* ' {'          */
                OStr_append_chr(&job->sink, ' '); 
                return false;
        }

        if (parent == sym_field_declaration_list & prev_sibling == anon_sym_LBRACE) {
                /* struct Foo {  */
                /* '{ ' or '{\n' */
                if (is_single_line(ts_node_parent(node))) {
                        OStr_append_chr(&job->sink, ' ');
                } else {
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }

        if (me == anon_sym_RBRACE & parent == sym_field_declaration_list) {
                /* struct Foo { .... } */
                /* ' }' or '\n}'       */
                if (is_single_line(ts_node_parent(node))) {
                        OStr_append_chr(&job->sink, ' ');
                } else {
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }

        if (parent == sym_declaration 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_field_declaration_list) {
                /* 'struct Foo { ... }' */
                /* '} foo'              */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }

        return true;
}

bool curly_brace_style_initializer_list(
        TSNode node,
        TSNode serial_node,
        OOO_Slice slice,
        OOO_Job *job
) {
        TSSymbol parent = ooo_parent(node);
        TSSymbol me = ts_node_symbol(node);
        TSSymbol grand = ooo_grand_parent(node);
        TSSymbol prev_sibling = ooo_previous_sibling(node);

        TSSymbol serial = ts_node_symbol(serial_node);
        TSSymbol serial_parent = ooo_parent(serial_node);

        if (me == sym_initializer_list) {
                /* int [] Foo = { ... }  */
                /* ' {'          */
                OStr_append_chr(&job->sink, ' '); 
                return false;
        }

        if (parent ==  sym_initializer_list & prev_sibling == anon_sym_LBRACE) {
                /* int [] Foo = { ... }  */
                /* '{' or '{\n' */
                if (is_single_line(ts_node_parent(node))) {
                        // OStr_append_chr(&job->sink, ' ');
                } else {
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }

        if (me == anon_sym_RBRACE & parent == sym_initializer_list) {
                /* int [] Foo = { ... }  */
                /* '}' or '\n}'  */
                if (is_single_line(ts_node_parent(node))) {
                        // OStr_append_chr(&job->sink, ' ');
                } else {
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }

#if 0 
        if (parent == sym_declaration 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_initializer_list
        & me == anon_sym_SEMI) {
                /* semikolon ; */
                /* '};'          */
                //OStr_append_chr(&job->sink, '');
                return false;
        }
#endif
        return true;
}

bool ooo_rule_dispatcher(
        TSNode node,
        TSNode serial_node,
        OOO_Slice slice,
        OOO_Job *job
) {
        return curly_brace_style_for_code_blocks(node, serial_node, slice, job)
        && curly_brace_style_field_declaration_list(node, serial_node, slice, job)
        && curly_brace_style_initializer_list(node, serial_node, slice, job);
        /* return false -> rule applyed and done */
}

size_t ooo_indentation(OOO_Transition const transition, TSNode const node, size_t level) {
        TSSymbol me = ts_node_symbol(node);
        TSSymbol parent = ooo_parent(node);
        TSSymbol grand = ooo_grand_parent(node);
        TSSymbol prev_sibling = ooo_previous_sibling(node);
        TSSymbol prev_grand_sibling = ooo_previous_grand_siblin(node);

        switch (transition) { 
        case OOO_ENTRY:
                if ((me != sym_compound_statement) & (me != sym_else_clause)
                & (parent == sym_if_statement 
                | (parent == sym_else_clause & me != anon_sym_else)
                |  parent == sym_for_statement
                |  parent == sym_while_statement)) {
                        /* indent after if wihtout curly brace */
                        /* if/else/for/while (true)            */
                        /* ----->do_something();               */
                        level++;
                }

                if (me == anon_sym_RBRACE & grand != sym_switch_statement & grand != sym_case_statement
                &  (parent == sym_compound_statement
                |   parent == sym_field_declaration_list
                |   parent == sym_initializer_list)) {
                        /* '}'  unindented */
                        level--;
                }

                if (me == anon_sym_RBRACE & grand == sym_case_statement & prev_grand_sibling != anon_sym_COLON
                &  (parent == sym_compound_statement)) {
                        /* '}'  unindented */
                        level--;
                }
                return level;
        case OOO_NEXT:  
                if ((me == sym_compound_statement 
                & parent != sym_switch_statement 
                & parent != sym_case_statement)
                |   me == sym_field_declaration_list
                |   me == sym_initializer_list) {
                        /* after '{'  indented */
                        level++;
                }
                if (me == sym_compound_statement 
                &   parent == sym_case_statement 
                &   prev_sibling != anon_sym_COLON) {
                        level++;
                }
                if (me == sym_case_statement) {
                        level++;
                }
                return level;
        case OOO_EXIT:  
                return level;
        }
}

char const *shift_args(int *argc, char const ***argv) {
        assert(*argc > 0);
        char const *result = **argv;
        *argc -= 1;
        *argv += 1;
        return result;
}
#define MEM_SIZE 1000*1024
int main(int argc, char const **argv) {
        printf("%s\n", shift_args(&argc, &argv));
        struct { 
                bool b; 
                size_t begin;
                size_t end;
        } print = { false, 0, 0 }; 
        while (argc > 0) {
                if (strcmp(shift_args(&argc, &argv), "p") == 0) {
                        if (argc > 0) {
                                errno = 0;
                                char *endptr;
                                char const *astr = shift_args(&argc, &argv); 
                                print.begin = strtol(astr, &endptr, 10); 
                                if (endptr == astr) break;
                        } else {
                                break;
                        }
                        if (argc > 0) {
                                errno = 0;
                                char *endptr;
                                char const *astr = shift_args(&argc, &argv); 
                                print.end = strtol(astr, &endptr, 10); 
                                if (endptr == astr) break;
                        } else {
                                break;
                        }
                        print.b = true;
                }
        }

        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OOO_Job job = {
                .cursor = {0},
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                }
        };

        // const char *file_path = "examples/CipActionDispatcher.c";
        read_txt_file(&job.source, "examples/CipActionDispatcher.c");
#if 1
        OStr_replace_tabs_with_one_space(&job.sink, &job.source);
        OStr_replace_tabs_with_one_space(&job.source, &job.sink);
        OStr_remove_indentation(&job.sink, &job.source);
        OStr_move(&job.source, &job.sink);
#endif
        TSParser *parser = ts_parser_new();
        ts_parser_set_language(parser, tree_sitter_c());
        TSTree *tree = ts_parser_parse_string(
                parser,
                NULL,
                job.source.at,
                job.source.size
        );
        if (print.b) { 
                ooo_print_nodes(
                        ts_tree_root_node(tree),
                        print.begin, // start row
                        print.end,   // end row
                        0            // level
                );
                return 0;
        }
        OStrCursor_reset(&job.cursor);
        ooo_visit_nodes(
                ts_tree_root_node(tree),
                ts_tree_root_node(tree),
                &job
        );

        OStr_clear(&job.source);

        OStrCursor_reset(&job.cursor);
        tree = ts_parser_parse_string(
                parser,
                NULL,
                job.sink.at,
                job.sink.size
        );
        printf("sink_code:\n%s", job.sink.at);
        ooo_set_indentation(
                &job.cursor,
                &job.source,
                &job.sink,
                ts_tree_root_node(tree),
                0
        );
        // job.source.at[2200] = 0;
        printf("source_code:\n%s", job.source.at);
        ts_tree_delete(tree);
        ts_parser_delete(parser);
        return 0;
}
