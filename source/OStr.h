#ifndef OSTRING_H
#define OSTRING_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree_sitter/api.h"
#include <stdlib.h>

typedef struct {
        size_t capacity;
        size_t size;
        char *at;
} OStr;

typedef struct {
        size_t row;
        size_t column;
        size_t idx;
} OStrCursor;


void OStr_clear(OStr *s);
void OStr_move(OStr *B, OStr *A);
void OStr_append_chr(OStr *m, char const chr);
void OStr_append_number_of_char(OStr *m, size_t n, char chr);
void OStr_append_spaces(OStr *m, size_t n);
size_t OStr_at_least_1(OStr const *m, size_t begin, size_t end, char chr);
size_t OStr_at_least_1_not_3(OStr const *m, size_t begin, size_t end, char chr);
void OStr_replace_tabs_with_one_space(OStr *B, OStr *A);

char OStr_set_NewLine_with_LineFeed(OStr *B, OStr *A);
void OStr_replace_LineFeed(OStr *B, OStr *A, char lineFeed);

void OStr_remove_indentation(OStr *B, OStr *A);
void OStrCursor_reset(OStrCursor *m);
size_t OStrCursor_move_to_point(OStrCursor *m, OStr const *s, TSPoint const p);

#endif

//#define OStr_IMPLEMENTAION
#ifdef OStr_IMPLEMENTAION


void OStr_clear(OStr *s) {
        for (size_t i = 0; i < s->size; i++) {
                s->at[i] = 0;
        }
        s->size = 0;
}

void OStr_move(OStr *B, OStr *A) {
        for (size_t i = 0; i < A->size; i++) {
                B->at[i] = A->at[i];
                A->at[i] = 0;
        }
        B->size = A->size;
        B->at[B->size] = 0;
        A->size = 0;
}

void OStr_append_chr(OStr *m, char const chr) {
        if (m->size < m->capacity) {
                m->at[m->size++] = chr;
                m->at[m->size] = 0;
        }
}

void OStr_append_number_of_char( OStr *m, size_t n, char chr) {
        for (size_t i = 0; i < n; i++) {
                OStr_append_chr(m, chr);
        }
}

void OStr_append_spaces(OStr *m, size_t n) {
        for (size_t i = 0; i < n; i++) {
                OStr_append_chr(m, ' ');
        }
}

size_t OStr_at_least_1(OStr const *m, size_t begin, size_t end, char chr) {
        size_t count = 0;
        for (size_t i = begin; i < end; i++) {
                if (m->at[i] == chr) count++;
        }
        return (count == 0) ? 1 : count; 
}

size_t OStr_at_least_1_not_3(OStr const *m, size_t begin, size_t end, char chr) {
        size_t count = 0;
        for (size_t i = begin; i < end; i++) {
                if (m->at[i] == chr) count++;
        }
        switch (count) {
        case 0:
                return 1;
        case 1:
        case 2:
                return count;
        default:
                return 2;
        }
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
                A->at[i] = 0;
        }
        B->at[x] = 0;
        B->size = x;
        A->size = 0;
}

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
                        B->at[x++] = '\n';
                        if (A->at[i] == '\r') { 
                                r += 2;
                                B->at[x++] = '\n';
                                state = 0; 
                                break; 
                        }
                        if (A->at[i] == '\n') { 
                                rn++; 
                                state = 0; 
                                break; 
                        }
                        state = 0;
                        B->at[x++] = A->at[i];
                        break;
                case 'n':
                        B->at[x++] = '\n';
                        if (A->at[i] == '\r') { 
                                nr++; 
                                state = 0; 
                                break; 
                        }
                        if (A->at[i] == '\n') { 
                                n += 2;  
                                B->at[x++] = '\n';
                                state = 0; 
                                break; 
                        }
                        state = 0;
                        B->at[x++] = A->at[i];
                        break;
                default:
                        /* never happens */
                        break;
                }
                A->at[i] = 0;
        }
        B->at[x] = 0;
        B->size = x;
        A->size = 0;
#if 0 
        if (r>rn & r>nr & r>n) return 'r';
        if (n>rn & n>nr & n>r) return 'n';
        if (rn>r & rn>nr & rn>r) return 'R';
        if (nr>r & nr>rn & nr>n) return 'N';
#else
        int max = r;
        if (n > max) max = n;
        if (rn > max) max = rn;
        if (nr > max) max = nr;
        if (max == r) return 'r';
        if (max == n) return 'n';
        if (max == rn) return 'R';
        if (max == nr) return 'N';
        assert(true);
        return 'N';
#endif
}

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
                A->at[i] = 0;
        }
        B->at[x] = 0;
        B->size = x;
        A->size = 0;
}

void OStr_remove_indentation(OStr *B, OStr *A) {
        bool indentation = false;
        size_t column = 0;
        size_t row = 0;
        size_t x = 0;
        for (size_t i = 0; i < A->size; i++) {
                if (A->at[i] == '\n') {
                        column = 0;
                        row++;
                } else {
                        column++;
                }
                if (A->at[i] == ' ' & column == 1) {
                       indentation = true; 
                } else if ((A->at[i] == ' ') & indentation) {
                       /* nothing */
                } else {
                       indentation = false; 
                       B->at[x++] = A->at[i];
                }
                A->at[i] = 0;
        }
        B->at[x] = 0;
        B->size = x;
        A->size = 0;
}

void OStrCursor_reset(OStrCursor *m) {
        m->idx = 0;
        m->column = 0;
        m->row = 0;
}

static size_t column(OStrCursor *m, OStr const *s) {
        size_t count = 1;
        while ((m->idx - count > 0) & (s->at[m->idx - count] != '\n')) {
                count++;
        }
        return count--;
}

size_t OStrCursor_move_to_point(OStrCursor *m, OStr const *s, TSPoint const p) {
        bool increment = (m->row < p.row) 
                       | ((m->row == p.row) & (m->column < p.column));
        bool decrement = (m->row > p.row) 
                       | ((m->row == p.row) & (m->column > p.column));
        if (increment) {
                do {
                        if (m->idx >= s->size) {
                                exit(1);
                        }
                        if (s->at[m->idx++] == '\n') {
                                m->row++;
                                m->column = 0;
                        } else {
                                m->column++;
                        }
                } while(m->row != p.row | m->column != p.column);
                return m->idx;
        }
        if (decrement) {
                do {
                        if (s->at[m->idx--] == '\n') {
                                m->row--;
                                m->column = column(m, s);
                        } else {
                                m->column--;
                        }
                } while(m->row != p.row | m->column != p.column);
                return m->idx;
        }
        return m->idx;
}

#endif
