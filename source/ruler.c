#include "ruler.h"
#include "OJob.h"
#include "OStr.h"
#include "ooo_treesitter_symbol_ids.h"
#include "tree_navigator.h"
#include "Pars.h"
#include <string.h>

static bool preproc_include(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_string_literal) {
                /* #include "foo.h" */
                /*         ^        */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_system_lib_string) {
                /* #include <foo.h> */
                /*         ^        */
                OJob_space(job);
                return true;

        }
        return false;
}

static bool preproc_def(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* #define */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_identifier) {
                /* #define ALICE */
                /*        ^      */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_preproc_arg) {
                /* #define ALICE 50 */
                /*              ^   */
                OJob_LF_or_space(job, slice);
                return true;
        }
        return false;
}

static bool preproc_call(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* #pragma */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_preproc_arg) {
                /* #pragma once */
                /*        ^     */
                OJob_space(job);
                return true;
        }
        return false;
}

static bool preproc_function_def(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* #define */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_identifier) {
                /* #define ALICE(a, b) */
                /*        ^            */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_preproc_params) {
                /* #define ALICE(a, b) */
                /*             ^^      */
                return true;
        }
        if (me(node) == sym_preproc_arg) {
                /* #define ALICE(a, b) a + b */
                /*                    ^      */
                OJob_LF_or_space(job, slice);
                return true;
        }
        return false;
}

