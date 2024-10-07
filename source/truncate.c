#include "truncate.h"
#include "OJob.h"
#include "Regex_commentOpen.h"
#include <stdio.h>

/* \n is the last character in a row */
/* returns the column idx            */
static size_t fn_column(char const *str, size_t idx) {
        size_t i = idx;
        while (i > 0) {
                i--;
                if (str[i] == '\n') {
                        break;
                }
        }
        size_t result = (i > 0) ? idx - i - 1 : idx;
        return result;
}

#if 0
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
        size_t column = fn_column(A->at, start_idx);

        bool indentation = false;
        struct trunc {
                bool b;
                size_t idx;
        } trunc = { .b = false, .idx = 0 };
        size_t x = job->sink.size;
        for (size_t i = start_idx; i < end_idx; i++) {
                if (A->at[i] == '\\') {
                        /* preprocessor line continuation */
                        /* r = s\                         */
                        /* in(x);                         */
                        /* becomes this:                  */
                        /* r = sin(x);                    */
                        if ((i > 0) 
                        & ((i + 1) < (A->size)) 
                        & ((i + 2) < (A->size))
                        & (A->at[i - 1] != ' ' & A->at[i - 1] != '\n')
                        & (A->at[i + 1] == '\n') 
                        & (A->at[i + 2] != ' ' & A->at[i + 2] != '\n')) {
                                break;
                        }
                }
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

        size_t const i_column = fn_column(A->at, start_idx);
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

#else 

#define REGEX_TRUNC_NODE_SPACE_IMPLEMENTATION
#include "Regex_truncNodeSpace.h"
#define REGEX_LINE_CONT_IMPLEMENTATION
#include "Regex_lineCont.h"

#define REGEX_TRUNC_COMMENT_SPACE_IMPLEMENTATION
#include "Regex_truncCommentSpace.h"

static void trunc_spaces(OJob *job, size_t const begin, size_t const end) {
        OStr const *source= &job->source;
        OStr *sink = &job->sink;

        size_t const column = fn_column(source->at, begin);
        uint32_t n = sink->size;
        Regex_truncNodeSpace_t nodeSpace;
        Regex_lineCont_t lineCont;

        Regex_truncNodeSpace_first(&nodeSpace, column);
        Regex_lineCont_first(&lineCont, source->at[begin - 1]);

        for (uint32_t i = begin; i < end; i++) {
                // iterate slice between two tree-sitter nodes
                n -= Regex_truncNodeSpace(&nodeSpace, source->at[i]);
                n -= Regex_lineCont(&lineCont, source->at[i]);
                sink->at[n++] = source->at[i];
        }

        if (end < source->size) {
                n -= Regex_truncNodeSpace(&nodeSpace, source->at[end]);
                n -= Regex_lineCont(&lineCont, source->at[end]);
        } else {
                n -= Regex_truncNodeSpace(&nodeSpace, 0);
                n -= Regex_lineCont(&lineCont, 0);
        }
        sink->size = n;
}

static size_t trunc_spaces_in_comment(OJob *job, size_t const begin, size_t const end) {
        OStr *source = &job->source;
        OStr *sink = &job->sink;

        size_t const column = fn_column(source->at, begin);
        uint32_t n = sink->size;

        Regex_truncCommentSpace_t truncComment;
        Regex_truncCommentSpace_first(&truncComment, column);

        for (uint32_t i = begin; i < end; i++) {
                // iterate over tree-sitter comment nodes
                n -= Regex_truncCommentSpace(&truncComment, source->at[i]);
                sink->at[n++] = source->at[i];
        }
        
        // c block comment always returns 0
        // c++ comment line returns size to trunc space
        n -= Regex_truncCommentSpace(&truncComment, 0); 
        size_t const new_comment_size = n - sink->size;
        sink->size = n;
        return new_comment_size;
}
#endif

void ooo_truncate_spaces(
        TSNode node,
        OJob *job
) {
        TSSymbol me = sym(node);

        Slice slice = {
                .begin = job->idx,
                .end = job->idx = ts_node_start_byte(node)
        };

        trunc_spaces(
                job,
                slice.begin,
                slice.end
        );
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                ooo_truncate_spaces(child, job);
        }

        slice.begin = job->idx;
        slice.end = job->idx = ts_node_end_byte(node);
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
        if (me == sym_comment) {
                size_t const new_comment_size = trunc_spaces_in_comment(
                        job,
                        slice.begin,
                        slice.end
                );
                size_t const idx = job->sink.size - new_comment_size;
                if (job->sink.at[idx] == '/' & job->sink.at[idx + 1] == '/') {
                        /* line comment becomes a block comment */
                        // a comment
                        /* a comment */
                        job->sink.at[idx] = '/';
                        job->sink.at[idx + 1] = '*';
                        OStr_append_cstring(&job->sink, REGEX_COMMENT_CLOSE_ID"*/");
                }
        } else {
                for (size_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }
}
