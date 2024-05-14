#include "indentation.h"
#include "stdio.h"

#if 0
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
        OJob *job,
        TSNode node,
        size_t indentation_level
) {
        TSSymbol me = ooo(node);

        size_t cx = job->idx;
        size_t sx = OJob_cursor(job, ts_node_start_byte(node));
        indentation_level = indent(OOO_ENTRY, node, indentation_level);
        for (size_t i = cx; i < sx; i++) {
                if (job->source.at[i] == '\n') {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                        OStr_append_spaces(&job->sink, 4 * indentation_level); // <-- her plays the magic
                } else {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        indentation_level = indent(OOO_NEXT, node, indentation_level);
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                ooo_set_indentation(
                        job,
                        child,
                        indentation_level
                );
        }
        size_t ax = job->idx;
        size_t ex = OJob_cursor(job, ts_node_end_byte(node));
        // anon_sym_LF sym_preproc_if
        //if (ooo(super(1, node)) == sym_preproc_if) {
        if (me == anon_sym_LF) {
                /* preproc_def node includes the \n                */
                /* \n is then not used for indentation             */
                job->idx = (job->idx > 0) ? job->idx - 1 : 0;
                ex = job->idx;
                for (size_t i = ax; i < ex; i++) {
                        if (i == ax) OStr_append_chr(&job->sink, 'X');
                        if (i == ex-1) OStr_append_chr(&job->sink, 'Y');
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
                return;
        } 
        if (me == sym_comment) {
                /* inside of block comment */
                for (size_t i = ax; i < ex; i++) {
                        if (job->source.at[i] == '\n') {
                                OStr_append_chr(&job->sink, job->source.at[i]);
                                OStr_append_spaces(&job->sink, 4 * indentation_level);
                        } else {
                                OStr_append_chr(&job->sink, job->source.at[i]);
                        }
                }
        } else {
                for (size_t i = ax; i < ex; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }
}

#else

static size_t compound_statement(OOO_Transition const transition, TSNode const node, size_t level) {
        TSSymbol me = ooo(node);
        TSSymbol parent = ooo(super(1, node));
        TSSymbol grand = ooo(super(2, node));
        if (parent != sym_compound_statement) {
               return level;
        }
        if (me == anon_sym_LBRACE) {
                return level;
        }
        if (me == anon_sym_RBRACE) {
                return level;
        }
        return ++level;
}

static size_t indent(OOO_Transition const transition, TSNode const node, size_t level) {
        return compound_statement(transition, node, level);
}

void ooo_set_indentation(
        OJob *job,
        TSNode node,
        size_t indentation_level
) {
        TSSymbol me = ooo(node);

        size_t cx = job->idx;
        size_t sx = OJob_cursor(job, ts_node_start_byte(node));
        indentation_level = indent(OOO_ENTRY, node, indentation_level);
        for (size_t i = cx; i < sx; i++) {
                if (job->source.at[i] == '\n') {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                        OStr_append_spaces(&job->sink, 4 * indentation_level); // <-- her plays the magic
                } else {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        //indentation_level = indent(OOO_NEXT, node, indentation_level);
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                ooo_set_indentation(
                        job,
                        child,
                        indentation_level
                );
        }
        size_t ax = job->idx;
        size_t ex = OJob_cursor(job, ts_node_end_byte(node));
        // anon_sym_LF sym_preproc_if
        //if (ooo(super(1, node)) == sym_preproc_if) {
        if (me == anon_sym_LF) {
                /* preproc_def node includes the \n                */
                /* \n is then not used for indentation             */
                job->idx = (job->idx > 0) ? job->idx - 1 : 0;
                ex = job->idx;
                for (size_t i = ax; i < ex; i++) {
                        if (i == ax) OStr_append_chr(&job->sink, 'X');
                        if (i == ex-1) OStr_append_chr(&job->sink, 'Y');
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
                return;
        } 
        if (me == sym_comment) {
                /* inside of block comment */
                for (size_t i = ax; i < ex; i++) {
                        if (job->source.at[i] == '\n') {
                                OStr_append_chr(&job->sink, job->source.at[i]);
                                OStr_append_spaces(&job->sink, 4 * indentation_level);
                        } else {
                                OStr_append_chr(&job->sink, job->source.at[i]);
                        }
                }
        } else {
                for (size_t i = ax; i < ex; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }
}
#endif
