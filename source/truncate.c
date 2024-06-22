#include "truncate.h"
#include "OJob.h"

/* \n is the last character in a row */
/* returns the column idx            */
static size_t fn_column(OStr const *str, size_t idx) {
        size_t i = idx;
        while (i > 0) {
                i--;
                if (str->at[i] == '\n') {
                        break;
                }
        }
        size_t result = (i > 0) ? idx - i - 1 : idx;
        return result;
}

/* source funcA();   \n         funcB();    */
/*      start_idx ^     end_idx ^           */
/* ---------------------------------------- */
/* sink   funcA(); <--before trunc_space    */
/*      sink.size ^                         */
/* sink   funcA();\n <--after trunc_space   */
/*        sink.size ^                       */
static void trunc_spaces(OJob *job, size_t const start_idx, size_t const end_idx) {
        OStr *A = &job->source;
        OStr *B = &job->sink;
        size_t column = fn_column(A, start_idx);

        bool indentation = false;
        struct trunc {
                bool b;
                size_t idx;
        } trunc = { .b = false, .idx = 0 };
        size_t x = job->sink.size;
        for (size_t i = start_idx; i < end_idx; i++) {
                if (A->at[i] == ' ' & column == 0) {
                       indentation = true; 
                } 
                if ((A->at[i] == ' ') & !trunc.b) {
                        trunc.b = true;
                        trunc.idx = x;
                }
                if ((A->at[i] != ' ') | !indentation) {
                        if ((A->at[i] == '\n') & trunc.b) {
                                x = trunc.idx;
                                trunc.b = false;
                        } else if (A->at[i] != ' ') {
                                trunc.b = false;
                        }
                        indentation = false; 
                        B->at[x++] = A->at[i];
                }
                column = (A->at[i] == '\n') ? 0 :  column + 1;
        }
        B->at[x] = 0;
        B->size = x;
}

static size_t trunc_spaces_in_comment(OJob *job, size_t const start_idx, size_t const end_idx) {
        OStr *A = &job->source;
        OStr *B = &job->sink;

        bool indentation = false;
        struct trunc {
                bool b;
                size_t idx;
        } trunc = { .b = false, .idx = 0 };

        size_t const i_column = fn_column(A, start_idx);
        size_t column = i_column;

        size_t x = job->sink.size;
        for (size_t i = start_idx; i < end_idx; i++) {
                if (A->at[i] == ' ' & column == 0) {
                       indentation = true; 
                } 
                if ((A->at[i] == ' ') & !trunc.b) {
                        trunc.b = true;
                        trunc.idx = x;
                }
                if ((A->at[i] != ' ') | (column >= i_column) | !indentation) {
                        if ((A->at[i] == '\n') & trunc.b) {
                                x = trunc.idx;
                                trunc.b = false;
                        } else if (A->at[i] != ' ') {
                                trunc.b = false;
                        }
                        indentation = false; 
                        B->at[x++] = A->at[i];
                }
                column = (A->at[i] == '\n') ? 0 :  column + 1;
        }
        size_t const new_comment_size = x - B->size;
        B->at[x] = 0;
        B->size = x;
        return new_comment_size;
}

void ooo_truncate_spaces(
        TSNode node,
        OJob *job
) {
        TSSymbol me = ooo(node);

        Slice s = {
                .begin = job->idx,
                .end = job->idx = ts_node_start_byte(node)
        };

        trunc_spaces(
                job,
                s.begin,
                s.end
        );
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                ooo_truncate_spaces(child, job);
        }

        s.begin = job->idx;
        s.end = job->idx = ts_node_end_byte(node);
        if (me == sym_comment) {
                size_t const new_comment_size = trunc_spaces_in_comment(
                        job,
                        s.begin,
                        s.end
                );
                if (job->sink.at[job->sink.size - 1] != '/') {
                        /* line comment becomes a block comment */
                        // a comment
                        /* a comment */
                        size_t const idx = job->sink.size - new_comment_size;
                        job->sink.at[idx] = '/';
                        job->sink.at[idx + 1] = '*';
                        OStr_append_cstring(&job->sink, " */");
                }
        } else {
                for (size_t i = s.begin; i < s.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }
}
