#ifndef REGEX_TRUNC_COMMENT_SPACE_H
#define REGEX_TRUNC_COMMENT_SPACE_H

#include <stdint.h>

typedef enum {
        TCS_IDLE,
        TCS_INDENTATION,
        TCS_TAIL_SPACE,
} Regex_truncCommentSpace_e;

typedef struct {
        Regex_truncCommentSpace_e state;
        uint32_t size;
        uint32_t column;
        uint32_t max_trunc;
}Regex_truncCommentSpace_t;

// see diagrams.uxf
void Regex_truncCommentSpace_first(
        Regex_truncCommentSpace_t *self, 
        uint32_t const column_idx
);
uint32_t Regex_truncCommentSpace(
        Regex_truncCommentSpace_t *self, 
        char const chr
);

#endif

#ifdef REGEX_TRUNC_COMMENT_SPACE_IMPLEMENTATION

#define TCS_MIN(A, B) ((A) < (B) ? (A) : (B))

void Regex_truncCommentSpace_first(
        Regex_truncCommentSpace_t *self, 
        uint32_t const column_idx
) {
        self->state = TCS_IDLE;
        self->column = column_idx;
        self->max_trunc = column_idx;
}

static uint32_t tcs_space(Regex_truncCommentSpace_t *self) {
        switch(self->state) {
        case TCS_IDLE:
                self->size = 1;
                if (self->column == 0) {
                        self->state = TCS_INDENTATION;
                } else {
                        self->state = TCS_TAIL_SPACE;
                }
                return 0;
        case TCS_INDENTATION:
        case TCS_TAIL_SPACE:
                self->size += 1;
                self->column += 1;
                return 0;
        }
        return 0;
}

static uint32_t tcs_endOfLine(Regex_truncCommentSpace_t *self) {
        switch(self->state) {
        case TCS_IDLE:
                self->column = 0;
                return 0;
        case TCS_INDENTATION:
                self->state = TCS_IDLE;
                self->column = 0;
                return TCS_MIN(self->size, self->max_trunc);
        case TCS_TAIL_SPACE:
                self->state = TCS_IDLE;
                self->column = 0;
                return self->size;
        }
        return 0;
}

static uint32_t tcs_default(Regex_truncCommentSpace_t *self) {
        switch(self->state) {
        case TCS_IDLE:
                self->column += 1;
                return 0;
        case TCS_INDENTATION:
                self->state = TCS_IDLE;
                self->column += 1;
                return TCS_MIN(self->size, self->max_trunc);
        case TCS_TAIL_SPACE:
                self->state = TCS_IDLE;
                self->column += 1;
                return 0;
        }
        return 0;
}

uint32_t Regex_truncCommentSpace(
        Regex_truncCommentSpace_t *self, 
        char const chr
) {
        switch(chr) {
        case ' ':
                return tcs_space(self);
        case '\n':
        case 0:
                return tcs_endOfLine(self);
        default:
                return tcs_default(self);
        }
        return 0;
}

#endif
