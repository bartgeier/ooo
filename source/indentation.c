#include "indentation.h"
#include "stdio.h"

static size_t indent(OOO_Transition const transition, TSNode const node, size_t level) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        TSSymbol prev_sibling = ooo(sibling(-1, node));
        TSSymbol prev_parent_sibling = ooo(sibling(-1, super(1, node)));
        TSSymbol next_parent_sibling = ooo(sibling(1, super(1, node)));

        switch (transition) { 
        case OOO_ENTRY:
                if (me != sym_compound_statement 
                & me != anon_sym_while
                & me != sym_parenthesized_expression
                & parent == sym_while_statement) { 
                        /* indent after while wihtout curly brace */
                        /* while (fuu)              */
                        /* ----->do_something(); */
                        return ++level;
                }
                if (me != sym_compound_statement 
                & me != anon_sym_if
                & me != sym_parenthesized_expression
                & me != sym_else_clause
                & parent == sym_if_statement) { 
                        /* indent after if wihtout curly brace */
                        /* if (fuu)              */
                        /* ----->do_something(); */
                        return ++level;
                }
                if (me != sym_compound_statement 
                & me != anon_sym_else 
                & me != sym_if_statement 
                & parent ==  sym_else_clause) {
                        /* indent after else wihtout curly brace */
                        /* else                  */
                        /* ----->do_something(); */
                        return ++level;
                }
                // if (me != sym_compound_statement 
                // & me != anon_sym_for
                // & prev_sibling == anon_sym_RPAREN
                // & parent == sym_for_statement) { 
                //         /* indent after for wihtout curly brace */
                //         /* for (fuu)              */
                //         /* ----->do_something(); */
                //         return ++level;
                // }
                if (me == anon_sym_RBRACE 
                & grand != sym_switch_statement 
                & grand != sym_case_statement
                & (parent == sym_compound_statement
                | parent == sym_enumerator_list
                | parent == sym_field_declaration_list
                | parent == sym_initializer_list)) {
                        /* '}' */
                        return --level;
                }
                if (me == anon_sym_RBRACE 
                & parent == sym_compound_statement
                & prev_parent_sibling != anon_sym_COLON
                & grand == sym_case_statement) {
                        /* '}' */
                        return --level;
                }
                if (me == anon_sym_RBRACE 
                & next_parent_sibling == sym_break_statement 
                & parent == sym_compound_statement
                & prev_parent_sibling == anon_sym_COLON
                & grand == sym_case_statement) {
                        /* '} break;' */
                        // return --level;
                }
                if (me == anon_sym_RBRACE 
                & prev_sibling == sym_break_statement 
                & parent == sym_compound_statement
                & prev_parent_sibling == anon_sym_COLON
                & grand == sym_case_statement) {
                        /* '    break;'  */
                        /* '}'           */
                        return --level;
                }
                if (me == anon_sym_RPAREN
                & (parent == sym_parameter_list 
                | parent == sym_argument_list)) {
                        /* ')'           */
                        return --level;
                }
                if (parent == sym_for_statement & me == anon_sym_RPAREN) {
                        /* for ( ) */ 
                        /*       ^ */
                        return --level;
                }
                return level;
        case OOO_NEXT:  
                if ((me == sym_compound_statement 
                & parent != sym_switch_statement 
                & parent != sym_case_statement
                & parent != sym_for_statement)
                | me == sym_enumerator_list
                | me == sym_field_declaration_list
                | me == sym_initializer_list) {
                        /* after '{'  */
                        return ++level;
                }
                if (me == sym_compound_statement 
                &   parent == sym_case_statement 
                &   prev_sibling != anon_sym_COLON) {
                        return ++level;
                }
                if (me == sym_case_statement) {
                        return ++level;
                }
                if (me == sym_argument_list) {
                        /* after 'printf ('  */
                        return ++level;
                }
                if (me == sym_parameter_list) {
                        /* after 'void foo('  */
                        return ++level;
                }
                if (me == sym_for_statement ) {//& me == anon_sym_LPAREN) {
                        /* after 'for ('  */
                        return ++level;
                }
                return level;
        case OOO_EXIT:  
                return level;
        }
}

void ooo_set_indentation(
        OStrCursor *cursor,
        OStr *sink,
        OStr *source,
        TSNode node,
        size_t indentation_level
) {
        TSSymbol me = ooo(node);

        size_t cx = cursor->idx;
        size_t sx = OStrCursor_move_to_point(
                cursor, 
                source, 
                ts_node_start_point(node)
        );
        indentation_level = indent(OOO_ENTRY, node, indentation_level);
        for (size_t i = cx; i < sx; i++) {
                if (source->at[i] == '\n') {
                        OStr_append_chr(sink, source->at[i]);
                        OStr_append_spaces(sink, 4 * indentation_level); // <-- her plays the magic
                } else {
                        OStr_append_chr(sink, source->at[i]);
                }
        }

        indentation_level = indent(OOO_NEXT, node, indentation_level);
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                ooo_set_indentation(
                        cursor,
                        sink,
                        source,
                        child,
                        indentation_level
                );
        }
        size_t ax = cursor->idx;
        size_t ex = OStrCursor_move_to_point(cursor, source, ts_node_end_point(node));
#if 0
        if (me == sym_preproc_def 
        | me == anon_sym_LF & ooo(super(1, node)) == sym_preproc_if) {
                /* preproc_def node includes the \n                */
                /* \n is then not used for indentation             */
                /* OStrCursor_decrement set the ex index before \n */
                OStrCursor_decrement(cursor, source);
                ex = cursor->idx;
        } 
#else
// anon_sym_LF sym_preproc_if
        //if (ooo(super(1, node)) == sym_preproc_if) {
        if (me == anon_sym_LF) {
                /* preproc_def node includes the \n                */
                /* \n is then not used for indentation             */
                /* OStrCursor_decrement set the ex index before \n */
                OStrCursor_decrement(cursor, source);
                ex = cursor->idx;
                for (size_t i = ax; i < ex; i++) {
                        if (i == ax) OStr_append_chr(sink, 'X');
                        if (i == ex-1) OStr_append_chr(sink, 'Y');
                        OStr_append_chr(sink, source->at[i]);
                }
                return;
        } 
#endif
        if (me == sym_comment) {
                /* inside of block comment */
                for (size_t i = ax; i < ex; i++) {
                        if (source->at[i] == '\n') {
                                OStr_append_chr(sink, source->at[i]);
                                OStr_append_spaces(sink, 4 * indentation_level);
                        } else {
                                OStr_append_chr(sink, source->at[i]);
                        }
                }
        } else {
                for (size_t i = ax; i < ex; i++) {
                        OStr_append_chr(sink, source->at[i]);
                }
        }
}
