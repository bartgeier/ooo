#include "indentation.h"
#include "stdio.h"

static bool compound_statement(Relation const *node, size_t *level) {
        if (grand(node) == sym_switch_statement
        && me(node) == sym_case_statement) {
                return true;
        }
        if (grand(node) == sym_case_statement && ts_node_start_point(node->parent).column > 0) {
                return true;
        }
        if (me(node) == anon_sym_LBRACE) {
                return true;
        }
        if (me(node) == anon_sym_RBRACE) {
                return true;
        }
        *level += 1;
        return true;
}

static bool field_declaration_list(Relation const *node, size_t *level) {
        if (me(node) == anon_sym_LBRACE) {
                return true;
        }
        if (me(node) == anon_sym_RBRACE) {
                return true;
        }
        *level += 1;
        return true;
}

static bool enumerator_list(Relation const *node, size_t *level) {
        if (me(node) == anon_sym_LBRACE) {
                return true;
        }
        if (me(node) == anon_sym_RBRACE) {
                return true;
        }
        *level += 1;
        return true;
}

static bool initializer_list(Relation const *node, size_t *level) {
        if (me(node) == anon_sym_LBRACE) {
                return true;
        }
        if (me(node) == anon_sym_RBRACE) {
                return true;
        }
        *level += 1;
        return true;
}

static bool parameter_list(Relation const *node, size_t *level) {
        if (me(node) == anon_sym_LPAREN) {
                return true;
        }
        if (me(node) == anon_sym_RPAREN) {
                return true;
        }
        *level += 1;
        return true;
}

static bool argument_list(Relation const *node, size_t *level) {
        if (me(node) == anon_sym_LPAREN) {
                return true;
        }
        if (me(node) == anon_sym_RPAREN) {
                return true;
        }
        *level += 1;
        return true;
}

static bool case_statement(Relation const *node, size_t *level) {
        *level += 1;
        return true;
}

static bool for_statement(Relation const *node, size_t *level) {
        if (me(node) == anon_sym_LPAREN) {
                return true;
        }
        if (me(node) == anon_sym_RPAREN) {
                return true;
        }
        if (me(node) == sym_compound_statement) {
                return true;
        }
        *level += 1;
        return true;
}

static bool while_statement(Relation const *node, size_t *level) {
        if (find_child(node, sym_compound_statement) < 0) {
                *level += 1;
                return true;
        }
        return true;
}

static bool if_statement(Relation const *node, size_t *level) {
        if (me(node) == sym_else_clause) {
                return true;
        }
        if (find_child(node, sym_compound_statement) < 0) {
                *level += 1;
                return true;
        }
        return true;
}

static bool else_clause(Relation const *node, size_t *level) {
        if (find_child(node, sym_compound_statement) < 0) {
                *level += 1;
                return true;
        }
        return true;
}

#if 0
static size_t dispatcher(Nodes *nodes, size_t level) {
        Relation node;
        Relation_init(&node, nodes);

        compound_statement(&node, &level)
        || field_declaration_list(&node, &level)
        || enumerator_list(&node, &level)
        || initializer_list(&node, &level)
        || parameter_list(&node, &level)
        || argument_list(&node, &level)
        || case_statement(&node, &level)
        || for_statement(&node, &level)
        || while_statement(&node, &level)
        || if_statement(&node, &level)
        || else_clause(&node, &level);

        return level;
}
#else
static size_t dispatcher(Nodes *nodes, size_t level) {
        Relation node;
        Relation_init(&node, nodes);

        switch(parent(&node)) {
                case sym_compound_statement: 
                        compound_statement(&node, &level);
                        break;
                case sym_field_declaration_list: 
                        field_declaration_list(&node, &level);
                        break;
                case sym_enumerator_list: 
                        enumerator_list(&node, &level);
                        break;
                case sym_initializer_list: 
                        initializer_list(&node, &level);
                        break;
                case sym_parameter_list: 
                        parameter_list(&node, &level);
                        break;
                case sym_argument_list: 
                        argument_list(&node, &level);
                        break;
                case sym_case_statement: 
                        case_statement(&node, &level);
                        break;
                case sym_for_statement: 
                        for_statement(&node, &level);
                        break;
                case sym_while_statement: 
                        while_statement(&node, &level);
                        break;
                case sym_if_statement: 
                        if_statement(&node, &level);
                        break;
                case sym_else_clause: 
                        else_clause(&node, &level);
                        break;
        }
        return level;
}
#endif

void ooo_set_indentation(
        OJob *job,
        Nodes *nodes,
        size_t indentation_level
) {
        TSNode node = Nodes_at(nodes, 0);
        TSSymbol me = ooo(node);

        size_t cx = job->idx;
        size_t sx = OJob_cursor(job, ts_node_start_byte(node));
        indentation_level = dispatcher(nodes, indentation_level);
        for (size_t i = cx; i < sx; i++) {
                if (job->source.at[i] == '\n') {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                        OStr_append_spaces(&job->sink, 4 * indentation_level); // <-- her plays the magic
                } else {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                Nodes_push(nodes,  child);
                ooo_set_indentation(
                        job,
                        nodes,
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
