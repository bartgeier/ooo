#ifndef OOO_JOB_H
#define OOO_JOB_H

#include "OStr.h"

typedef struct {
        uint32_t idx;
        OStr sink;
        OStr source;
} OJob;

void OJob_swap(OJob *m);
void OJob_LF(OJob *m);
void OJob_2LF(OJob *m);
void OJob_space(OJob *m);
void OJob_1_or_2LF(OJob *m, Slice const slice);
void OJob_LF_or_space(OJob *m, Slice const slice);

void OJob_reset_pre_processor_line_continuation();
void OJob_set_pre_processor_line_continuation();
#endif

#ifdef OJOB_IMPLEMENTATION 

static bool pre_processor_line_continuation = false;

void OJob_reset_pre_processor_line_continuation() {
        pre_processor_line_continuation = false;
}

void OJob_set_pre_processor_line_continuation() {
        pre_processor_line_continuation = true;
}

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

void OJob_LF(OJob *m) {
        if (pre_processor_line_continuation) {
                OStr_append_chr(&m->sink, ' ');
                OStr_append_chr(&m->sink, '\\');
                OStr_append_chr(&m->sink, '\n');
                return;
        }
        OStr_append_chr(&m->sink, '\n');
}

void OJob_2LF(OJob *m) {
        if (pre_processor_line_continuation) {
                OStr_append_chr(&m->sink, ' ');
                OStr_append_chr(&m->sink, '\\');
                OStr_append_chr(&m->sink, '\n');
                OStr_append_chr(&m->sink, '\\');
                OStr_append_chr(&m->sink, '\n');
                return;
        }
        OStr_append_number_of_chr(&m->sink, 2, '\n');
}

void OJob_space(OJob *m) {
        OStr_append_chr(&m->sink, ' ');
}

void OJob_1_or_2LF(OJob *m, Slice const slice) {
        uint32_t const num_of_LF = OStr_need_1_or_2LF(&m->source, slice);
        assert(num_of_LF == 1 | num_of_LF == 2);
        if (pre_processor_line_continuation) {
                OStr_append_chr(&m->sink, ' ');
                OStr_append_chr(&m->sink, '\\');
                OStr_append_chr(&m->sink, '\n');
                if (num_of_LF == 2) {
                        OStr_append_chr(&m->sink, '\\');
                        OStr_append_chr(&m->sink, '\n');
                }
                return;
        }
        OStr_append_number_of_chr(&m->sink, num_of_LF, '\n');
}

void OJob_LF_or_space(OJob *m, Slice const slice) {
        if (OStr_last_has_LF(&m->source, slice)) {
                return;
        }
        char const chr = OStr_need_LF_or_space(&m->source, slice);
        if (pre_processor_line_continuation & chr == '\n') {
                OStr_append_chr(&m->sink, ' ');
                OStr_append_chr(&m->sink, '\\');
        }
        OStr_append_chr(&m->sink, chr);
}

#endif
#undef OJOB_IMPLEMENTATION
