#ifndef STRINGVIEW_H
#define STRINGVIEW_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
        size_t capacity;
        size_t size;
        size_t idx;
        char *at;
} StringView;

void StringView_clear(StringView *s);

#endif

//#define StringView_IMPLEMENTAION
#ifdef StringView_IMPLEMENTAION


void StringView_clear(StringView *s) {
        for (size_t i = 0; i < s->size; i++) {
                s->at[i] = 0;
        }
        s->size = 0;
        s->idx = 0;
}

void StringView_move(StringView *B, StringView *A) {
        for (size_t i = 0; i < A->size; i++) {
                B->at[i] = A->at[i];
                A->at[i] = 0;
        }
        B->size = A->size;
        B->at[B->size] = 0;
        A->size = 0;
}

void StringView_append_chr(StringView *m, char const chr) {
        if (m->size < m->capacity) {
                m->at[m->size++] = chr;
                m->at[m->size] = 0;
        }
}

void StringView_append_number_of_char( StringView *m, size_t n, char chr) {
        for (size_t i = 0; i < n; i++) {
                StringView_append_chr(m, chr);
        }
}

void StringView_append_spaces(StringView *m, size_t n) {
        for (size_t i = 0; i < n; i++) {
                StringView_append_chr(m, ' ');
        }
}

size_t StringView_at_least_1(StringView const *m, size_t begin, size_t end, char chr) {
        size_t count = 0;
        for (size_t i = begin; i < end; i++) {
                if (m->at[i] == chr) count++;
        }
        return (count == 0) ? 1 : count; 
}

size_t StringView_at_least_1_not_3(StringView const *m, size_t begin, size_t end, char chr) {
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

void StringView_replace_tabs_with_one_space(StringView *B, StringView *A) {
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

void StringView_remove_indentation(StringView *B, StringView *A) {
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

#endif
#undef StringView_IMPLEMENTAION
