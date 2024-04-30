#include "ruler.h"
#include "OJob.h"
#include "OStr.h"
#include "tree_navigator.h"
#include <stdio.h>

#define SYM_ERROR 65535

static bool preproc_include(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_preproc_include) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (me(node) == sym_string_literal) {
                /* #include "foo.h" */
                /*         ^        */
                OJob_space(job);
                //OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me(node) == sym_system_lib_string) {
                /* #include <foo.h> */
                /*         ^        */
                OJob_space(job);
                return false;

        }
        return true;
}

static bool preproc_def(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_preproc_def) {
                return true;
        }
        if (is_first_child(node)) {
                /* #define */
                return false;
        }
        if (me(node) == sym_identifier) {
                /* #define ALICE */
                /*        ^      */
                OJob_space(job);
                return false;
        }
        return true;
}

static bool preproc_ifdef(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_preproc_ifdef) {
                return true;
        }
        if (is_first_child(node)) {
                /* #ifndef or #ifdef */
                return false;
        }
        if(me(node) == sym_identifier) {
                /* #ifndef FOO_H  */
                /*        ^       */
                OJob_space(job);
                return false; 
        }
        if (ooo(Nodes_at(node->nodes, 1)) == sym_identifier 
        & ooo(Nodes_at(node->nodes, 2)) == aux_sym_preproc_def_token1
        & ooo(Nodes_at(node->nodes, 3)) == sym_preproc_def
        & ooo(Nodes_at(node->nodes, 4)) == sym_identifier
        & ooo(Nodes_at(node->nodes, 5)) == aux_sym_preproc_ifdef_token2
        & ooo(Nodes_at(node->nodes, 6)) == sym_preproc_ifdef) {
                /* #ifndef FOO_H   */
                /* #define FOO_H\n\n */
                /*              ^ ^  */
                OJob_2LF(job, slice);
                return false;
        }
        if (me(node) == aux_sym_preproc_if_token2 & is_last_child(node)) {
                /* \n#endif */
                /* ^        */
                if (me_size(node) > 0) {
                        OJob_1_or_2LF(job, slice);
                        return false;
                }
                /* #ifdef __cplusplus #endif              */
                /*                      ^ node size == 0  */
                return false;
        }
        if (me(node) == sym_linkage_specification) {
                /* \nextern "C" { */
                /* ^              */
                OJob_LF(job, slice);
                return false;
        }
        return true;
}

static bool translation_unit(Relation const *node, Slice const slice, OJob *job) {
        /* root of an file */
        if (parent(node) != sym_translation_unit) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        return true;
}

static bool linkage_specification(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_linkage_specification) {
                return true;
        }
        if (is_first_child(node)) {
                /* extern */
                return false;
        }
        if (me(node) == sym_string_literal) {
                /* extern "C" */
                /*       ^    */
                OJob_space(job);
                return false;
        }
        if (me(node) == sym_declaration_list) {
                /* extern "C" { */
                /*           ^  */
                OJob_space(job);
                return false;
        }
        return true;
}

static bool declaration_list(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_declaration_list) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (ooo(Nodes_at(node->nodes, 1)) == sym_preproc_directive
        & ooo(Nodes_at(node->nodes, 2)) == sym_preproc_call
        & ooo(Nodes_at(node->nodes, 3)) == anon_sym_LBRACE
        & ooo(Nodes_at(node->nodes, 4)) == sym_declaration_list) {
                /* #ifdef __cplusplus  */
                /* extern "C" {        */
                /* #endif\n\n          */
                /*         ^           */
                OJob_2LF(job, slice);
                return false;
        }
        if (me(node) == anon_sym_RBRACE) {
                /* #ifdef __cplusplus */
                /* \n}                */
                /* ^                  */
                OJob_LF(job, slice);
                return false;
        }
        return true;
}

static bool function_definition(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_function_definition) {
                return true;
        }
        if (is_first_child(node)) {
                /* static void foo(int a, int b) */
                /* static                        */
                return false;
        }
        if (me(node) == sym_primitive_type) {
                /* static void foo(int a, int b) */
                /*       ^                       */
                OJob_space(job);
                return false;
        }
        if (me(node) == sym_storage_class_specifier) {
                /* extern static void foo(int a, int b) */
                /*       ^                       */
                OJob_space(job);
                return false;
        }
        if (me(node) == sym_function_declarator) {
                /* extern static void foo(int a, int b) */
                /*                   ^                  */
                OJob_space(job);
                return false;
        }
        if (me(node) == sym_compound_statement) {
                /* extern static void foo(int a, int b) { */
                /*                                     ^  */ // todo K&R option
                OJob_space(job);
                return false;
        }
        return true;
}

