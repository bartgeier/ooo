#include "indentation.h"
#include "tree_navigator.h"
#include "Pars.h"

static uint32_t preproc_call(Relation const *node, uint32_t level) {
        if (me(node) == sym_preproc_arg) {
                TSNode me_n = Relation_track_node(node, 0); // me as TSNode
                TSPoint a = ts_node_start_point(node->parent);
                TSPoint b = ts_node_start_point(me_n);
                if (a.row == b.row) {
                        /* #pragma do { */
                        /*     ...      */
                        return level;
                }
                /* #pragma \      */
                /*     do {       */
                /*         ...    */
                level += 1;
        }
        return level;
}

static uint32_t preproc_def(Relation const *node, uint32_t level) {
        if (me(node) == sym_preproc_arg) {
                TSNode me_n = Relation_track_node(node, 0); // me as TSNode
                TSPoint a = ts_node_start_point(node->parent);
                TSPoint b = ts_node_start_point(me_n);
                if (a.row == b.row) {
                        /* #define FOO \  */
                        /*     do {       */
                        /*         ...    */
                        return level;
                }
                /* #define FOO \  */
                /*     do {       */
                /*         ...    */
                level += 1;
        }
        return level;
}

static uint32_t preproc_function_def(Relation const *node, uint32_t level) {
        if (me(node) == sym_preproc_arg) {
                TSNode me_n = Relation_track_node(node, 0); // me as TSNode
                TSPoint a = ts_node_start_point(node->parent);
                TSPoint b = ts_node_start_point(me_n);
                if (a.row == b.row) {
                        /* #define FOO(a, b) do { */
                        /*     ...                */
                        return level;
                }
                /* #define FOO(a, b) \ */
                /*     do {            */
                /*         ...         */
                level += 1;
        }
        return level;
}

static uint32_t preproc_params(Relation const *node, uint32_t level) {
        if (me(node) == anon_sym_RPAREN) {
                return level;
        }
        level += 1;
        return level;

}

static uint32_t preproc_arg(Relation const *node, uint32_t level) {
        return level;
}





static uint32_t compound_statement(Relation const *node, uint32_t level) {
        if (grand(node) == sym_switch_statement
        && me(node) == sym_case_statement) {
                return level;
        }
        if (grand(node) == sym_case_statement && ts_node_start_point(node->parent).column > 0) {
                return level;
        }
        if (me(node) == anon_sym_LBRACE) {
                return level;
        }
        if (me(node) == anon_sym_RBRACE) {
                return level;
        }
        level += 1;
        return level;
}

static uint32_t field_declaration_list(Relation const *node, uint32_t level) {
        if (me(node) == anon_sym_LBRACE) {
                return level;
        }
        if (me(node) == anon_sym_RBRACE) {
                return level;
        }
        level += 1;
        return level;
}

static uint32_t enumerator_list(Relation const *node, uint32_t level) {
        if (me(node) == anon_sym_LBRACE) {
                return level;
        }
        if (me(node) == anon_sym_RBRACE) {
                return level;
        }
        level += 1;
        return level;
}

static uint32_t initializer_list(Relation const *node, uint32_t level) {
        if (me(node) == anon_sym_LBRACE) {
                return level;
        }
        if (me(node) == anon_sym_RBRACE) {
                return level;
        }
        level += 1;
        return level;
}

static uint32_t parameter_list(Relation const *node, uint32_t level) {
        if (me(node) == anon_sym_LPAREN) {
                return level;
        }
        if (me(node) == anon_sym_RPAREN) {
                return level;
        }
        level += 1;
        return level;
}

static uint32_t argument_list(Relation const *node, uint32_t level) {
        if (me(node) == anon_sym_LPAREN) {
                return level;
        }
        if (me(node) == anon_sym_RPAREN) {
                return level;
        }
        level += 1;
        return level;
}

static uint32_t case_statement(Relation const *node, uint32_t level) {
        level += 1;
        return level;
}

static uint32_t for_statement(Relation const *node, uint32_t level) {
        if (me(node) == anon_sym_LPAREN) {
                return level;
        }
        if (me(node) == anon_sym_RPAREN) {
                return level;
        }
        if (me(node) == sym_compound_statement) {
                return level;
        }
        level += 1;
        return level;
}

static uint32_t while_statement(Relation const *node, uint32_t level) {
        if (find_child(node, sym_compound_statement) < 0) {
                level += 1;
                return level;
        }
        return level;
}

