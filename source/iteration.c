#include "iteration.h"
#include "OStr.h"
#define REGEX_SIGNED_COMMENT_IMPLEMENTATION
#include "Regex_signedComment.h"

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
        NEW_LINE = set_NewLine_with_LineFeed(&job->sink, &job->source);
        replace_tabs_with_one_space(&job->source, &job->sink);
}

static void replace_LineFeed(OStr *B, OStr *A, char const lineFeed) {
        size_t x = 0;
        Regex_signedComment_t reg = {
                .state = RSC_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < A->size; i++) {
                bool const found = Regex_signedComment(&reg, i, A->at[i]);
                if (found) {
                        // remove signe
                        if (A->at[i] == '\n') {
                                B->at[x - (i - reg.begin)] = '/';
                                x -= reg.id_size;
                        } else {
                                x -= reg.id_size;
                                B->at[x++] = ' ';
                                B->at[x++] = '*';
                                B->at[x++] = '/';
                        }
                }

                if (A->at[i] == '\n') {
                        switch (lineFeed) {
                        case 'r':
                                B->at[x++] = '\r';
                                break;
                        case 'n':
                                B->at[x++] = '\n';
                                break;
                        case 'R':
                                B->at[x++] = '\r';
                                B->at[x++] = '\n';
                                break;
                        case 'N':
                                /* this is not normal */
                                B->at[x++] = '\n';
                                B->at[x++] = '\r';
                                break;
                        }
                } else {
                        B->at[x++] = A->at[i];
                }
        }
        bool const found = Regex_signedComment(&reg, A->size, 0);
        if (found) {
                B->at[reg.begin] = '/';
                x -= reg.id_size;
        }
        B->at[x] = 0;
        B->size = x;
        OStr_clear(A);
}

void last_iteration(OJob *job) {
        replace_LineFeed(&job->sink, &job->source, NEW_LINE);
}