static bool preproc_params(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (node->child_idx == 1) {
                /* second child */
                if (is_single_line(node->parent)) {
                        /* #define ALICE(a  */
                        /*              ^^  */
                        return true;
                } else {
                        /* #define ALICE(\\\na */
                        /*               ^ ^   */
                        OJob_LF(job);
                        return true;
                }
        }
        if (me(node) == anon_sym_COMMA) {
                /* #define ALICE(A, B)  */
                /*               ^^     */
                return true;
        }
        if (me(node) == sym_identifier) {
                /* #define ALICE(A, B)  */
                /*                 ^    */
                OJob_LF_or_space(job, slice);
                return true;
        }
        if (me(node) == anon_sym_DOT_DOT_DOT) {
                /* #define ALICE(A, ...)  */
                /*                 ^    */
                OJob_LF_or_space(job, slice);
                return true;
        }
        if (is_last_child(node)) {
                if (is_single_line(node->parent)) {
                /* #define ALICE(A, B)  */
                /*                  ^^  */
                        return true;
                } else {
                        /* #define ALICE(\\\nA,\\\nB\\\n)  */
                        /*                           ^^    */
                        OJob_LF(job);
                        return true;
                }
        }
        if (me(node) == anon_sym_RPAREN) {
                /* #define ALICE(A, B)  */
                /*                  ^^  */
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}

static bool macro_type_specifier(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /*typeof */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* typeof(xxxx) */
        /*      ^^   ^ */
        return true;
}

#if 0
static bool preproc_ifdef(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* #ifndef or #ifdef */
                return true;
        }
        if (unknown(node)) {
                return false;
        }
        if (me(node) == sym_comment) {
                if (ts_node_start_point(Nodes_at(node, 0)).column == 0) {
                        OJob_1_or_2LF(job, slice);
                        return true;
                }
                return false;
        }
        if(me(node) == sym_identifier) {
                /* #ifndef FOO_H  */
                /*        ^       */
                OJob_space(job);
                return true; 
        }
        if (sym(Nodes_at(node, 1)) == sym_identifier 
        & sym(Nodes_at(node, 2)) == aux_sym_preproc_def_token1
        & sym(Nodes_at(node, 3)) == sym_preproc_def
        & sym(Nodes_at(node, 4)) == sym_identifier
        & sym(Nodes_at(node, 5)) == aux_sym_preproc_ifdef_token2
        & sym(Nodes_at(node, 6)) == sym_preproc_ifdef) {
                /* #ifndef FOO_H   */
                /* #define FOO_H\n\n */
                /*              ^ ^  */
                OJob_2LF(job);
                return true;
        }
        if (me(node) == aux_sym_preproc_if_token2 & is_last_child(node)) {
                if (sym(ts_node_child(node->parent, node->child_idx - 1)) == sym_linkage_specification) {
                        /* #ifdef __cplusplus */
                        /* } */
                        /* \n#endif */
                        /* ^         */
                        OJob_LF(job);
                        return true;
                }
                /* \n#endif */
                /* ^        */
                if (me_size(node) > 0) {
                        OJob_1_or_2LF(job, slice);
                        return true;
                }
                /* #ifdef __cplusplus #endif              */
                /*                      ^ node size == 0  */
                return true;
        }
        if (me(node) == sym_linkage_specification) {
                /* \nextern "C" { */
                /* ^              */
                OJob_LF(job);
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* enum AE { A, B, C }; */
                /*                   ^^ */
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}
#else
static bool preproc_ifdef(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* #ifndef or #ifdef */
                return true;
        }
        if (unknown(node)) {
                return false;
        }
        if (me(node) == sym_comment) {
                return OJob_max_2LF(job, slice);
        }
        if(me(node) == sym_identifier) {
                /* #ifndef FOO_H  */
                /*        ^       */
                OJob_space(job);
                return true; 
        }
        if (me(node) == aux_sym_preproc_if_token2 & is_last_child(node)) {
                if (me_size(node) > 0) {
                        /* \n#endif */
                        /* ^        */
                        OJob_1_or_2LF(job, slice);
                        return true;
                }
                /* #ifdef __cplusplus #endif              */
                /*                      ^ node size == 0  */
                return true;
        }
        if (me(node) == sym_linkage_specification) {
                /* \nextern "C" { */
                /* ^              */
                OJob_LF(job);
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* enum AE { A, B, C }; */
                /*                   ^^ */
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}
#endif

static bool preproc_if(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* #if */
                return true;
        }
        if (unknown(node)) {
                return false;
        }
        if (me(node) == sym_comment) {
                return OJob_max_2LF(job, slice);
        }
        if (is_before_child(node, anon_sym_LF)) {
                /* #if 1 */
                /*    ^  */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_LF) {
                return false;
        }
        if (me(node) == anon_sym_SEMI) {
                /* enum AE { A, B, C }; */
                /*                   ^^ */
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}

// #if defined(ALICE)
// do_something...
// #endif
// functionally equivalent to => #ifdef ALICE
static bool preproc_defined(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* defined */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* defined(ALICE) */
        return true;
}

static bool translation_unit(Relation const *node, Slice const slice, OJob *job) {
        /* root of an file */
        if (is_first_child(node)) {
                return true;
        }
        if (unknown(node)) {
                return false;
        }
        if (me(node) == sym_comment) {
                return OJob_max_2LF(job, slice);
        }
        if (me(node) == sym_compound_statement) {
                /* workaround for issue => (google test) TEST(hello, init) { */
                /* TEST(int a, int b) { */
                /*                   ^  */ // todo K&R option
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* enum AE { A, B, C }; */
                /*                   ^^ */
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}

static bool linkage_specification(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* extern */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_string_literal) {
                /* extern "C" */
                /*       ^    */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_declaration_list) {
                /* extern "C" { */
                /*           ^  */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* enum AE { A, B, C }; */
                /*                   ^^ */
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}

static bool declaration_list(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                return true;
        }
        if (unknown(node)) {
                return false;
        }
        if (me(node) == sym_comment) {
                return OJob_max_2LF(job, slice);
        }
        if (me(node) == sym_preproc_call & node->child_idx == 1) {
                /* extern "C" {        */
                /* \n#endif            */
                /* ^                   */
                OJob_LF(job);
                return true;
        }
        if (sym(Relation_track_node(node, 1)) == sym_preproc_directive
        & sym(Relation_track_node(node, 2)) == sym_preproc_call
        & sym(Relation_track_node(node, 3)) == anon_sym_LBRACE
        & sym(Relation_track_node(node, 4)) == sym_declaration_list) {
                /* #ifdef __cplusplus  */
                /* extern "C" {        */
                /* #endif\n\n          */
                /*         ^           */
                OJob_2LF(job);
                return true;
        }
        if (me(node) == anon_sym_RBRACE) {
                /* #ifdef __cplusplus */
                /* \n}                */
                /* ^                  */
                OJob_LF(job);
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* enum AE { A, B, C }; */
                /*                   ^^ */
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}

static bool function_definition(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* static void foo(int a, int b) */
                /* static                        */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_primitive_type) {
                /* static void foo(int a, int b) */
                /*       ^                       */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_storage_class_specifier) {
                /* extern static void foo(int a, int b) */
                /*       ^                              */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_function_declarator) {
                /* extern static void foo(int a, int b) */
                /*                   ^                  */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_compound_statement) {
                /* extern static void foo(int a, int b) { */
                /*                                     ^  */ // todo K&R option
                OJob_space(job);
                return true;
        }
        return false;
}

static bool function_declarator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* void foo(int a, int b) */
                /*      foo               */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_parameter_list) {
                /* void foo(int a, int b) */
                /*        ^^              */
                return true;
        }
        return false;
}

