#include "iteration.h"
#include "OStr.h"
#define REGEX_SIGNED_COMMENT_IMPLEMENTATION
#include "Regex_signedComment.h"
#define REGEX_LINE_FEED_IMPLEMENATATION
#include "Regex_lineFeed.h"
#define REGEX_TAB_FILTER_IMPLEMENTATION
#include "Regex_tabFilter.h"
#define REGEX_LINE_UP_IMPLEMENTATION
#include "Regex_lineUp.h"

char NEW_LINE = 0;

void first_iteration(OJob *job) {
        OStr *A = &job->source;
        OStr *B = &job->sink;
        Regex_tabFilter_t tabFilter = {.state = RTF_CHAR};
        Regex_lineFeed_t lineFeed = { .state = RLF_CHAR, .r = 0, .n = 0, .rn = 0, .nr = 0 };
        for (uint32_t i = 0; i < A->size; i++) {
                OptionalChar_t o = Regex_tabFilter(&tabFilter, A->at[i]);
                if (o.ok == false) {
                        continue;
                }
                o = Regex_lineFeed(&lineFeed, o.chr);
                if (o.ok) {
                        OStr_append_chr(B, o.chr);
                }
        }
        NEW_LINE = Regex_lineFeed_last(&lineFeed);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static void line_up(OJob *job, Regex_lineUp_t const *regex) {
        OStr *A = &job->source;
        OStr *B = &job->sink;
        uint32_t column = 0;
        for (uint32_t i = regex->begin; i < regex->end; i++) {
                if (A->at[i] == '\\' & A->at[i + 1] == '\n') {
                        uint32_t num_of_spaces = regex->max_col - column;
                        OStr_append_spaces(B, num_of_spaces);
                        OStr_append_chr(B, '\\');
                } else {
                        OStr_append_chr(B, A->at[i]);
                }
                if (A->at[i] == '\n') {
                        column = 0;
                } else {
                        column++;
                }
        }
}

// see truncate.c in:
//     removes block comment inside C++ comment, replace '*' with '\t'
// But here we replace '\t' with '*' or '/'
//    so we restore block comment inside a C++ comment
static void replace_A_with_B(
        OStr *s, 
        uint32_t const begin, 
        char A, char B
) {
        for (uint32_t i = begin; i < s->size; i++) {
                if (s->at[i] == A) {
                        s->at[i] = B;
                }
        }
}

void last_iteration(OJob *job) { 
        {
                /* line up => line continuation '\' character */
                OStr const *A = &job->source; 
                OStr *B = &job->sink; 
                Regex_lineUp_t regex;
                Regex_lineUp_first(&regex, 0);
                for (uint32_t i = 0; i < A->size + 1; i++) {
                        if (Regex_lineUp(&regex, A->at[i], i)) {
                                uint32_t delta = i - regex.begin;
                                B->size -= delta;
                                line_up(job, &regex); 
                        } else {
                                if (A->at[i] != 0) {
                                        OStr_append_chr(B, A->at[i]);
                                }
                        }
                }
        }

        OJob_swap(job);

        {
                OStr const *A = &job->source; 
                OStr *B = &job->sink; 
                Regex_signedComment_t reg = {
                        .state = RSC_IDLE, 
                        .found = false,
                        .begin = 0, 
                        .id_size = 0
                };
                for (uint32_t i = 0; i < A->size + 1; i++) {
                        bool const found = Regex_signedComment(&reg, i, A->at[i]);
                        if (found) {
                                // remove sign
                                if (A->at[i] == '\n' | A->at[i] == 0) {
                                        uint32_t begin = B->size - (i - reg.begin);
                                        B->at[begin] = '/';
                                        //B->size -= reg.id_size;
                                        OStr_shorten(B, reg.id_size);
                                        replace_A_with_B(B, begin, '\t', '*'); 
                                } else {
                                        uint32_t begin = B->size - (i - reg.begin);
                                        //B->size -= reg.id_size;
                                        OStr_shorten(B, reg.id_size);
                                        replace_A_with_B(B, begin, '\t', '/'); 
                                        OStr_append_chr(B, ' ');
                                        OStr_append_chr(B, '*');
                                        OStr_append_chr(B, '/');
                                }
                        }


                        if (A->at[i] == '\n') {
                                // replace line feed
                                switch (NEW_LINE) {
                                case 'r':
                                        OStr_append_chr(B, '\r');
                                        break;
                                case 'n':
                                        OStr_append_chr(B, '\n');
                                        break;
                                case 'R':
                                        OStr_append_chr(B, '\r');
                                        OStr_append_chr(B, '\n');
                                        break;
                                case 'N':
                                        /* this is not normal */
                                        OStr_append_chr(B, '\n');
                                        OStr_append_chr(B, '\r');
                                        break;
                                }
                        } else {
                                if (A->at[i] != 0) {
                                        OStr_append_chr(B, A->at[i]);
                                }
                        }
                }
                OJob_set_final_truncate(job);
                switch (NEW_LINE) {
                case 'r':
                        OStr_append_chr(B, '\r');
                        break;
                case 'n':
                        OStr_append_chr(B, '\n');
                        break;
                case 'R':
                        OStr_append_chr(B, '\r');
                        OStr_append_chr(B, '\n');
                        break;
                case 'N':
                        /* this is not normal */
                        OStr_append_chr(B, '\n');
                        OStr_append_chr(B, '\r');
                        break;
                }
        }
        //OJob_set_final_LF(&job);
}

