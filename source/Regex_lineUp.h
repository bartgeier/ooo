#ifndef REGEX_LINE_UP_H
#define REGEX_LINE_UP_H

#include "stdint.h"
#include <stdbool.h>

typedef enum {
        RLU_LF,
        RLU_FIRST,
        RLU_BACK_SLASH,
        RLU_LINE_CONT,
} Regex_lineUp_e;

typedef struct {
        Regex_lineUp_e state;
        uint32_t begin;
        uint32_t end;
        uint32_t max_col;
        uint32_t column;
} Regex_lineUp_t;

// see diagrams.uxf
void Regex_lineUp_first(Regex_lineUp_t *self, uint32_t const column_idx);
bool Regex_lineUp(Regex_lineUp_t *self, char const chr, uint32_t const idx);

#endif

#ifdef REGEX_LINE_UP_IMPLEMENTATION

void Regex_lineUp_first(Regex_lineUp_t *self, uint32_t const column_idx) {
        self->column = column_idx;
        self->state = RLU_LF;
}

static uint32_t rlu_max(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

static bool rlu_end_of_line(Regex_lineUp_t *self, uint32_t idx) {
        switch(self->state) {
        case RLU_LF:
                self->column = 0;
                break;
        case RLU_FIRST:
                self->state = RLU_LINE_CONT;
                self->begin = idx - self->column;
                self->max_col = self->column - 1;
                self->column = 0;
                break;
        case RLU_LINE_CONT:
                self->state = RLU_LF;
                self->end = idx + 1;
                self->column = 0;
                return true;
        case RLU_BACK_SLASH:
                self->state = RLU_LINE_CONT;
                self->max_col = rlu_max(self->max_col, self->column - 1);
                self->column = 0;
                break;
        }
        return false;
}

static bool rlu_end_of_string(Regex_lineUp_t *self, uint32_t idx) {
        switch(self->state) {
        case RLU_LF:
        case RLU_FIRST:
                break;
        case RLU_LINE_CONT:
                self->state = RLU_LF;
                self->end = idx + 1;
                self->column = 0;
                return true;
        case RLU_BACK_SLASH:
                self->state = RLU_LF;
                self->max_col = rlu_max(self->max_col, self->column - 1);
                self->end = idx + 1;
                self->column = 0;
                return true;
        }
        return false;
}

static void rlu_back_slash(Regex_lineUp_t *self) {
        switch(self->state) {
        case RLU_LF:
                self->state = RLU_FIRST;
                self->column += 1;
                break;
        case RLU_FIRST:
                self->column += 1;
                break;
        case RLU_LINE_CONT:
                self->state = RLU_BACK_SLASH;
                self->column += 1;
                break;
        case RLU_BACK_SLASH:
                self->column += 1;
                break;
        }
}

static void rlu_default(Regex_lineUp_t *self) {
        switch(self->state) {
        case RLU_LF:
        case RLU_LINE_CONT:
                break;
        case RLU_FIRST:
                self->state = RLU_LF;
                break;
        case RLU_BACK_SLASH:
                self->state = RLU_LINE_CONT;
                break;
        }
        self->column += 1;
}

bool Regex_lineUp(Regex_lineUp_t *self, char const chr, uint32_t const idx) {
        switch(chr) {
        case '\n':
                return rlu_end_of_line(self, idx);
        case 0:
                return rlu_end_of_string(self, idx);
        case '\\':
                rlu_back_slash(self);
                return false;
        default:
                rlu_default(self);
                break;
        }
        return false;
}

#endif // IMPLEMENTATION