static bool parameter_list(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* void foo(int a, int b) */
                /*         (              */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (node->child_idx == 1) {
                /* second child */
                if (is_single_line(node->parent)) {
                        /* void foo(int a  */
                        /*         ^^      */
                        return true;
                } else {
                        /* void foo(\nint a  */
                        /*          ^        */
                        OJob_LF(job);
                        return true;
                }
        }
        if (is_last_child(node)) {
                if (is_single_line(node->parent)) {
                        /* void foo(int a, int b)  */
                        /*                     ^^  */
                        return true;
                } else {
                        /* void foo(\nint a,\nint b\n)  */
                        /*                          ^   */
                        OJob_LF(job);
                        return true;
                }
        }
        if (me(node) == anon_sym_COMMA) {
                /* void foo(int a, int b) */
                /*              ^^        */
                return true;
        }
        if (me(node) == sym_parameter_declaration) {
                /* void foo(int a, int b) */
                /*                ^       */
                OJob_LF_or_space(job, slice);
                return true;
        }
        return false;
}

static bool parameter_declaration(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* void foo(int a, int b) */
                /*          int             */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* void foo(const int a, int b) */
        /*               ^   ^      ^   */
        OJob_space(job);
        return true;
}

static bool enum_specifier(Relation const *node, Slice slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* enum alice */
                /* enum       */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* enum alice { */
        /*     ^     ^  */
        OJob_space(job);
        return true;
}

static bool union_specifier(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* union alice */
                /* union       */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* union alice { */
        /*      ^     ^  */
        OJob_space(job);
        return true;
}

static bool compound_statement(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* { */
                return true;
        }
        if (unknown(node)) {
                return false;
        }
        if (me(node) == sym_comment) {
                return OJob_max_2LF(job, slice);
        }
        if (is_single_line(node->parent)) {
                OJob_space(job);
                return true;
        }
        if (node->child_idx == 1) {
                /* second child */
                /* {\n */
                /*  ^  */
                OJob_LF(job);
                return true;
        }
        if (me(node) == anon_sym_RBRACE 
        && grand(node) == sym_case_statement 
        && sym(ts_node_prev_sibling(node->parent)) == anon_sym_COLON
        && sym(child(node, node->num_of_childs - 2)) == sym_break_statement) {
                /* case ABC: {\n    foo();    break; } */
                /*                                  ^  */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_RBRACE 
        && grand(node) == sym_case_statement 
        && sym(ts_node_prev_sibling(node->parent)) == anon_sym_COLON
        && sym(child(node, node->num_of_childs - 2)) == sym_return_statement) {
                /* case ABC: {\n    foo();\n    return 5; } */
                /*                                       ^  */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_RBRACE) {
                /* \n} */
                /*  ^  */
                OJob_LF(job);
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}

static bool pointer_declarator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* *alice */
                /* *      */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (node->child_idx == 1) {
                /* *const alice */
                /* *alice */
                /* ^^     */
                return true;
        }
        /* *const alice */
        /*       ^      */
        OJob_space(job);
        return true;
}

static bool pointer_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* &alice */
                /* ^      */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (node->child_idx == 1) {
                /* &const alice */
                /* &alice */
                /* ^^     */
                return true;
        }
        OJob_space(job);
        return true;
}

static bool sizeof_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* sizeof */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_parenthesized_expression) {
                /* sizeof( */
                /*      ^^ */
                return true;
        }
        return false;
}

static bool cast_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* ( */
                return true;
        }
        if (is_second_child(node)) {
                /* (char */
                /* ^^    */
                return true;
        }
        if (is_last_child(node)) {
                /* (char)X */
                /*      ^^ */
                return true;
        }
        if (me(node) == sym_comment) {
                return false;
        }
        if (me(node) == anon_sym_RPAREN) {
                /* char)  */
                /*    ^^  */
                return true;
        }
        OJob_space(job);
        return true;
}

static bool type_descriptor(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_abstract_pointer_declarator) {
                /* const char * */
                /*           ^  */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_abstract_array_declarator) {
                /* list[0]  */
                /*    ^^    */
                return true;
        }
        /* char const * */
        /*     ^       */
        /* const char * */
        /*      ^      */
        OJob_space(job);
        return true;
}

static bool abstract_pointer_declarator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* * */
                return true;
        }
        if (me(node) == sym_abstract_pointer_declarator) {
                /* ** */
                /* ^^ */
                return true;
        }
        return false;
}

static bool abstract_array_declarator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* [ */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* [1234] */
        /* ^^  ^^ */
        return true;
}

static bool declaration(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* bool alice */
                /* bool      */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_COMMA) {
                /* int32_t a, b = 1; */
                /*         ^^        */
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* bool alice; */
                /*          ^^ */
                return true;
        }
        /* static bool alice; */
        /*       ^    ^       */
        OJob_space(job);
        return true;
}

