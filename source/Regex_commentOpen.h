#ifndef REGEX_COMMENT_OPEN_H
#define REGEX_COMMENT_OPEN_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
        REGEX1_IDLE,
        REGEX1_CPP,
        REGEX1_SLASH_0,
        REGEX1_SLASH_1,
        REGEX1_ASTERISK_0,
        REGEX1_ASTERISK_1,
        REGEX1_ASTERISK_2,
        REGEX1_ID_OK,
        REGEX1_DISCARD,
} Regex_commentOpen_e;

typedef struct {
        Regex_commentOpen_e state;
        bool found;
        size_t begin;
        size_t id_size;
} Regex_commentOpen_t;

bool Regex_commentOpen(Regex_commentOpen_t *self, size_t const idx, char const chr);

#endif

