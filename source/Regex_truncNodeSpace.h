#ifndef REGEX_TRUNCATE_SPACE_NODES_H
#define REGEX_TRUNCATE_SPACE_NODES_H

#include <stdint.h>

typedef enum {
        TNS_IDLE,
        TNS_INDENTATION,
        TNS_TAIL_SPACE,
} Regex_truncNodeSpace_e;

typedef struct {
        Regex_truncNodeSpace_e state;
        uint32_t size;
        uint32_t column;
} Regex_truncNodeSpace_t;

// see diagrams.uxf
void Regex_truncNodeSpace_first(
        Regex_truncNodeSpace_t *self, 
        uint32_t const column_idx
);

uint32_t Regex_truncNodeSpace(
        Regex_truncNodeSpace_t *self, 
        char const chr
);

#endif

#ifdef REGEX_TRUNC_NODE_SPACE_IMPLEMENTATION

void Regex_truncNodeSpace_first(
        Regex_truncNodeSpace_t *self,
        uint32_t const column_idx
) {
        self->state = TNS_IDLE;
        self->column = column_idx;
}

static uint32_t TNS_space(Regex_truncNodeSpace_t *self) {
        switch(self->state) {
        case TNS_IDLE:
                self->size = 1;
                if (self->column == 0) {
                        self->state = TNS_INDENTATION;
                } else {
                        self->state = TNS_TAIL_SPACE;
                }
                return 0;
        case TNS_INDENTATION:
        case TNS_TAIL_SPACE:
                self->size += 1;
                self->column += 1;
                return 0;
        }
        return 0;
}


static uint32_t TNS_endOfLine(Regex_truncNodeSpace_t *self) {
        switch(self->state) {
        case TNS_IDLE:
                self->column = 0;
                return 0; 
        case TNS_INDENTATION:
        case TNS_TAIL_SPACE:
                self->state = TNS_IDLE;
                self->column = 0;
                return self->size;
        }
        return 0;
}

static uint32_t TNS_default(Regex_truncNodeSpace_t *self) {
        switch(self->state) {
                case TNS_IDLE:
                        self->column += 1;
                        return 0; 
                case TNS_INDENTATION:
                        self->state = TNS_IDLE;
                        self->column += 1;
                        return self->size;
                case TNS_TAIL_SPACE:
                        self->state = TNS_IDLE;
                        self->column += 1;
                        return 0;
        }
        return 0;
}

uint32_t Regex_truncNodeSpace( Regex_truncNodeSpace_t *self, char const chr) {
        switch(chr) {
        case ' ':
                return TNS_space(self);
        case '\n':
        case 0:
                return TNS_endOfLine(self);
        default:
                break;
        }
        return TNS_default(self);
}

#endif