static bool init_declarator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_concatenated_string & !is_single_line(node->parent)) {
                /* char a[] =\n"hello world" */
                /*           ^               */
                OJob_LF(job);
                return true;
        }
        /* Record a = {1, 2, 3}; */
        /*         ^ ^           */
        OJob_space(job);
        return true;
}

static bool struct_specifier(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* struct a { */
        /*       ^ ^  */
        OJob_space(job);
        return true;
}

static bool field_declaration_list(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* struct big {\nint a;\nint b;\n} */
        /*              ^       ^       ^  */
        OJob_LF(job);
        return true;
}

static bool field_declaration(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_SEMI) {
                /* uint8_t big; */
                /*           ^^ */
                return true;
        }
        /* uint8_t big; */
        /*        ^     */
        OJob_space(job);
        return true;
}

static bool initializer_list(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_COMMA) {
                /* {.a = 1, */
                /*       ^^ */
                return true;
        }
        if (is_single_line(node->parent)) {
                if (node->child_idx == 1) {
                        /* is_second_child */
                        /* {.a = 1, */
                        /* ^^       */
                        return true;
                }
                if (me(node) == anon_sym_RBRACE) {
                        /* .a = 1} */
                        /*      ^^ */
                        return true;
                }
                /* {.a = 1, .b = 2}  */
                /*         ^         */
                OJob_space(job);
                return true;
        } else {
                if (node->child_idx == 1) {
                        /* {\n.a = 1, */
                        /* ^^         */
                        OJob_LF(job);
                        return true;
                }
                if (me(node) == anon_sym_RBRACE) {
                        /* .a = 1\n} */
                        /*        ^  */
                        OJob_LF(job);
                        return true;
                }
                /* {.a = 1, .b = 2\n}  */
                /* {.a = 1,\n.b = 2\n} */
                /*         ^           */
                OJob_LF_or_space(job, slice);
                return true;
        }
        return false;
}

static bool initializer_pair(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* .cut = LED_ON, */
        /*     ^ ^       */
        OJob_space(job);
        return true;
}

static bool array_declarator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* a[] */
                /* a   */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* a[4] */
        /* ^^^^ */
        return true;
}

static bool subscript_designator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* [ */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* [4] */
        /* ^^^ */
        return true;
}

static bool subscript_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* f[2] */
                /* f    */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* f[2] */
        /* ^^^^ */
        return true;
}

static bool type_definition(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* typedef enum { A, B, C } ABC; */
                /* typedef                       */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == alias_sym_type_identifier) {
                /* typedef enum { A, B, C } ABC; */
                /*                         ^     */
                OJob_space(job);
                return true;

        }
        if (me(node) == anon_sym_SEMI) {
                /* typedef struct { A, B, C } ABC; */
                /*                              ^^ */
                return true;
        }
        /* typedef enum { A, B, C } ABC; */
        /*        ^                ^     */
        OJob_space(job);
        return true;
}

static bool enumerator_list(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* enum { */
                /*      { */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_COMMA) {
                /* SECTOR_A, */
                /*        ^^ */
                return true;
        }
        if (is_single_line(node->parent)) {
                /* { SECTOR_A, SECTOR_B } */
                /*  ^         ^        ^  */
                OJob_space(job);
                return true;
        } else {
                /* {\nSECTOR_A,\nSECTOR_B\n} */
                /*  ^          ^         ^   */
                OJob_LF(job);
                return true;
        }
        return false;
}

static bool enumerator(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* FlashState_READY = 0 */
                /* FlashState_READY     */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* FlashState_READY = 0 */
        /*                 ^ ^  */
        OJob_space(job);
        return true;
}

static bool expression_statement(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* ach(); */
                /* acht() */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_SEMI) {
                /* ach(); */
                /*     ^^ */
                return true;
        }
        return false;
}

static bool return_statement(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* return */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_SEMI) {
                /* return X; */
                /*        ^^ */
                return true;
        }
        OJob_space(job);
        return true;
}

static bool assignment_expression(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* foo = false; */
                /* foo          */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_EQ) {
                if (is_single_line(node->parent)) {
                        /* foo = false; */
                        /*    ^         */
                        OJob_space(job);
                } else {
                        /* foo\n= false; */
                        /*    ^          */
                        OJob_LF_or_space(job, slice);
                }
                return true;
        }
        /* foo\n= false; */
        /*       ^       */
        OJob_space(job);
        return true;
}

static bool call_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* foo(int a) */
                /* foo        */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_argument_list) {
                /* foo(int a) */
                /*   ^^       */
                return true;
        }
        return false;
}

