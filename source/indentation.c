#include "indentation.h"
#include "stdio.h"

static size_t compound_statement(Relation const *node, size_t level) {
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

static size_t field_declaration_list(Relation const *node, size_t level) {
        if (me(node) == anon_sym_LBRACE) {
                return level;
        }
        if (me(node) == anon_sym_RBRACE) {
                return level;
        }
        level += 1;
        return level;
}

static size_t enumerator_list(Relation const *node, size_t level) {
        if (me(node) == anon_sym_LBRACE) {
                return level;
        }
        if (me(node) == anon_sym_RBRACE) {
                return level;
        }
        level += 1;
        return level;
}

static size_t initializer_list(Relation const *node, size_t level) {
        if (me(node) == anon_sym_LBRACE) {
                return level;
        }
        if (me(node) == anon_sym_RBRACE) {
                return level;
        }
        level += 1;
        return level;
}

static size_t parameter_list(Relation const *node, size_t level) {
        if (me(node) == anon_sym_LPAREN) {
                return level;
        }
        if (me(node) == anon_sym_RPAREN) {
                return level;
        }
        level += 1;
        return level;
}

static size_t argument_list(Relation const *node, size_t level) {
        if (me(node) == anon_sym_LPAREN) {
                return level;
        }
        if (me(node) == anon_sym_RPAREN) {
                return level;
        }
        level += 1;
        return level;
}

static size_t case_statement(Relation const *node, size_t level) {
        level += 1;
        return level;
}

static size_t for_statement(Relation const *node, size_t level) {
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

static size_t while_statement(Relation const *node, size_t level) {
        if (find_child(node, sym_compound_statement) < 0) {
                level += 1;
                return level;
        }
        return level;
}

static size_t if_statement(Relation const *node, size_t level) {
        if (me(node) == sym_else_clause) {
                return level;
        }
        if (find_child(node, sym_compound_statement) < 0) {
                level += 1;
                return level;
        }
        return level;
}

static size_t else_clause(Relation const *node, size_t level) {
        if (find_child(node, sym_compound_statement) < 0) {
                level += 1;
                return level;
        }
        return level;
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
                return compound_statement(&node, level);
        case sym_field_declaration_list: 
                return field_declaration_list(&node, level);
        case sym_enumerator_list: 
                return enumerator_list(&node, level);
        case sym_initializer_list: 
                return initializer_list(&node, level);
        case sym_parameter_list: 
                return parameter_list(&node, level);
        case sym_argument_list: 
                return argument_list(&node, level);
        case sym_case_statement: 
                return case_statement(&node, level);
        case sym_for_statement: 
                return for_statement(&node, level);
        case sym_while_statement: 
                return while_statement(&node, level);
        case sym_if_statement: 
                return if_statement(&node, level);
        case sym_else_clause: 
                return else_clause(&node, level);
        default:
                return level;
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
