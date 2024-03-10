#ifndef OOO_JOB_H
#define OOO_JOB_H

#include "OStr.h"

typedef struct {
        OStrCursor cursor;
        OStr sink;
        OStr source;
        size_t indentation_level;
} OOO_Job;

#endif