static bool update_expression(Relation const *node, Slice const slice, OJob *job) { 
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* counter++ */
                /* ++counter */
                /* counter-- */
                /* --counter */
                /* ^         */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        /* --counter */
        /*  ^^       */
        return true;
}

static bool conditional_expression(Relation const *node,Slice const slice,OJob *job) {
        if (is_first_child(node)) {
                /* (a > b) ? a : b; */
                /* ^                */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_QMARK) {
                uint32_t a = ts_node_start_point(child(node, node->child_idx)).row;
                uint32_t b = ts_node_start_point(child(node, node->child_idx + 1)).row;
                if (a != b) {
                        OJob_LF(job);
                        return true;
                }
                OJob_LF_or_space(job, slice);
                return true;
        }
        if (me(node) == anon_sym_COLON) {
                uint32_t a = ts_node_start_point(child(node, node->child_idx)).row;
                uint32_t b = ts_node_start_point(child(node, node->child_idx + 1)).row;
                if (a != b) {
                        OJob_LF(job);
                        return true;
                }
                OJob_LF_or_space(job, slice);
                return true;
        }
        OJob_space(job);
        return true;
}

static bool argument_list(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* function(alice */
                /*         (      */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (node->child_idx == 1) {
                /* second child */
                if(is_single_line(node->parent)) {
                        /* void foo(alice  */
                        /*         ^^      */
                        return true;
                } else {
                        /* void foo(\nalice  */
                        /*          ^        */
                        OJob_LF(job);
                        return true;
                }
        }
        if (me(node) == anon_sym_COMMA) {
                /* void foo(alice,  */
                /*              ^^  */
                return true;
        }
        if (me(node) == anon_sym_RPAREN) {
                if(is_single_line(node->parent)) {
                        /* void foo(alice, bob)  */
                        /*                   ^^  */
                        return true;
                } else {
                        /* void foo(\nalice, bob\n)  */
                        /*                       ^   */
                        OJob_LF(job);
                        return true;
                }
        }
        /* void foo(int a, int b)  */
        /* void foo(int a,\nint b) */
        /*                ^        */
        OJob_LF_or_space(job, slice);
        return true;
}

static bool for_statement(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* for */
                return true;
        }
        bool single_line_PAREN = false;
        int l = find_child(node, anon_sym_LPAREN);
        int r = find_child(node, anon_sym_RPAREN);
        if ( l >= 0 & r >= 0) {
                uint32_t A = ts_node_start_point(child(node, l)).row;
                uint32_t B = ts_node_end_point(child(node, r)).row;
                single_line_PAREN = ((A - B) == 0);
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == anon_sym_LPAREN) {
                /* for ( */
                /*    ^  */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* for (int i = 0; i < max; i++) */
                /*                       ^^      */
                return true;
        }
        if (is_middle_child(anon_sym_LPAREN, node, anon_sym_RPAREN)
        | me(node) == anon_sym_RPAREN) {
                if (single_line_PAREN) {
                        if (sym(Relation_track_node(node, 1)) == anon_sym_LPAREN) {
                                /* (int i = 0; i < max; i++) */
                                /* ^^                        */
                                return true;
                        }
                        if (me(node) == anon_sym_RPAREN) {
                                /* for (int i = 0; i < max; i++) */
                                /*                            ^^ */
                                return true;
                        }
                        /*  (\nint i; i < max; i++) */
                        /*           ^        ^     */
                        OJob_space(job);
                        return true;
                } else {
                        /*  (\nint i;\ni < max;\ni++\n) */
                        /*           ^         ^     ^  */
                        OJob_LF(job);
                        return true;
                }
        }
        if (me(node) == sym_compound_statement) {
                /* for (\nint i;\ni < max;\ni++\n) { */
                /*                                ^  */
                OJob_space(job);
                return true;
        }
        if (is_last_child(node)) {
                if (!is_single_line(node->parent)) {
                        /* for (int i; i < max; i++) if (a) {\n */
                        /*                          ^           */
                        OJob_LF(job);
                        return true;
                }
                /* for (int i; i < max; i++) doSomething();  */
                /* for (int i; i < max; i++)\ndoSomething(); */
                /*                          ^                */
                OJob_LF_or_space(job, slice);
                return true;
        }
        return false;
}

static bool while_statement(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* while */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_parenthesized_expression) {
                /* while (true) { */
                /*      ^         */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_compound_statement) {
                /* while (true) { */
                /*             ^  */
                OJob_space(job);
                return true;
        }
        if (is_last_child(node)) {
                if (!is_single_line(node->parent)) {
                        /* while (true) if (a) {\n */
                        /*             ^           */
                        OJob_LF(job);
                        return true;
                }
                /* while (true) doSomething();  */
                /* while (true)\ndoSomething(); */
                /*             ^                */
                OJob_LF_or_space(job, slice);
                return true;
        }
        return false;
}

