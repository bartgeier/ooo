#ifndef OOO_JOB_H
#define OOO_JOB_H

#include "OStr.h"

typedef struct {
        OStrCursor cursor;
        OStr sink;
        OStr source;
        size_t indentation_level;
} OJob;

void OJob_LF(OJob *m, Slice const slice);
void OJob_2LF(OJob *m, Slice const slice);
void OJob_space(OJob *m);
void OJob_1_or_2LF(OJob *m, Slice const slice);
void OJob_LF_or_space(OJob *m, Slice const slice);

#endif

#ifdef OJOB_IMPLEMENTATION 

void OJob_LF(OJob *m, Slice const slice) {
        size_t const num_of_LF = OStr_need_1LF(&m->source, slice);
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
        char const chr = OStr_need_1LF_or_1Space(&m->source, slice);
        OStr_append_chr(&m->sink, chr);
}


#endif
#undef OJOB_IMPLEMENTATION