static uint32_t do_statement(Relation const *node, uint32_t level) {
        return level;
}

static uint32_t if_statement(Relation const *node, uint32_t level) {
        if (me(node) == sym_else_clause) {
                return level;
        }
        if (find_child(node, sym_compound_statement) < 0) {
                level += 1;
                return level;
        }
        return level;
}

static uint32_t else_clause(Relation const *node, uint32_t level) {
        if (me(node) == sym_if_statement) {
                return level;
        }
        if (find_child(node, sym_compound_statement) < 0) {
                level += 1;
                return level;
        }
        return level;
}

static uint32_t conditional_expression(Relation const *node, uint32_t level) {
       level +=1;
       return level; 
}

static uint32_t dispatcher(Relation *node, uint32_t level) {
        switch(parent(node)) {

        case sym_preproc_call: 
                return preproc_call(node, level);
        case sym_preproc_def: 
                return preproc_def(node, level);
        case sym_preproc_function_def: 
                return preproc_function_def(node, level);
        case sym_preproc_params:
                return preproc_params(node, level);
        case sym_preproc_arg: 
                return preproc_arg(node, level);

        case sym_compound_statement: 
                return compound_statement(node, level);
        case sym_field_declaration_list: 
                return field_declaration_list(node, level);
        case sym_enumerator_list: 
                return enumerator_list(node, level);
        case sym_initializer_list: 
                return initializer_list(node, level);
        case sym_parameter_list: 
                return parameter_list(node, level);
        case sym_argument_list: 
                return argument_list(node, level);
        case sym_case_statement: 
                return case_statement(node, level);
        case sym_for_statement: 
                return for_statement(node, level);
        case sym_while_statement: 
                return while_statement(node, level);
        case sym_do_statement: 
                return do_statement(node, level);
        case sym_if_statement: 
                return if_statement(node, level);
        case sym_else_clause: 
                return else_clause(node, level);
        case sym_conditional_expression:
                return conditional_expression(node, level);
        default:
                return level;
        }
        return level;
}

void ooo_set_indentation(
        OJob *job,
        Relation *relation,
        uint32_t indentation_level
) {
        TSNode const node = Relation_track_node(relation, 0);
        TSSymbol const me = sym(node);

        Slice slice = {
                .begin = job->idx,
                .end = job->idx = (ts_node_start_byte(node) + job->offset)
        };
        indentation_level = dispatcher(relation, indentation_level);
        for (uint32_t i = slice.begin; i < slice.end; i++) {
                if (job->source.at[i] == '\n') {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                        if (job->source.at[i+1] != '\n'
                        & job->source.at[i+1] != 0) {
                                OStr_append_spaces(&job->sink, 4 * indentation_level); // <-- her plays the magic
                        }
                } else {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        for (uint32_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode const child = ts_node_child(node, it);
                Relation_parent_push(relation, node);
                Relation_track(relation, child);
                ooo_set_indentation(
                        job,
                        relation,
                        indentation_level
                );
        }

        slice.begin = job->idx;
        slice.end = job->idx = (ts_node_end_byte(node) + job->offset);
        if (slice.end > 0 && job->source.at[slice.end - 1] == '\n') {
                /* \n is last member byte in the node  */
                /* thats caused missing indentation    */
                /* therefore decrement job->idx        */
                /* problematic nodes are for example:  */
                /*     anon_sym_LF                     */
                /*     sym_preproc_def                 */
                job->idx--;
                slice.end = job->idx;
        } 
        if (me == sym_comment) {
                /* inside of block comment */
                for (uint32_t i = slice.begin; i < slice.end; i++) {
                        if (job->source.at[i] == '\n') {
                                OStr_append_chr(&job->sink, job->source.at[i]);
                                OStr_append_spaces(&job->sink, 4 * indentation_level);
                        } else {
                                OStr_append_chr(&job->sink, job->source.at[i]);
                        }
                }
        } else if (me == sym_preproc_arg) {
                uint32_t const o = job->offset;
                job->idx = job->offset = slice.begin;
                RootNode_t root = Pars_getTree(&job->source.at[slice.begin], slice.end - slice.begin);
                Relation_parent_push(relation, node);
                Relation_track(relation, root.node);
                ooo_set_indentation(job, relation, indentation_level);
                Pars_freeTree(root);
                job->offset = o;
        } else {
                for (uint32_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }
}
