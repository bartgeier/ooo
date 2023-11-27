#ifndef CURSOR_H
#define CURSOR_H

#include "tree_sitter/api.h"
#include <stdlib.h>
#include "StringView.h"

typedef struct {
        size_t row;
        size_t column;
        size_t idx;
} Cursor;

void Cursor_reset(Cursor *m);
size_t Cursor_move_to_point(Cursor *m, StringView const *s, TSPoint const p);
bool Cursor_increment(Cursor *m, StringView const *s);
bool Cursor_decrement(Cursor *m, StringView const *s);

#endif

//#define CURSOR_IMPLEMENTATION
#ifdef CURSOR_IMPLEMENTATION

void Cursor_reset(Cursor *m) {
        m->idx = 0;
        m->column = 0;
        m->row = 0;
}

static size_t column(Cursor *m, StringView const *s) {
        size_t count = 1;
        while ((m->idx - count > 0) & (s->at[m->idx - count] != '\n')) {
                count++;
        }
        return count--;
}

size_t Cursor_move_to_point(Cursor *m, StringView const *s, TSPoint const p) {
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

bool Cursor_increment(Cursor *m, StringView const *s) {
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


bool Cursor_decrement(Cursor *m, StringView const *s) {
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
#undef CURSOR_IMPLEMENTATION