static bool do_statement(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* do */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_compound_statement) {
                /* do { */
                /*   ^  */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_while) {
                /* } while (1); */
                /*  ^           */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_parenthesized_expression) {
                /* while (1); */
                /*      ^     */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_SEMI) {
                /* while (1); */
                /*         ^^ */
                return true;
        }
        return false;
}

static bool if_statement(Relation const *node, Slice slice, OJob *job) {
        if (is_first_child(node)) {
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_parenthesized_expression) {
                /* if (true) */
                /*   ^       */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_compound_statement) {
                /* if (true) { */
                /*          ^  */
                OJob_space(job);
                return true;
        } 
        if (is_middle_child(sym_parenthesized_expression, node, sym_else_clause)) {
                if (is_single_line(node->parent)) {
                        /* if (true) foo(); else */
                        /*          ^            */
                        OJob_space(job);
                        return true;
                } else {
                        /* if (true)\nfoo(); */
                        /*          ^        */
                        OJob_LF(job);
                        return true;
                }
        }
        if (me(node) == sym_else_clause) {
                if (is_after_child(sym_compound_statement, node)) {
                        /* if (true) { foo(); } else */
                        /*                     ^     */
                        OJob_space(job);
                        return true;
                } else { 
                        if (is_single_line(node->parent)) {
                                /* if (true) foo(); else */
                                /*                 ^     */
                                OJob_space(job);
                                return true;
                        } else {
                                /* if (true)\nfoo();\nelse */
                                /*                  ^      */
                                OJob_LF(job);
                                return true;
                        }
                }
        }
        if (is_last_child(node)) {
                if (!is_single_line(node->parent)) {
                        /* if (true)\nif (a) {\n */
                        /*          ^            */
                        OJob_LF(job);
                        return true;
                }
                /* if (true) doSomething();  */
                /* if (true)\ndoSomething(); */
                /*          ^                */
                OJob_LF_or_space(job, slice);
                return true;
        }
        return false;
}

static bool else_clause(Relation const *node, Slice slice, OJob *job)  {
        (void) slice;
        if (is_first_child(node)) {
                /* else */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_compound_statement | me(node) == sym_if_statement) {
                /* else {  */
                /* else if */
                /*     ^   */
                OJob_space(job);
                return true;
        } else if (is_single_line(node->grand)) {
                /* else foo();  */
                /*     ^        */
                OJob_space(job);
                return true;
        } else {
                /* else\nfoo();  */
                /*     ^         */
                OJob_LF(job);
                return true;
        }
        return false;
}


static bool parenthesized_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* (a == 0) */
                /* (        */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (sym(Relation_track_node(node, 1)) == anon_sym_LPAREN) {
                /* (a == 0) */
                /* ^^       */
                return true;
        }
        if (me(node) == anon_sym_RPAREN) {
                /* (a == 0) */
                /*       ^^ */
                return true;
        }
        return false;
}

static bool binary_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* a + b */
                /* a     */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (strcmp(ts_node_field_name_for_child(node->parent, node->child_idx),"operator") == 0) {
                uint32_t const left_row = ts_node_end_point(
                        ts_node_child_by_field_id(node->parent, field_left)
                ).row;
                uint32_t const operator_row = ts_node_start_point(
                        Relation_track_node(node, 0)
                ).row;
                uint32_t const right_row = ts_node_start_point(
                        ts_node_child_by_field_id(node->parent, field_right)
                ).row;
                if (left_row != operator_row | operator_row != right_row) {
                        /* a\n* b */
                        /*  ^    */
                        OJob_LF(job);
                        return true;
                }
                /* a * b */
                /*  ^   */
                OJob_space(job);
                return true;
        }
        /* a * b */
        /*    ^  */
        OJob_space(job);
        return true;
}

static bool switch_statement(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        if (is_first_child(node)) {
                /* switch */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_parenthesized_expression) {
                /* switch (true) { */
                /*       ^         */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_compound_statement) {
                /* switch (true) { */
                /*              ^  */
                OJob_space(job);
                return true;
        }
        return false;
}

