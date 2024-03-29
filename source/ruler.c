#include "ruler.h"
#include <stdio.h>

typedef struct {
        size_t begin;
        size_t end;
} Slice;

bool curly_brace_compound_statement(
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol prev_parent_sibling = ooo(sibling(-1, super(1, node)));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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

bool curly_brace_field_declaration_list(
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));

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
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol serial = ooo(serial_node);
        TSSymbol serial_parent = ooo(super(1, serial_node));
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


bool dispatcher(
        TSNode node,
        TSNode serial_node,
        Slice slice,
        OJob *job
) {
        return 
        append_nothing(node, serial_node, slice, job)
        && curly_brace_compound_statement(node, serial_node, slice, job)
        && curly_brace_field_declaration_list(node, serial_node, slice, job)
        && curly_brace_enumerator_list(node, serial_node, slice, job)
        && curly_brace_initializer_list(node, serial_node, slice, job)
        && parenthesize_parameter_list(node, serial_node, slice, job)
        && parenthesize_argument_list(node, serial_node, slice, job)
        && expression_statement(node, serial_node, slice, job)
        && append_space(node, serial_node, slice, job);
        /* return false -> rule applyed and done */
}

TSNode ooo_ruler(
        TSNode node,
        TSNode serial_node,
        OJob *job
) {
        static size_t count = 0;

        TSPoint start_point = ts_node_start_point(node);
        Slice slice = {
                .begin = job->cursor.idx,
                .end = OStrCursor_move_to_point(&job->cursor, &job->source, start_point)
        };

        if (dispatcher(node, serial_node, slice, job)) {
                for (size_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        size_t num_of_childs = ts_node_child_count(node);
        TSNode serial = node;
        for (size_t it = 0; it < num_of_childs; it++) {
               TSNode child = ts_node_child(node, it);
                serial = ooo_ruler(
                        child,
                        serial,
                        job
                );
        }

        TSPoint end_point = ts_node_end_point(node);
        slice.begin = job->cursor.idx;
        slice.end = OStrCursor_move_to_point(&job->cursor, &job->source, end_point);
        for (size_t i = slice.begin; i < slice.end; i++) {
                OStr_append_chr(&job->sink, job->source.at[i]);
        }
        return serial;
}