static bool function_declarator(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_function_declarator) {
                return true;
        }
        if (is_first_child(node)) {
                /* void foo(int a, int b) */
                /*      foo               */
                return false;
        }
        if (me(node) == sym_parameter_list) {
                /* void foo(int a, int b) */
                /*        ^^              */
                return false;
        }
        return true;
}

static bool parameter_list(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_parameter_list) {
                return true;
        }
        if (is_first_child(node)) {
                /* void foo(int a, int b) */
                /*         (              */
                return false;
        }
        if (node->child_idx == 1) {
                /* second child */
                if (parent_num_of_lines(node) == 0) {
                        /* void foo(int a  */
                        /*         ^^      */
                        return false;
                } else {
                        /* void foo(\nint a  */
                        /*          ^        */
                        OJob_LF(job, slice);
                        return false;
                }
        }
        if (is_last_child(node)) {
                if (parent_num_of_lines(node) == 0) {
                        /* void foo(int a, int b)  */
                        /*                     ^^  */
                        return false;
                } else {
                        /* void foo(\nint a,\nint b\n)  */
                        /*                          ^   */
                        OJob_LF(job, slice);
                        return false;
                }
        }
        if (me(node) == anon_sym_COMMA) {
                /* void foo(int a, int b) */
                /*              ^^        */
                return false;
        }
        if (me(node) == sym_parameter_declaration) {
                /* void foo(int a, int b) */
                /*                ^       */
                OJob_LF_or_space(job, slice);
                return false;
        }
        return true;
}

static bool parameter_declaration(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_parameter_declaration) {
                return true;
        }
        if (is_first_child(node)) {
                /* void foo(int a, int b) */
                /*          int             */
                return false;
        }
        if (is_last_child(node)) {
                /* void foo(int a, int b) */
                /*             ^      ^   */
                OJob_space(job);
                return false;
        }
        return true;
}

static bool enum_specifier(Relation const *node, Slice slice, OJob *job) {
        if (parent(node) != sym_enum_specifier) {
                return true;
        }
        if (is_first_child(node)) {
                /* enum alice */
                /* enum       */
                return false;
        }
        if (is_last_child(node)) {
                /* enum alice { */
                /*     ^     ^  */
                OJob_space(job);
                return false;
        }
        return true;
}

static bool union_specifier(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_union_specifier) {
                return true;
        }
        if (is_first_child(node)) {
                /* union alice */
                /* union       */
                return false;
        }
        if (is_last_child(node)) {
                /* union alice */
                /*      ^      */
                OJob_space(job);
                return false;
        }
        return true;
}

static bool compound_statement(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_compound_statement) {
                return true;
        }
        if (is_first_child(node)) {
                /* { */
                /* { */
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        if (parent_num_of_lines(node) == 0) {
                OJob_space(job);
                return false;
        }
        if (node->child_idx == 1) {
                /* second child */
                /* {\n */
                /*  ^  */
                OJob_LF(job, slice);
                return false;
        }
        if (is_last_child(node)) {
                /* \n} */
                /*  ^  */
                OJob_LF(job, slice);
                return false;
        }
        return true;
}

static bool pointer_declarator(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_pointer_declarator) {
                return true;
        }
        if (is_first_child(node)) {
                /* *alice */
                /* *      */
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        /* *alice */
        /* ^^     */
        return false;
}

static bool declaration(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_declaration) {
                return true;
        }
        if (is_first_child(node)) {
                /* bool alice */
                /* bool      */
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        if (me(node) == anon_sym_COMMA) {
                /* int32_t a, b = 1; */
                /*         ^^        */
                return false;
        }
        if (me(node) == anon_sym_SEMI) {
                /* bool alice; */
                /*          ^^ */
                return false;
        }
        /* static bool alice; */
        /*       ^    ^       */
        OJob_space(job);
        return false;
}