static bool case_statement(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* case */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (is_before_child(node, anon_sym_COLON)) {
                /* case ABC: */
                /*     ^     */
                OJob_space(job);
                return true;
        }
        if (me(node) == anon_sym_COLON) {
                /* case ABC: */
                /*        ^^ */
                return true;
        }
        if (me(node) == sym_compound_statement 
        && sym(Relation_track_node(node, 1)) == anon_sym_COLON
        && is_last_child(node)) {
                /* case ABC: { .... } case */
                /*          ^              */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_compound_statement 
        && sym(Relation_track_node(node, 1)) == anon_sym_COLON
        && (node->child_idx + 2 == node->num_of_childs)
        && sym(child(node, node->num_of_childs - 1)) == sym_break_statement) {
                /* case ABC: { .... } break; case */
                /*          ^                     */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_compound_statement 
        && sym(Relation_track_node(node, 1)) == anon_sym_COLON
        && (node->child_idx + 2 == node->num_of_childs)
        && sym(child(node, node->num_of_childs - 1)) == sym_return_statement) {
                /* case ABC: { .... } return; case */
                /*          ^                     */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_break_statement
        && is_last_child(node)
        && sym(child(node, node->child_idx - 1)) == sym_compound_statement
        && sym(child(node, node->child_idx - 2)) == anon_sym_COLON) {
                /* case ABC: { .... } break; case */
                /*                   ^            */
                OJob_space(job);
                return true;
        }
        if (me(node) == sym_return_statement
        && is_last_child(node)
        && sym(child(node, node->child_idx - 1)) == sym_compound_statement
        && sym(child(node, node->child_idx - 2)) == anon_sym_COLON) {
                /* case ABC: { .... } return; case */
                /*                   ^            */
                OJob_space(job);
                return true;
        }
        OJob_1_or_2LF(job, slice);
        return true;
}

static bool unary_expression(Relation const *node, Slice const slice, OJob *job) {
        (void) slice;
        (void) job;
        if (is_first_child(node)) {
                /* operator !x ++x --x ect. */
                return true;
        }
        return true;
}

static bool concatenated_string(Relation const *node, Slice const slice, OJob *job) {
        if (is_first_child(node)) {
                /* " */
                return true;
        }
        if (me(node) == sym_comment | unknown(node)) {
                return false;
        }
        if (me(node) == sym_string_literal) {
                OJob_LF_or_space(job, slice);
                return true;
        }
        return true;
}
// static bool roots(Relation const *node, Slice const slice, OJob *job) {
//         if (parent(node) != sym_translation_unit 
//         & parent(node) != sym_preproc_ifdef
//         & parent(node) != sym_linkage_specification
//         & parent(node) != sym_declaration_list) {
//                 return false;
//         }
//         if (is_first_child(node)) {
//                 return true;
//         }
//         if (unknown(node)) {
//                 return false;
//         }
//         if (me(node) == sym_comment) {
//                 if (ts_node_start_point(Nodes_at(node, 0)).column == 0) {
//                         OJob_1_or_2LF(job, slice);
//                         return true;
//                 }
//                 return false;
//         }
//         OJob_1_or_2LF(job, slice);
//         return true;
// }

