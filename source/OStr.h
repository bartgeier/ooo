#ifndef OSTRING_H
#define OSTRING_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
        uint32_t begin;
        uint32_t end;
} Slice;

typedef struct {
        uint32_t capacity;
        uint32_t size;
        char *at;
} OStr;

void OStr_clear(OStr *s);
void OStr_append_chr(OStr *m, char const chr);
void OStr_append_number_of_chr(OStr *m, uint32_t n, char chr);
void OStr_append_spaces(OStr *m, uint32_t n);
void OStr_append_cstring(OStr *m, char const * str);
void OStr_final_truncate(OStr *str);
void OStr_shorten(OStr *m, uint32_t const size);
uint32_t OStr_need_1_or_2LF(OStr const *m, Slice const s);
bool OStr_last_has_LF(OStr const *m, Slice const s);
char OStr_need_LF_or_space(OStr const *m, Slice const s);
uint32_t OStr_number_of_chr( OStr const *m, Slice const s, char const chr);
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

void OStr_append_number_of_chr( OStr *m, uint32_t n, char chr) {
        for (uint32_t i = 0; i < n; i++) {
                OStr_append_chr(m, chr);
        }
}

void OStr_append_spaces(OStr *m, uint32_t n) {
     OStr_append_number_of_chr(m, n, ' ');
}


void OStr_append_cstring(OStr *m, char const * str) {
        for (uint32_t i = 0; i < strlen(str); i++) {
                OStr_append_chr(m, str[i]);
        }
}

void OStr_final_truncate(OStr *m) {
        assert(m->size < m->capacity - 1);
        if (m->size == 0) {
                return;
        }
        for (uint32_t i = m->size - 1; i < m->size; i--) {
                if (m->at[i] != '\n' & m->at[i] != ' ' & m->at[i] != 0) {
                        m->size = i + 1;
                        m->at[m->size] = 0;
                        return;
                }
        }
}

void OStr_shorten(OStr *m, uint32_t const minus) {
        assert(minus <= m->size);
        m->size -= minus;
        m->at[m->size] = 0;
}

uint32_t OStr_need_1_or_2LF(OStr const *m, Slice const s) {
        uint32_t count = 0;
        for (uint32_t i = s.begin; i < s.end; i++) {
                if (m->at[i] == '\n') count++;
        }
        return (count < 2) ? 1 : 2;
}

uint32_t OStr_number_of_chr( OStr const *m, Slice const s, char const chr) {
        assert(s.end <= m->size);
        uint32_t count = 0;
        for (uint32_t i = s.begin; i < s.end; i++) {
                if (m->at[i] == chr) count++;
        }
        return count;
}

bool OStr_last_has_LF(OStr const *m, Slice const s) {
        uint32_t const begin = s.begin;
        bool const b = (begin > 0) ? (m->at[begin -1] == '\n')  : false;
        return b;
}

char OStr_need_LF_or_space(OStr const *m, Slice const s) {
        uint32_t count = 0;
        for (uint32_t i = s.begin; i < s.end; i++) {
                if (m->at[i] == '\n') count++;
        }
        return (count > 0) ? '\n' : ' ';
}

#endif
#undef OSTR_IMPLEMENTAION
