#ifndef OSTRING_H
#define OSTRING_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tree_sitter/api.h"
#include <stdlib.h>
//#include "Regex.h"
#include "Regex_commentOpen.h"

typedef struct {
        size_t begin;
        size_t end;
} Slice;

typedef struct {
        size_t capacity;
        size_t size;
        char *at;
} OStr;

void OStr_clear(OStr *s);
void OStr_append_chr(OStr *m, char const chr);
void OStr_append_number_of_chr(OStr *m, size_t n, char chr);
void OStr_append_spaces(OStr *m, size_t n);
void OStr_append_cstring(OStr *m, char const * str);
size_t OStr_need_1_or_2LF(OStr const *m, Slice const s);
bool OStr_last_has_LF(OStr const *m, Slice const s);
char OStr_need_LF_or_space(OStr const *m, Slice const s);

void OStr_replace_tabs_with_one_space(OStr *B, OStr *A);
char OStr_set_NewLine_with_LineFeed(OStr *B, OStr *A);
void OStr_replace_LineFeed(OStr *B, OStr *A, char lineFeed);

#endif

#ifdef OSTR_IMPLEMENTAION

void OStr_clear(OStr *m) {
        m->size = 0;
        m->at[0] = 0;
}

void OStr_append_chr(OStr *m, char const chr) {
        assert(m->size < m->capacity - 1);
        m->at[m->size++] = chr;
        m->at[m->size] = 0;
}

void OStr_append_number_of_chr( OStr *m, size_t n, char chr) {
        for (size_t i = 0; i < n; i++) {
                OStr_append_chr(m, chr);
        }
}

void OStr_append_spaces(OStr *m, size_t n) {
     OStr_append_number_of_chr(m, n, ' ');
}


void OStr_append_cstring(OStr *m, char const * str) {
        for (size_t i = 0; i < strlen(str); i++) {
                OStr_append_chr(m, str[i]);
        }
}

size_t OStr_need_1_or_2LF(OStr const *m, Slice const s) {
        size_t count = 0;
        for (size_t i = s.begin; i < s.end; i++) {
                if (m->at[i] == '\n') count++;
        }
        return (count < 2) ? 1 : 2;
}

bool OStr_last_has_LF(OStr const *m, Slice const s) {
        size_t const begin = s.begin;
        bool const b = (begin > 0) ? (m->at[begin -1] == '\n')  : false;
        return b;
}

char OStr_need_LF_or_space(OStr const *m, Slice const s) {
        size_t count = 0;
        for (size_t i = s.begin; i < s.end; i++) {
                if (m->at[i] == '\n') count++;
        }
        return (count > 0) ? '\n' : ' ';
}


void OStr_replace_tabs_with_one_space(OStr *B, OStr *A) {
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

/* B = A; and lineFeeds are replaced with \n            */
/* A->size = 0;                                         */
/* return what kind of lineFeed original used in A      */
/* the moste used one will be returned                  */
/* \n -> n                                              */
/* \r -> r                                              */
/* \r\n -> R                                            */
/* \n\r -> N                                            */
char OStr_set_NewLine_with_LineFeed(OStr *B, OStr *A) {
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

#if 0
void OStr_replace_LineFeed(OStr *B, OStr *A, char lineFeed) {
        size_t x = 0;
        for (size_t i = 0; i < A->size; i++) {
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
                        //printf("hello");
                        B->at[x++] = A->at[i];
                }
        }
        B->at[x] = 0;
        B->size = x;
        OStr_clear(A);
}
#else

void OStr_replace_LineFeed(OStr *B, OStr *A, char lineFeed) {
        size_t x = 0;
        //Regex_CommentBlock_1Line_t reg = {RCB1_IDLE, 0, 0};
        Regex_commentOpen_t reg = {
                .state = REGEX1_IDLE, 
                .found = false,
                .begin = 0, 
                .id_size = 0
        };
        for (size_t i = 0; i < A->size; i++) {

                // bool const found =  Regex_commentBlock_1line(&reg, i, A->at[i]);
                // if (found) {
                //         B->at[x - (i - reg.begin)] = '/'; // '/*' becomes '//'
                //         x = x - reg.num_of_truncate;      // trunc end of line
                // }

                bool const found = Regex_commentOpen(&reg, i, A->at[i]);
                if (found) {
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
        
        // bool const found =  Regex_commentBlock_1line(&reg, A->size, 0);
        // if (found) {
        //         // case end of OStr
        //         B->at[x - (x - reg.begin)] = '/';
        //         x = x - reg.num_of_truncate;
        // }

        bool const found = Regex_commentOpen(&reg, A->size, 0);
        if (found) {
                B->at[reg.begin] = '/';
                x -= reg.id_size;
        }
        B->at[x] = 0;
        B->size = x;
        OStr_clear(A);
}
#endif

#endif
#undef OSTR_IMPLEMENTAION