bool dispatcher(
        Relation const *relation,
        Slice slice,
        OJob *job
) {
        switch(parent(relation)) {

        case sym_preproc_include: 
                return preproc_include(relation, slice, job); 
        case sym_preproc_def: 
                return preproc_def(relation, slice, job);
        case sym_preproc_call: 
                return preproc_call(relation, slice, job);
        case sym_preproc_function_def: 
                return preproc_function_def(relation, slice, job);

        case sym_preproc_ifdef: 
                return preproc_ifdef(relation, slice, job); 
        case sym_preproc_defined: 
                return preproc_defined(relation, slice, job);
        case sym_preproc_if: 
                return preproc_if(relation, slice, job); 
        case sym_preproc_params:
                return preproc_params(relation, slice, job);
        case sym_macro_type_specifier:
                return macro_type_specifier(relation, slice, job);

        case sym_function_definition: 
                return function_definition(relation, slice, job);
        case sym_function_declarator: 
                return function_declarator(relation, slice, job);
        case sym_parameter_list: 
                return parameter_list(relation, slice, job);
        case sym_parameter_declaration: 
                return parameter_declaration(relation, slice, job);
        case sym_enum_specifier: 
                return enum_specifier(relation, slice, job);
        case sym_union_specifier: 
                return union_specifier(relation, slice, job);
        case sym_pointer_declarator: 
                return pointer_declarator(relation, slice, job);
        case sym_pointer_expression: 
                return pointer_expression(relation, slice, job);
        case sym_sizeof_expression: 
                return sizeof_expression(relation, slice, job);
        case sym_cast_expression: 
                return cast_expression(relation, slice, job);
        case sym_type_descriptor: 
                return type_descriptor(relation, slice, job);
        case sym_abstract_pointer_declarator: 
                return abstract_pointer_declarator(relation, slice, job);
        case sym_abstract_array_declarator: 
                return abstract_array_declarator(relation, slice, job);
        case sym_compound_statement: 
                return compound_statement(relation, slice, job);
        case sym_declaration: 
                return declaration(relation, slice, job);
        case sym_init_declarator: 
                return init_declarator(relation, slice, job);
        case sym_struct_specifier: 
                return struct_specifier(relation, slice, job);
        case sym_field_declaration_list: 
                return field_declaration_list(relation, slice, job);
        case sym_field_declaration: 
                return field_declaration(relation, slice, job);
        case sym_initializer_list: 
                return initializer_list(relation, slice, job);
        case sym_initializer_pair: 
                return initializer_pair(relation, slice, job);
        case sym_array_declarator: 
                return array_declarator(relation, slice, job);
        case sym_subscript_designator: 
                return subscript_designator(relation, slice, job);
        case sym_subscript_expression:
                return subscript_expression(relation, slice, job);
        case sym_type_definition: 
                return type_definition(relation, slice, job);
        case sym_enumerator_list: 
                return enumerator_list(relation, slice, job);
        case sym_enumerator: 
                return enumerator(relation, slice, job);
        case sym_expression_statement: 
                return expression_statement(relation, slice, job);
        case sym_return_statement: 
                return return_statement(relation, slice, job);
        case sym_assignment_expression: 
                return assignment_expression(relation, slice, job);
        case sym_call_expression: 
                return call_expression(relation, slice, job);
        case sym_update_expression:
                return update_expression(relation, slice, job);
        case sym_conditional_expression:
                return conditional_expression(relation, slice, job);
        case sym_argument_list: 
                return argument_list(relation, slice, job);
        case sym_for_statement: 
                return for_statement(relation, slice, job);
        case sym_while_statement: 
                return while_statement(relation, slice, job);
        case sym_do_statement: 
                return do_statement(relation, slice, job);
        case sym_if_statement: 
                return if_statement(relation, slice, job);
        case sym_parenthesized_expression: 
                return parenthesized_expression(relation, slice, job);
        case sym_binary_expression: 
                return binary_expression(relation, slice, job);
        case sym_else_clause: 
                return else_clause(relation, slice, job);
        case sym_switch_statement: 
                return switch_statement(relation, slice, job);
        case sym_case_statement: 
                return case_statement(relation, slice, job);
        case sym_concatenated_string:
                return concatenated_string(relation, slice, job);

        case sym_translation_unit: 
                return translation_unit(relation, slice, job);
        case sym_linkage_specification: 
                return linkage_specification(relation, slice, job);
        case sym_declaration_list: 
                return declaration_list(relation, slice, job);
        case sym_unary_expression:
                return unary_expression(relation, slice, job);
        default:
                return false;
        }
}

#include <stdio.h>
void ooo_ruler(
        Relation *relation,
        OJob *job
) {
        TSNode node = Relation_track_node(relation, 0);
        TSSymbol symbol = ts_node_symbol(node);

        Slice slice = {
                .begin = job->idx,
                .end = job->idx = (ts_node_start_byte(node) + job->offset)
        };

        if (!dispatcher(relation, slice, job)) {
                for (uint32_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        if (symbol == sym_preproc_def | symbol == sym_preproc_function_def | symbol == sym_preproc_call) {
                OJob_set_lineContinuation(job);
        }

        TSNode const grand = relation->grand;
        TSNode const parent = relation->parent;
        uint32_t num_of_childs = ts_node_child_count(node);
        for (uint32_t it = 0; it < num_of_childs; it++) {
               TSNode child = ts_node_child(node, it);
               Relation_parent_push(relation, node, num_of_childs, parent, grand);
               Relation_track(relation, child, it);
               ooo_ruler(relation, job);
        }

        slice.begin = job->idx;
        slice.end = job->idx = (ts_node_end_byte(node) + job->offset);
        if (slice.end > 0 && job->source.at[slice.end - 1] == '\n') {
                /* \n is last member byte in the node   */
                /* that caused and unexpected line feed */
                /* therefore decrement job->idx         */
                /* problematic nodes are for example:   */
                /*     anon_sym_LF                      */
                /*     sym_preproc_def                  */
                job->idx--;
                slice.end = job->idx;
        } 

        if (symbol == sym_preproc_arg) {
                uint32_t const o = job->offset;
                job->idx = job->offset = slice.begin;
                RootNode_t root = Pars_getTree(&job->source.at[slice.begin], slice.end - slice.begin);
                Relation_parent_push(relation, node, num_of_childs, parent, grand);
                Relation_track(relation, root.node, 0);
                ooo_ruler(relation, job);
                Pars_freeTree(root);
                job->offset = o;
        } else {
                for (uint32_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        if (symbol == sym_preproc_def | symbol == sym_preproc_function_def | symbol == sym_preproc_call) {
                OJob_reset_lineContinuation(job);
        }
}
