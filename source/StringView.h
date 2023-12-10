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
        size_t idx;
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
void OStr_remove_indentation(OStr *B, OStr *A);

void OStrCursor_reset(OStrCursor *m);
size_t OStrCursor_move_to_point(OStrCursor *m, OStr const *s, TSPoint const p);
bool OStrCursor_increment(OStrCursor *m, OStr const *s);
bool OStrCursor_decrement(OStrCursor *m, OStr const *s);

#endif

//#define OStr_IMPLEMENTAION
#ifdef OStr_IMPLEMENTAION


void OStr_clear(OStr *s) {
        for (size_t i = 0; i < s->size; i++) {
                s->at[i] = 0;
        }
        s->size = 0;
        s->idx = 0;
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

bool OStrCursor_increment(OStrCursor *m, OStr const *s) {
       if (s->at[m->idx + 1] >= s->size) {
               return true; // fail
       }
       m->idx++;
       if (s->at[m->idx] == '\n') {
               m->row++;
               m->column = 0;
       } else {
               m->column++;
       }
       return false; // successful
}


bool OStrCursor_decrement(OStrCursor *m, OStr const *s) {
       if (s->at[m->idx] == 0) {
               return true; // fail
       }
       m->idx--;
       if (s->at[m->idx] == '\n') {
               m->row--;
               m->column = column(m, s);
       } else {
               m->column--;
       }
       return false; // successful
}
#endif
