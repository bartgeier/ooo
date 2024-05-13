#ifndef OOO_JOB_H
#define OOO_JOB_H

#include "OStr.h"

typedef struct {
        size_t idx;
        OStr sink;
        OStr source;
} OJob;

void OJob_swap(OJob *m);
size_t OJob_cursor(OJob *m, size_t const idx);
void OJob_LF(OJob *m, Slice const slice);
void OJob_2LF(OJob *m, Slice const slice);
void OJob_space(OJob *m);
void OJob_1_or_2LF(OJob *m, Slice const slice);
void OJob_LF_or_space(OJob *m, Slice const slice);

#endif

#ifdef OJOB_IMPLEMENTATION 

void OJob_swap(OJob *m) {
        m->idx = 0;
        m->sink.capacity ^= m->source.capacity;
        m->source.capacity ^= m->sink.capacity;
        m->sink.capacity ^= m->source.capacity;

        m->sink.size ^= m->source.size;
        m->source.size ^= m->sink.size;
        m->sink.size ^= m->source.size;

        char *p = m->sink.at;
        m->sink.at = m->source.at;
        m->source.at = p;

        OStr_clear(&m->sink);
}

size_t OJob_cursor(OJob *m, size_t const idx) {
        m->idx = idx;
        return m->idx;
}

void OJob_LF(OJob *m, Slice const slice) {
        size_t const num_of_LF = OStr_need_LF(&m->source, slice);
        OStr_append_number_of_chr(&m->sink, num_of_LF, '\n');
}

void OJob_2LF(OJob *m, Slice const slice) {
        size_t const num_of_LF = OStr_need_2LF(&m->source, slice);
        OStr_append_number_of_chr(&m->sink, num_of_LF, '\n');
}


void OJob_space(OJob *m) {
        OStr_append_chr(&m->sink, ' ');
}

void OJob_1_or_2LF(OJob *m, Slice const slice) {
        size_t const num_of_LF = OStr_need_1_or_2LF(&m->source, slice);
        OStr_append_number_of_chr(&m->sink, num_of_LF, '\n');
}

void OJob_LF_or_space(OJob *m, Slice const slice) {
        if (OStr_last_has_LF(&m->source, slice)) {
                return;
        }
        char const chr = OStr_need_LF_or_space(&m->source, slice);
        OStr_append_chr(&m->sink, chr);
}


#endif
#undef OJOB_IMPLEMENTATION
