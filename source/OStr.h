#ifndef OSTRING_H
#define OSTRING_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tree_sitter/api.h"
#include <stdlib.h>

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

#endif
#undef OSTR_IMPLEMENTAION