static bool init_declarator(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_init_declarator) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        /* Record a = {1, 2, 3}; */
        /*         ^ ^           */
        OJob_space(job);
        return false;
}

static bool struct_specifier(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_struct_specifier) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        /* struct a { */
        /*       ^ ^  */
        OJob_space(job);
        return false;
}

static bool field_declaration_list(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_field_declaration_list) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        /* struct big {\nint a;\nint b;\n} */
        /*              ^       ^       ^  */
        OJob_LF(job, slice);
        return false;
}

static bool field_declaration(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_field_declaration) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* uint8_t big; */
                /*           ^^ */
                return false;
        }
        /* uint8_t big; */
        /*        ^     */
        OJob_space(job);
        return false;
}

static bool initializer_list(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_initializer_list) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        if (me(node) == anon_sym_COMMA) {
                /* {.a = 1, */
                /*       ^^ */
                return false;
        }
        if (parent_num_of_lines(node) == 0) {
                if (node->child_idx == 1) {
                        /* is_second_child */
                        /* {.a = 1, */
                        /* ^^       */
                        return false;
                }
                if (me(node) == anon_sym_RBRACE) {
                        /* .a = 1} */
                        /*      ^^ */
                        return false;
                }
                OJob_space(job);
                return false;
        } else {
                if (node->child_idx == 1) {
                        /* {\n.a = 1, */
                        /* ^^         */
                        OJob_LF(job, slice);
                        return false;
                }
                if (me(node) == anon_sym_RBRACE) {
                        /* .a = 1\n} */
                        /*        ^  */
                        OJob_LF(job, slice);
                        return false;
                }
                OJob_LF_or_space(job, slice);
                return false;
        }
        return true;
}

static bool initializer_pair(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_initializer_pair) {
                return true;
        }
        if (is_first_child(node)) {
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        /* .cut = LED_ON, */
        /*     ^ ^       */
        OJob_space(job);
        return false;
}

static bool array_declarator(Relation const *node, Slice const slice, OJob *job) {
        if (parent(node) != sym_array_declarator) {
                return true;
        }
        if (is_first_child(node)) {
                /* a[] */
                /* a   */
                return false;
        }
        if (me(node) == sym_comment | me(node) == SYM_ERROR) {
                return true;
        }
        /* a[4] */
        /* ^^^^ */
        return false;
}

static bool type_definition(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes,0);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol me = ooo(node);
        if (parent != sym_type_definition) {
                return true;
        }
        if (first_sibling(node)) {
                /* typedef enum { A, B, C } ABC; */
                /* typedef                       */
                return false;
        }
        if (me == alias_sym_type_identifier) {
                /* typedef enum { A, B, C } ABC; */
                /*                         ^     */
                OStr_append_chr(&job->sink, ' ');
                return false;

        }
        if (last_sibling(node)) {
                /* typedef struct { A, B, C } ABC; */
                /*                              ^^ */
                return false;
        }
        /* typedef enum { A, B, C } ABC; */
        /*        ^                      */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool enumerator_list(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol me = ooo(node);
        if (parent != sym_enumerator_list) {
                return true;
        }
        if (first_sibling(node)) {
                /* enum { */
                /*      { */
                return false;
        }
        if (me == anon_sym_COMMA) {
                /* SECTOR_A, */
                /*        ^^ */
                return false;
        }
        if (is_single_line(super(1, node))) {
                /* SECTOR_A, SECTOR_B */
                /*          ^         */
                OStr_append_chr(&job->sink, ' ');
                return false;
        } else {
                /* SECTOR_A,\nSECTOR_B */
                /*          ^          */
                size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                return false;
        }
        return true;
}

static bool enumerator(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_enumerator) {
                return true;
        }
        if (first_sibling(node)) {
                /* FlashState_READY = 0 */
                /* FlashState_READY   */
                return false;
        }
        /* FlashState_READY = 0 */
        /*                 ^ ^  */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool expression_statement(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_expression_statement) {
                return true;
        }
        if (first_sibling(node)) {
                /* ach(); */
                /* acht() */
                return false;
        }
        if (last_sibling(node)) {
                /* ach(); */
                /*     ^^ */
                return false;
        }
        return true;
}

