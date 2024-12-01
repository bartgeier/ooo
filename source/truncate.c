#include "truncate.h"
#include "OJob.h"
#include "Regex_signedComment.h"

#define REGEX_TRUNC_NODE_SPACE_IMPLEMENTATION
#include "Regex_truncNodeSpace.h"

#define REGEX_LINE_CONT_IMPLEMENTATION
#include "Regex_lineCont.h"

#define REGEX_TRUNC_COMMENT_SPACE_IMPLEMENTATION
#include "Regex_truncCommentSpace.h"

/* \n is the last character in a row */
/* returns the column idx            */
static uint32_t getColumn(char const *str, uint32_t idx) {
        uint32_t i = idx;
        while (i > 0) {
                i--;
                if (str[i] == '\n') {
                        break;
                }
        }
        uint32_t result = (i > 0) ? idx - i - 1 : idx;
        return result;
}

// Before the code gets formatted,
//      - all indentations are deleted and unnecessary spaces are removed.
//      - nodes damaged by line continuation are repaired
//        by removing the line continuation.
//      - begin end slice between two tree-sitter nodes
static void trunc_spaces(OJob *job, uint32_t const begin, size_t const end) {
        OStr const *source= &job->source;
        OStr *sink = &job->sink;

        size_t const column = getColumn(source->at, begin);
        uint32_t n = sink->size;
        Regex_truncNodeSpace_t nodeSpace;
        Regex_lineCont_t lineCont;

        Regex_truncNodeSpace_first(&nodeSpace, column);
        if (begin > 0) { 
                Regex_lineCont_first(&lineCont, source->at[begin - 1]); 
        }
 
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

// Is needed to correctly remove the indentation of an entire block comment
static size_t trunc_spaces_in_comment(OJob *job, size_t const begin, size_t const end) {
        OStr *source = &job->source;
        OStr *sink = &job->sink;

        size_t const column = getColumn(source->at, begin);
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
                        for (uint32_t i = idx; i < idx + new_comment_size - 1; i++) {
                                // remove block comment inside C++ comment, replace '*' with '\t'
                                if (job->sink.at[i] == '/' & job->sink.at[i+1] == '*') {
                                        job->sink.at[i+1] = '\t';
                                }
                                if (job->sink.at[i] == '*' & job->sink.at[i+1] == '/') {
                                        job->sink.at[i] = '\t';
                                }
                        }
                        /* line comment becomes a block comment */
                        // a comment
                        /* a comment */
                        job->sink.at[idx] = '/';
                        job->sink.at[idx + 1] = '*';
                        OStr_append_cstring(&job->sink, REGEX_SIGNED_COMMENT_ID"*/");
                }
        } else {
                for (size_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }
}
