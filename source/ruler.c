#include "ruler.h"
#include "OStr.h"
#include "tree_navigator.h"
#include <stdio.h>

#if 0
bool curly_brace_compound_statement(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol prev_parent_sibling = ooo(sibling(-1, super(1, node)));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (me == sym_compound_statement & parent == sym_function_definition) {
                /* Curly brace for function K&R-Rule */
                /* function(bool example)\n{         */
                /* '\n{'                             */
                if (is_single_line(ts_node_parent(node))) {
                        OStr_append_chr(&job->sink, '\n'); 
                } else {
                        OStr_append_chr(&job->sink, ' '); 
                }
                return false;
        }
        if (me == sym_compound_statement & parent != sym_function_definition 
        & (parent == sym_compound_statement
        | parent == sym_case_statement & prev_sibling !=  anon_sym_COLON)) {
                /* Curly brace not function  */
                /* nested code block compound statement  */
                /* '\n{'                                 */
                OStr_append_chr(&job->sink, '\n'); 
                return false;
        }
        if (me == sym_compound_statement
        & parent != sym_function_definition 
        & parent != sym_compound_statement
        & prev_parent_sibling !=  anon_sym_COLON) {
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (parent == sym_compound_statement 
        & prev_sibling == anon_sym_LBRACE) {
                /* new line after { begin code block compound statement */
                /* '{ ' or '{\n'                                        */
                if (is_single_line(ts_node_parent(node))) {
                        OStr_append_chr(&job->sink, ' ');
                } else {
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        if (me == anon_sym_RBRACE & parent == sym_compound_statement
        & !(prev_sibling == sym_break_statement & grand == sym_case_statement)) {
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
        if (me == anon_sym_RBRACE & parent == sym_compound_statement 
        & prev_sibling == sym_break_statement & grand == sym_case_statement) {
                /* end of an code block compound statement  */
                /* 'break; }'                               */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_else_clause
        & prev_sibling != sym_function_definition 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_compound_statement) {
                /* end of an code block compound statement  */
                /* '} else'                                 */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_break_statement 
        & parent == sym_case_statement
        & serial == anon_sym_RBRACE) {
                /* end of an code block compound statement  */
                /* '} break;'                                */
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
                OStr_append_number_of_chr(&job->sink, n, '\n');
                return false;
        }
        if (prev_sibling == sym_function_definition 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_compound_statement) {
                /* end of an function code block compound statement  */
                /* closed curly brace on a new line         */
                /* '}\n\n'                                  */
                OStr_append_number_of_chr(&job->sink, 2, '\n');
                return false;
        }
        return true;
}

bool curly_brace_field_declaration_list(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

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
                // OStr_append_chr(&job->sink, ' ');
                //return false;
        }
        return true;
}

bool curly_brace_enumerator_list(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (me == sym_enumerator_list) {
                /* enum Foo {  */
                /*         ^   */
                OStr_append_chr(&job->sink, ' '); 
                return false;
        }
        if (parent == sym_enumerator_list & prev_sibling == anon_sym_LBRACE) {
                if (is_single_line(ts_node_parent(node))) {
                        /* enum foo = {1, 2, 3} */
                        /*            ^^        */
                        //OStr_append_chr(&job->sink, ' ');
                } else {
                        /* enum foo = {\n  */
                        /*             ^   */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        if (me == anon_sym_RBRACE & parent == sym_enumerator_list) {
                if (is_single_line(ts_node_parent(node))) {
                        /* enum foo = {1, 2, 3} */
                        /*                   ^^ */
                        //OStr_append_chr(&job->sink, ' ');
                } else {
                        /* enum foo {    */
                        /*     A, B, C\n */
                        /* }          ^  */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        if (parent == sym_declaration 
        & serial == anon_sym_RBRACE 
        & serial_parent == sym_enumerator_list) {
                /* 'enum Foo { ... }' */
                /* '} foo'              */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        return true;
}

bool curly_brace_initializer_list(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (me == sym_initializer_list) {
                /* int foo[] = {1, 2, 3} */
                /*            ^          */
                OStr_append_chr(&job->sink, ' '); 
                return false;
        }
        if (parent ==  sym_initializer_list & prev_sibling == anon_sym_LBRACE) {
                if (is_single_line(ts_node_parent(node))) {
                        /* int foo[] = {1, 2, 3} */
                        /*             ^^ */
                        // OStr_append_chr(&job->sink, ' ');
                } else {
                        /* int foo[] = {\n  */
                        /*              ^   */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        if (me == anon_sym_RBRACE & parent == sym_initializer_list) {
                if (is_single_line(ts_node_parent(node))) {
                        /* int foo[] = {1, 2, 3} */
                        /*                    ^^ */
                        // OStr_append_chr(&job->sink, ' ');
                } else {
                        /* int foo[] = {\n  */
                        /*     1, 2, 3\n    */
                        /* }          ^     */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        return true;
}

bool parenthesize_parameter_list(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (me == sym_parameter_list) {
                /* void Foo(  */
                /*        ^^  */
                return false;
        }
        if (parent == sym_parameter_list & prev_sibling == anon_sym_LPAREN) {
                if (is_single_line(ts_node_parent(node))) {
                        /* void Foo(int  */
                        /*         ^^    */
                        //OStr_append_chr(&job->sink, ' ');
                } else {
                        /* void Foo(\n  */
                        /*          ^   */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        if (me == anon_sym_RPAREN & parent == sym_parameter_list) {
                if (is_single_line(ts_node_parent(node))) {
                        /* void Foo(int alice) */
                        /*                  ^^ */
                        //OStr_append_chr(&job->sink, ' ');
                } else {
                        /* void Foo(\n         */
                        /*         int alice\n */
                        /* )                ^  */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        return true;
}

bool parenthesize_argument_list(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (me == sym_argument_list) {
                /* foo(  */
                /*   ^^  */
                return false;
        }
        if (parent == sym_argument_list & prev_sibling == anon_sym_LPAREN) {
                if (is_single_line(ts_node_parent(node))) {
                        /* foo(alice */
                        /*    ^^     */
                } else {
                        /* foo(\n  */
                        /*     ^   */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        if (me == anon_sym_RPAREN & parent == sym_argument_list) {
                if (is_single_line(ts_node_parent(node))) {
                        /* foo(bob, alice) */
                        /*              ^^ */
                } else {
                        /* foo(\n      */
                        /*     alice\n */
                        /* )        ^  */
                        OStr_append_chr(&job->sink, '\n');
                }
                return false;
        }
        return true;
}

bool append_nothing(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (me == sym_init_declarator) {
                /* defer append space see also append_space me == sym_identifier */
                /*  static intfoo = 0;*/
                /*           ^^         */
                return false;
        }
        if (me == sym_function_declarator) {
                /* defer append space see also append_space me == sym_identifier */
                /* voidfoo( */
                /*    ^^    */
                return false;
        }
        if (me == sym_field_expression) {
                /* defer append space see also append_space me == sym_identifier */
                /* =data->member */
                /* ^^            */
                return false;
        }
        if (me == anon_sym_COMMA) {
                /*  int foo, int boo, int zoo */
                /*        ^^       ^^         */
                return false;
        }
        if (me == anon_sym_SEMI) {
                /*  int foo; int boo; int zoo */
                /*        ^^       ^^         */
                return false;
        }
        if (me == sym_identifier & serial == anon_sym_STAR) {
                /*  int *foo; */
                /*      ^^    */
                return false;
        }
        if (me == sym_number_literal & serial == anon_sym_LBRACK) {
                /*  foo[5] */
                /*     ^^  */
                return false;
        }
        if (me == sym_identifier & serial == anon_sym_LBRACK) {
                /*  foo[yellow] */
                /*     ^^       */
                return false;
        }
        if (me == sym_identifier & serial == sym_binary_expression) {
                /* defer append space see also me == sym_identifier */
                /* (foo; */
                /* ^^    */
                return false;
        }
        if (me == sym_identifier & parent == sym_subscript_expression & grand == sym_binary_expression) {
                /* defer append space see also me == sym_identifier */
                /* (foo[1]; */
                /* ^^       */
                return false;
        }
        return true;
}

bool expression_statement(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));

        if (me == sym_expression_statement) {
                if (ts_node_start_point(node).column == 0) {
                        /*hello(); callFoo(int f)  */
                        /*        ^                */
                        OStr_append_chr(&job->sink, '\n');
                        return false;
                }
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        return true;
}

bool append_space(
        Nodes *nodes,
        Slice slice,
        OJob *job
) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
// aux_sym_preproc_include_token1,
// sym_preproc_include,
// sym_string_literal,
        //if (me == sym_string_literal && parent == sym_preproc_include) {
        if (me == sym_string_literal & serial == aux_sym_preproc_include_token1) {
                /*  #include "foo.h" */
                /*          ^        */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        //if (me == sym_system_lib_string && parent == sym_preproc_include) {
        if (me == sym_system_lib_string &  serial == aux_sym_preproc_include_token1) {
                /*  #include <foo.h> */
                /*          ^        */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        //if (me == sym_preproc_arg  && prev_sibling == sym_identifier && parent == sym_preproc_def) {
        if (me == sym_preproc_arg  & serial == sym_identifier) {
                /*  #define FOO 42 */
                /*             ^   */
                OStr_append_chr(&job->sink, ' ');
               return false;
        }

        if (me == sym_pointer_declarator) {
                /*  int *foo; */
                /*     ^     */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_identifier) {
                switch(parent) {
                case sym_call_expression:
                        if (ts_node_start_point(node).column > 0) {
                                /*hello(); callFoo(int f)  */
                                /*        ^                */
                                OStr_append_chr(&job->sink, ' ');
                                return false;
                        }
                        break;
                case sym_assignment_expression:
                        if (ts_node_start_point(node).column > 0) {
                                /*hello(); callFoo(int f)  */
                                /*        ^                */
                                OStr_append_chr(&job->sink, ' ');
                                return false;
                        }
                        break;
                case sym_subscript_expression:
                        if (ts_node_start_point(node).column > 0) {
                                /* + s[1])  */
                                /*  ^       */
                                OStr_append_chr(&job->sink, ' ');
                                return false;
                        }
                        break;
                default:
                        /*  int foo; */
                        /*     ^     */
                        OStr_append_chr(&job->sink, ' ');
                        return false;
                }
        }
        if (me == anon_sym_EQ) {
                /*  int foo = 42; */
                /*         ^      */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_number_literal) {
                /*  int foo = 42; */
                /*           ^    */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        return true;
}
#endif
///////////////////////////////////////////////////////////////////////////

static bool preproc_include(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_preproc_include) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        /* #include "foo.h" */
        /*         ^        */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool preproc_def(Nodes *nodes, Slice slice, OJob *job)  {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_preproc_def) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        OStr_append_chr(&job->sink, ' ');
        return false;
}


static bool preproc_ifdef(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_preproc_ifdef) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        if(me == sym_identifier & first_sibling(last_node)) {
                /* #ifndef FOO_H  */
                /*        ^       */
                OStr_append_chr(&job->sink, ' ');
                return false; 
        }
        if (ooo(Nodes_at(nodes, 1)) == sym_identifier 
        & ooo(Nodes_at(nodes, 2)) == aux_sym_preproc_def_token1
        & ooo(Nodes_at(nodes, 3)) == sym_preproc_def
        & ooo(Nodes_at(nodes, 4)) == sym_identifier
        & ooo(Nodes_at(nodes, 5)) == aux_sym_preproc_ifdef_token2
        & ooo(Nodes_at(nodes, 6)) == sym_preproc_ifdef) {
                /* #ifndef FOO_H   */
                /* #define FOO_H\n\n */
                /*              ^ ^  */
                size_t const num_of_LF = OStr_need_2LF(&job->source, slice);
                //printf("ssss %zu\n", num_of_LF);
                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                return false;
        }
        if (me == aux_sym_preproc_if_token2 & last_sibling(node)) {
                /* \n#endif */
                /* ^        */
                if (ts_node_start_byte(node) != ts_node_end_byte(node)) {
                        size_t const num_of_LF = OStr_need_1_or_2LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
                /* #ifdef __cplusplus #endif              */
                /*                      ^ node size == 0  */
        }
        return true;
}

static bool translation_unit(Nodes *nodes, Slice slice, OJob *job) {
        /* root of an file */
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_translation_unit) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        return true;
}

static bool linkage_specification(Nodes *nodes, Slice slice, OJob *job) {
        /* extern "C" */
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_linkage_specification) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        if (me == sym_string_literal & second_sibling(node)) {
                /* extern "C" */
                /*       ^    */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_declaration_list) {
                /* extern "C" { */
                /*           ^  */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        return true;
}

static bool declaration_list(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_declaration_list) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        if (ooo(Nodes_at(nodes, 1)) == sym_preproc_directive
        & ooo(Nodes_at(nodes, 2)) == sym_preproc_call
        & ooo(Nodes_at(nodes, 3)) == anon_sym_LBRACE
        & ooo(Nodes_at(nodes, 4)) == sym_declaration_list) {
                /* #ifdef __cplusplus  */
                /* extern "C" {        */
                /* #endif\n\n          */
                /*         ^           */
                size_t const num_of_LF = OStr_need_2LF(&job->source, slice);
                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n'); 
                return false;
        }
        if (me == anon_sym_RBRACE & last_sibling(node)) {
                /* #ifdef __cplusplus */
                /* \n}                */
                /* ^                  */
                size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n'); 
                return false;
        }
        return true;
}

static bool function_definition(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_function_definition) {
                return true;
        }
        if (first_sibling(node)) {
                /* static void foo(int a, int b) */
                /* static                        */

                return false;
        }
        if (me == sym_primitive_type) {
                /* static void foo(int a, int b) */
                /*       ^^                      */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_storage_class_specifier) {
                /* extern static void foo(int a, int b) */
                /*       ^^                      */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_function_declarator) {
                /* extern static void foo(int a, int b) */
                /*                   ^                  */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == sym_compound_statement) {
                /* extern static void foo(int a, int b) { */
                /*                                     ^  */ // todo K&R option
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        return true;
}

static bool function_declarator(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_function_declarator) {
                return true;
        }
        if (first_sibling(node)) {
                /* void foo(int a, int b) */
                /*      foo               */
                return false;
        }
        if (me == sym_parameter_list) {
                /* void foo(int a, int b) */
                /*        ^^              */
                return false;
        }
        return true;
}

static bool parameter_list(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_parameter_list) {
                return true;
        }
        if (first_sibling(node)) {
                /* void foo(int a, int b) */
                /*         (              */
                return false;
        }
        if (second_sibling(node)) {
                if(is_single_line(ts_node_parent(node))) {
                        /* void foo(int a  */
                        /*         ^^      */
                        return false;
                } else {
                        /* void foo(\nint a  */
                        /*          ^        */
                        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
        }
        if (last_sibling(node)) {
                if(is_single_line(ts_node_parent(node))) {
                        /* void foo(int a, int b)  */
                        /*                     ^^  */
                        return false;
                } else {
                        /* void foo(\nint a,\nint b\n)  */
                        /*                          ^   */
                        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
        }
        if (me == anon_sym_COMMA) {
                /* void foo(int a, int b) */
                /*              ^^        */
                return false;
        }
        if (me == sym_parameter_declaration) {
                /* void foo(int a, int b) */
                /*                ^       */
                char const chr = OStr_need_1LF_or_1Space(&job->source, slice);
                OStr_append_chr(&job->sink, chr);
                return false;
        }
        return true;
}

static bool parameter_declaration(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSNode last_node = Nodes_at(nodes, 1); 

        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol last = ooo(last_node);
        TSSymbol serial_parent = ooo(super(1, last_node));
        if (parent != sym_parameter_declaration) {
                return true;
        }
        if (first_sibling(node)) {
                /* void foo(int a, int b) */
                /*          int             */
                return false;
        }
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool enum_specifier(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_enum_specifier) {
                return true;
        }
        if (first_sibling(node)) {
                /* enum alice */
                /* enum       */
                return false;
        }
        /* enum alice { */
        /*     ^     ^  */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool union_specifier(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_union_specifier) {
                return true;
        }
        if (first_sibling(node)) {
                /* union alice */
                /* union       */
                return false;
        }
        /* union alice */
        /*      ^      */
        OStr_append_chr(&job->sink, ' ');
        return false;
}


static bool compound_statement(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_compound_statement) {
                return true;
        }
        if (first_sibling(node)) {
                /* { */
                /* { */
                return false;
        }
        if (second_sibling(node)) {
                /* {\n */
                /*  ^  */
                size_t num_of_LF = OStr_need_1LF(&job->source, slice);
                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                return false;
        }
        if (last_sibling(node)) {
                /* \n} */
                /*  ^  */
                size_t num_of_LF = OStr_need_1LF(&job->source, slice);
                OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                return false;
        }
        return true;
}

static bool pointer_declarator(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_pointer_declarator) {
                return true;
        }
        if (first_sibling(node)) {
                /* *alice */
                /* *      */
                return false;
        }
        /* *alice */
        /* ^^     */
        return false;
}

static bool declaration(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_declaration) {
                return true;
        }
        if (first_sibling(node)) {
                /* bool alice */
                /* bool      */
                return false;
        }
        if (me == anon_sym_COMMA) {
                /* int32_t a, b = 1; */
                /*         ^^        */
                return false;
        }
        if (me == anon_sym_SEMI & last_sibling(node)) {
                /* bool alice; */
                /*          ^^ */
                return false;
        }
        /* bool alice */
        /*     ^      */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool init_declarator(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_init_declarator) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        /* Record a = {1, 2, 3}; */
        /*         ^ ^           */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool struct_specifier(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes,0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_struct_specifier) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        /* struct a { */
        /*       ^ ^  */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool field_declaration_list(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_field_declaration_list) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        /* struct big {\nint a;\nint b;\n} */
        /*              ^       ^       ^  */
        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
        return false;
}

static bool field_declaration(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_field_declaration) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        if (last_sibling(node)) {
                /* uint8_t big; */
                /*           ^^ */
                return false;
        }
        /* uint8_t big; */
        /*        ^     */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool initializer_list(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_initializer_list) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        if (me == anon_sym_COMMA) {
                /* {.a = 1, */
                /*       ^^ */
                return false;
        }
        if (is_single_line(super(1, node))) {
                if (second_sibling(node)) {
                        /* {.a = 1, */
                        /* ^^       */
                        return false;
                }
                if (last_sibling(node)) {
                        /* .a = 1} */
                        /*      ^^ */
                        return false;
                }
                char const ch = OStr_need_1LF_or_1Space(&job->source, slice);
                OStr_append_chr(&job->sink, ch);
                return false;
        } else {
                if (second_sibling(node)) {
                        /* {\n.a = 1, */
                        /* ^^         */
                        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
                if (me == sym_comment & !is_single_line(node)) {
                        /* {.a = 1,\n/* comment\n */
                        /*         ^              */
                        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
                if (last_sibling(node)) {
                        /* .a = 1\n} */
                        /*        ^  */
                        size_t const num_of_LF = OStr_need_1LF(&job->source, slice);
                        OStr_append_number_of_chr(&job->sink, num_of_LF, '\n');
                        return false;
                }
                char ch = OStr_need_1LF_or_1Space(&job->source, slice);
                OStr_append_chr(&job->sink, ch);
                return false;
        }
        return true;
}

static bool initializer_pair(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        if (parent != sym_initializer_pair) {
                return true;
        }
        if (first_sibling(node)) {
                return false;
        }
        /* .cut = LED_ON, */
        /*     ^ ^       */
        OStr_append_chr(&job->sink, ' ');
        return false;
}

static bool array_declarator(Nodes *nodes, Slice slice, OJob *job) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol me = ooo(node);
        if (parent != sym_array_declarator) {
                return true;
        }
        if (first_sibling(node)) {
                /* a[] */
                /* a   */
                return false;
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
        if (second_sibling(node)) {
                /* for int i; i < max; i++)         */
                /* for comment(int i; i < max; i++) */
                /*    ^                             */
                OStr_append_chr(&job->sink, ' ');
                return false;
        }
        if (me == anon_sym_LPAREN) {
                if (ooo(Nodes_at(nodes, 1)) ==  anon_sym_for) {
                        /* for ( */
                        /*    ^  */
                        OStr_append_chr(&job->sink, ' ');
                        return false;
                } else {
                        /* for comment(int i; i < max; i++) */
                        /*           ^^                     */
                        return false;
                }
        }
        if (me == anon_sym_SEMI) {
                /* for (int i; i < max; i++) */
                /*                   ^^      */
                return false;
        }
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
                if (me != sym_compound_statement & ooo(Nodes_at(nodes, 1)) == anon_sym_RPAREN) {
                        /* for (int i; i < max; i++) doSomething();  */
                        /* for (int i; i < max; i++)\ndoSomething(); */
                        /*                          ^                */
                        char const ch = OStr_need_1LF_or_1Space(&job->source, slice);
                        OStr_append_chr(&job->sink, ch);
                        return false;
                }
                if (me == sym_compound_statement) {
                        /* for (int i; i < max; i++) { */
                        /*                          ^  */
                        OStr_append_chr(&job->sink, ' ');
                        return false;
                }
                char const ch = OStr_need_1LF_or_1Space(&job->source, slice);
                OStr_append_chr(&job->sink, ch);
                return false;
        } else {
                /* multi line */
                if (ooo(Nodes_at(nodes, 1)) == anon_sym_RPAREN) {
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
        return 
        preproc_include(nodes, slice, job) 
        && preproc_def(nodes, slice, job)
        && function_definition(nodes, slice, job)
        && function_declarator(nodes, slice, job)
        && parameter_list(nodes, slice, job)
        && parameter_declaration(nodes, slice, job)
        && enum_specifier(nodes, slice, job)
        && union_specifier(nodes, slice, job)
        && pointer_declarator(nodes, slice, job)
        && compound_statement(nodes, slice, job)
        && declaration(nodes, slice, job)
        && init_declarator(nodes, slice, job)
        && struct_specifier(nodes, slice, job)
        && field_declaration_list(nodes, slice, job)
        && field_declaration(nodes, slice, job)
        && initializer_list(nodes, slice, job)
        && initializer_pair(nodes, slice, job)
        && array_declarator(nodes, slice, job)
        && type_definition(nodes, slice, job)
        && enumerator_list(nodes, slice, job)
        && enumerator(nodes, slice, job)
        && expression_statement(nodes, slice, job)
        && call_expression(nodes, slice, job)
        && argument_list(nodes, slice, job)
        && for_statement(nodes, slice, job)

        && preproc_ifdef(nodes, slice, job) 
        && translation_unit(nodes, slice, job)
        && linkage_specification(nodes, slice, job)
        && declaration_list(nodes, slice, job)
        && roots(nodes, slice, job);
#if 0
        && append_nothing(nodes, slice, job)
        && curly_brace_compound_statement(nodes, slice, job)
        && curly_brace_field_declaration_list(nodes, slice, job)
        && curly_brace_enumerator_list(nodes, slice, job)
        && curly_brace_initializer_list(nodes, slice, job)
        && parenthesize_parameter_list(nodes, slice, job)
        && parenthesize_argument_list(nodes, slice, job)
        && expression_statement(nodes, slice, job)
        && append_space(nodes, slice, job);
        /* return false -> rule applyed and done */
#endif
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