static bool call_expression(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_call_expression) {
                return true;
        }
        if (first_sibling(node)) {
                /* foo(int a) */
                /* foo        */
                return false;
        }
        /* foo(int a) */
        /*   ^^       */
        return false;
}

static bool argument_list(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol me = ooo(node);
        if (parent != sym_argument_list) {
                return true;
        }
        if (first_sibling(node)) {
                /* function(alice */
                /*        ^^      */
                return false;
        }
        if (second_sibling(node)) {
                if(is_single_line(ts_node_parent(node))) {
                        /* void foo(alice  */
                        /*         ^^      */
                        return false;
                } else {
                        /* void foo(\nalice  */
                        /*          ^        */
                        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
        }
        if (me == anon_sym_COMMA) {
                /* void foo(alice,  */
                /*              ^^  */
                return false;
        }
        if (last_sibling(node)) {
                if(is_single_line(ts_node_parent(node))) {
                        /* void foo(alice, bob)  */
                        /*                   ^^  */
                        return false;
                } else {
                        /* void foo(\nalice, bob\n)  */
                        /*                       ^   */
                        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
        }
        /* void foo(int a, int b) */
        /*                ^       */
        char const chr = OStr_need_1LF_or_1Space(&job->source, slice);
        OStr_append_chr(&job->sink, chr);
        return false;
}

static bool for_statement(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol me = ooo(node);
        TSNode parent_node = super(1, node);
        TSSymbol parent = ooo(parent_node);
        if (parent != sym_for_statement) {
                return true;
        }
        bool single_line = false;
        int l = find_child(parent_node, anon_sym_LPAREN);
        int r = find_child(parent_node, anon_sym_RPAREN);
        if ( l >= 0 & r >= 0) {
                size_t A = ts_node_start_point(child(l, parent_node)).row;
                size_t B = ts_node_end_point(child(r, parent_node)).row;
                single_line = ((A - B) == 0);
        }
        if (first_sibling(node)) {
                /* for */
                return false;
        }
        if (me == anon_sym_LPAREN) {
                /* for ( */
                /*    ^  */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == anon_sym_SEMI) {
                /* for (int i; i < max; i++) */
                /*                   ^^      */
                return false;
        }
        /* todo comment between ) and las_sibling and then \n */
        if (single_line) {
                if (ooo(Nodes_at(nodes, 1)) == anon_sym_LPAREN) {
                        /* for (int i; i < max; i++) */
                        /*     ^^                    */
                        return false;
                }
                if (me == anon_sym_RPAREN) {
                        /* for (int i; i < max; i++) */
                        /*                        ^^ */
                        return false;
                }
                if (last_sibling(node)) {
                        if (me == sym_compound_statement) {
                                /* for (int i; i < max; i++) { */
                                /*                          ^  */
                                OStr_append_chr(&job->sink, ' ');
                                return false;

                        } else {
                                /* for (int i; i < max; i++) doSomething();  */
                                /* for (int i; i < max; i++)\ndoSomething(); */
                                /*                          ^                */
                                char const ch = OStr_need_1LF_or_1Space(&job->source, slice);
                                OStr_append_chr(&job->sink, ch);
                                return false;
                        }
                }
                OStr_append_chr(&job->sink, ' ');
                return false;
        } else {
                /* multi line */
                if (last_sibling(node)) {
                        /* for (\nint i;\ni < max;\ni++\n) doSomething(); */
                        /* for (\nint i;\ni < max;\ni++\n) {              */
                        /*                                ^               */
                        OStr_append_chr(&job->sink, ' ');
                        return false;
                }
                if (me == sym_comment) {
                        OStr_append_chr(&job->sink, ' ');
                        return false;
                }
                /*  for (\nint i;\ni < max;\ni++\n) { */
                /*               ^         ^     ^    */
                size_t num_of_LF = OStr_need_1LF(&job->source, slice);
                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                return false;
        }
        return true;
}

static bool while_statement(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_while_statement) {
                return true;
        }
        if (first_sibling(node)) {
                /* while */
                return false;
        }
        if (last_sibling(node)) {
                TSSymbol me = ooo(node);
                if (me == sym_compound_statement) {
                        /* while (true) { */
                        /*             ^  */
                        OStr_append_chr(&job->sink, ' ');
                        return false;
                } else {
                        /* while (true) doSomething(); */
                        /* while (true)\ndoSomething(); */
                        /*             ^               */
                        char const ch = OStr_need_1LF_or_1Space(&job->source, slice);
                        OStr_append_chr(&job->sink, ch);
                        return false;
                }
        }
        /* todo comment between ) and las_sibling and then \n */

        /* while comment ( */
        /*      ^       ^  */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool do_statement(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_do_statement) {
                return true;
        }
        if (first_sibling(node)) {
                /* do */
                return false;
        }
        if (last_sibling(node)) {
                /* while (1); */
                /*         ^^ */
                return false;
        }
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool if_statement(Relation *r, Slice slice, OJob *job) {
        if (ooo(r->parent) != sym_if_statement) {
                return true;
        }
        if (is_first_child(r)) {
                return false;
        }
        if (is_single_line(r->parent)) {
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        TSSymbol me = ooo(Nodes_at(r->nodes, 0));
        if (me == sym_comment) {
                /* if comment (true) comment foo();    */
                /* if\ncomment (true)\ncomment foo();  */
                /*   ^               ^                 */
                char const ch = OStr_need_1LF_or_1Space(&job->source, slice);
                OStr_append_chr(&job->sink, ch);
                return false;
        }
        if (me == sym_parenthesized_expression) {
                /* if (true) foo();  */
                /* if (true)\nfoo(); */
                /*   ^               */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (has_child(r, sym_compound_statement)) {
                OStr_append_chr(&job->sink, ' ');
                return false;
        } 
        /* if (true) foo();  */
        /* if (true)\nfoo(); */
        /*          ^        */
        size_t num_of_LF = OStr_need_1LF(&job->source, slice);
        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
        return false;
}

static bool else_clause(Relation *node, Slice slice, OJob *job)  {
        if (parent(node) != sym_else_clause) {
                return true;
        }
        if (is_first_child(node)) {
                /* else */
                return false;
        }
        if (is_single_line(node->grand)) {
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me(node) == sym_comment) {
                /* else comment */
                /*     ^        */
                char const ch = OStr_need_1LF_or_1Space(&job->source, slice);
                OStr_append_chr(&job->sink, ch);
                return false;
        }
        if (has_child(node, sym_compound_statement)) {
                /* else { */
                /*     ^  */
                OStr_append_chr(&job->sink, ' ');
                return false;
        } 
        /* else foo();  */
        /* else\nfoo(); */
        /*     ^        */
        size_t num_of_LF = OStr_need_1LF(&job->source, slice);
        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
        return false;
}


static bool parentesized_expression(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_parenthesized_expression) {
                /* (a == 0) */
                return true;
        }
        if (first_sibling(node)) {
                /* (a == 0) */
                /* (        */
                return false;
        }
        if (ooo(Nodes_at(nodes, 1)) == anon_sym_LPAREN) {
                /* (a == 0) */
                /* ^^       */
                return false;
        }
        if (me == anon_sym_RPAREN) {
                /* (a == 0) */
                /*       ^^ */
                return false;
        }
        return true;
}

static bool binary_expression(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol me = ooo(node);
        TSNode parent_node = super(1, node);
        TSSymbol parent = ooo(parent_node);
        if (parent != sym_binary_expression) {
                return true;
        }
        if (first_sibling(node)) {
                /* a != 0 */
                /* a      */
                return false;
        }
        if (is_single_line(parent_node)) {
                /* a != 0 */
                /*  ^  ^  */
                OStr_append_chr(&job->sink, ' ');
                return false;  
        } else {
                if (last_sibling(node)) {
                        /* a\n!= 0 */
                        /*      ^  */
                        OStr_append_chr(&job->sink, ' ');
                        return false;
                }
                if (me != sym_comment) {
                        /* a\n!= 0 */
                        /*  ^      */
                        /* todo comment destroys code //comment -> block comment */ 
                        /* todo is_operator(node) would be better because of macros and comments */
                        OStr_append_chr(&job->sink, '\n');
                        return false;  
                }
                /* a\n!= 0 */
                /*      ^  */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        return true;
}

static bool roots(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (parent == sym_translation_unit 
        | parent == sym_preproc_ifdef
        | parent == sym_linkage_specification
        | parent == sym_declaration_list
        ) {
                size_t num_of_LF; 
                switch (me) {
                case sym_function_definition:
                case sym_comment:
                        if (prev_sibling == sym_comment) {
                                num_of_LF = OStr_need_1LF(&job->source, slice);
                                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                                return false;
                        }
                        num_of_LF = OStr_need_2LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                case sym_declaration:
                        if (prev_sibling == sym_comment) {
                                num_of_LF = OStr_need_1_or_2LF(&job->source, slice);
                                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                                return false;
                        }
                        if (!is_single_line(node) 
                        | !is_single_line(sibling(-1, node))  
                        | prev_sibling != sym_declaration ) {
                                num_of_LF = OStr_need_2LF(&job->source, slice);
                                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                                return false;
                        } 
                        num_of_LF = OStr_need_1_or_2LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                case sym_type_definition:
                        if (prev_sibling == sym_comment) {
                                num_of_LF = OStr_need_1_or_2LF(&job->source, slice);
                                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                                return false;
                        }
                        if (!is_single_line(node) 
                        | !is_single_line(sibling(-1, node))
                        | prev_sibling != sym_type_definition) {
                                num_of_LF = OStr_need_2LF(&job->source, slice);
                                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                                return false;
                        } 
                        num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                default:
                        //if (!first_sibling(node)) {
                        //        num_of_LF = OStr_need_1_or_2LF(&job->source, slice);
                        //        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        //}
                        return true;
                }
        }
        return true;
}

bool dispatcher(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        Relation relation = {0}; 
        Relation_init(&relation, nodes); 

        return 
        preproc_include(&relation, slice, job) 
        && preproc_def(&relation, slice, job)
        && function_definition(&relation, slice, job)
        && function_declarator(&relation, slice, job)
        && parameter_list(&relation, slice, job)
        && parameter_declaration(&relation, slice, job)
        && enum_specifier(&relation, slice, job)
        && union_specifier(&relation, slice, job)
        && pointer_declarator(&relation, slice, job)
        && compound_statement(&relation, slice, job)
        && declaration(&relation, slice, job)
        && init_declarator(&relation, slice, job)
        && struct_specifier(&relation, slice, job)
        && field_declaration_list(&relation, slice, job)
        && field_declaration(&relation, slice, job)
        && initializer_list(&relation, slice, job)
        && initializer_pair(&relation, slice, job)
        && array_declarator(&relation, slice, job)
        && type_definition(nodes, slice, job)
        && enumerator_list(nodes, slice, job)
        && enumerator(nodes, slice, job)
        && expression_statement(nodes, slice, job)
        && call_expression(nodes, slice, job)
        && argument_list(nodes, slice, job)
        && for_statement(nodes, slice, job)
        && while_statement(nodes, slice, job)
        && do_statement(nodes, slice, job)
        && if_statement(&relation, slice, job)
        && parentesized_expression(nodes, slice, job)
        && binary_expression(nodes, slice, job)
        && else_clause(&relation, slice, job)

        && preproc_ifdef(&relation, slice, job) 
        && translation_unit(&relation, slice, job)
        && linkage_specification(&relation, slice, job)
        && declaration_list(&relation, slice, job)
        && roots(nodes, slice, job);
}

void ooo_ruler(
        Nodes *nodes,
        OJob *job
) {
        //static size_t count = 0;
        TSNode node = Nodes_at(nodes, 0);

        TSPoint start_point = ts_node_start_point(node);
        Slice slice = {
                .begin = job->cursor.idx,
                .end = OStrCursor_move_to_point(&job->cursor, &job->source, start_point)
        };

        if (dispatcher(nodes, slice, job)) {
                for (size_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        size_t num_of_childs = ts_node_child_count(node);
        //TSNode serial = node;
        for (size_t it = 0; it < num_of_childs; it++) {
               TSNode child = ts_node_child(node, it);
               Nodes_push(nodes,  child);
               ooo_ruler(nodes, job);
        }

        TSPoint end_point = ts_node_end_point(node);
        slice.begin = job->cursor.idx;
        slice.end = OStrCursor_move_to_point(&job->cursor, &job->source, end_point);
        for (size_t i = slice.begin; i < slice.end; i++) {
                OStr_append_chr(&job->sink, job->source.at[i]);
        }
}
