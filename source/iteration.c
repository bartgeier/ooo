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

/* B = A; and lineFeeds are replaced with \n            */
/* A->size = 0;                                         */
/* return what kind of lineFeed original used in A      */
/* the moste used one will be returned                  */
/* \n -> n                                              */
/* \r -> r                                              */
/* \r\n -> R                                            */
/* \n\r -> N                                            */
static char set_NewLine_with_LineFeed(OStr *B, OStr *A) {
        int rn = 0;
        int nr = 0;
        int r = 0;
        int n = 0;
        char state = 0;
        size_t x = 0;
        for (size_t i = 0; i < A->size; i++) {
                switch (state) {
                case 0:
                        if (A->at[i] == '\r') { state = 'r'; break; }
                        if (A->at[i] == '\n') { state = 'n'; break; }
                        B->at[x++] = A->at[i];
                        break;
                case 'r':
                        if (A->at[i] == '\r') { 
                                B->at[x++] = '\n';
                                B->at[x++] = '\n';
                                r += 2;
                                state = 0; 
                                break; 
                        }
                        if (A->at[i] == '\n') { 
                                B->at[x++] = '\n';
                                rn++; 
                                state = 0; 
                                break; 
                        }
                        B->at[x++] = '\n';
                        B->at[x++] = A->at[i];
                        r++;
                        state = 0;
                        break;
                case 'n':
                        if (A->at[i] == '\r') { 
                                B->at[x++] = '\n';
                                nr++; 
                                state = 0; 
                                break; 
                        }
                        if (A->at[i] == '\n') { 
                                B->at[x++] = '\n';
                                B->at[x++] = '\n';
                                n += 2;  
                                state = 0; 
                                break; 
                        }
                        B->at[x++] = '\n';
                        B->at[x++] = A->at[i];
                        n++;
                        state = 0;
                        break;
                default:
                        /* never happens */
                        break;
                }
        }
        B->at[x] = 0;
        B->size = x;
        OStr_clear(A);
        int max = r;
        if (n > max) max = n;
        if (rn > max) max = rn;
        if (nr > max) max = nr;
        if (max == r) return 'r';
        if (max == n) return 'n';
        if (max == rn) return 'R';
        if (max == nr) return 'N';
        assert(false);
        return 'N';
}

char NEW_LINE = 0;

static void replace_tabs_with_one_space(OStr *B, OStr *A) {
        bool tab = false;
        size_t x = 0;
        for (size_t i = 0; i < A->size; i++) {
                if (A->at[i] == '\t') {
                        if (!tab) {
                                B->at[x++] = ' ';
                        }
                        tab = true;
                } else {
                        tab = false;
                        B->at[x++] = A->at[i];
                }
        }
        B->at[x] = 0;
        B->size = x;
        OStr_clear(A);
}

void first_iteration(OJob *job) {
        OStr *source = &job->source;
        OStr *sink = &job->sink;

        Regex_tabFilter_t tabFilter = {.state = RTF_CHAR};
        Regex_lineFeed_t lineFeed = { .state = RLF_CHAR, .r = 0, .n = 0, .rn = 0, .nr = 0 };
        for (uint32_t i = 0; i < source->size; i++) {
                OptionalChar_t o = Regex_tabFilter(&tabFilter, source->at[i]);
                if (o.ok == false) {
                        continue;
                }
                o = Regex_lineFeed(&lineFeed, o.chr);
                if (o.ok) {
                        OStr_append_chr(sink, o.chr);
                }
        }
        NEW_LINE = Regex_lineFeed_last(&lineFeed);
        OJob_swap(job);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static void line_up(OJob *job, Regex_lineUp_t const *regex) {
        uint32_t column = 0;
        for (uint32_t i = regex->begin; i < regex->end; i++) {
                if (job->source.at[i] == '\\' & job->source.at[i + 1] == '\n') {
                        uint32_t num_of_spaces = regex->max_col - column;
                        OStr_append_spaces(&job->sink, num_of_spaces);
                        OStr_append_chr(&job->sink, '\\');
                } else {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
                if (job->source.at[i] == '\n') {
                        column = 0;
                } else {
                        column++;
                }
        }
}

static void line_up_line_continuation(OJob *job) {
        OStr *A = &job->source;
        OStr *B = &job->sink; 
        A->at[A->size] = 0;
        Regex_lineUp_t regex;
        Regex_lineUp_first(&regex, 0);
        for (uint32_t i = 0; i < A->size + 1; i++) {
                if (Regex_lineUp(&regex, A->at[i], i)) {
                        uint32_t delta = i - regex.begin;
                        B->size -= delta;
                        line_up(job, &regex); 
                } else {
                        OStr_append_chr(B, A->at[i]);
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
        OStr *A = &job->source;
        OStr *B = &job->sink; 

        Regex_signedComment_t reg = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < A->size; i++) {
                bool const found = Regex_signedComment(&reg, i, A->at[i]);
                if (found) {
                        // remove sign
                        if (A->at[i] == '\n') {
                                uint32_t begin = B->size - (i - reg.begin);
                                B->at[begin] = '/';
                                B->size -= reg.id_size;
                                replace_A_with_B(B, begin, '\t', '*'); 
                        } else {
                                uint32_t begin = B->size - (i - reg.begin);
                                B->size -= reg.id_size;
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
                        OStr_append_chr(B, A->at[i]);
                }
        }
        bool const found = Regex_signedComment(&reg, A->size, 0);
        if (found) {
                uint32_t begin = B->size - (A->size - reg.begin);
                B->at[begin] = '/';
                B->size -= reg.id_size;
                replace_A_with_B(B, begin, '\t', '*'); 
        }
        OStr_clear(A);

        OJob_swap(job);
        line_up_line_continuation(job);
        todo uint tests for 0 end_of_string
}

