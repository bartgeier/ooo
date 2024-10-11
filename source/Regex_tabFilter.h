#ifndef REGEX_TAB_FILTER_H
#define REGEX_TAB_FILTER_H

#include "OptionalChar.h"

typedef enum {
        RTF_CHAR,
        RTF_TAB,
} Regex_tabFilter_e;

typedef struct {
        Regex_tabFilter_e state;
} Regex_tabFilter_t;

OptionalChar_t Regex_tabFilter(Regex_tabFilter_t *self, char const chr);

#endif

#ifdef REGEX_TAB_FILTER_IMPLEMENTATION

#ifndef __cplusplus
        #define RTF_RETURN_TYPE (OptionalChar_t)
#else
        #define RTF_RETURN_TYPE
#endif

static OptionalChar_t rtf_tab(Regex_tabFilter_t *self) {
        switch(self->state) {
        case RTF_TAB:
                return RTF_RETURN_TYPE{.ok = false, .chr = 0};
        case RTF_CHAR:
                self->state = RTF_TAB;
                return RTF_RETURN_TYPE{.ok = true, .chr = ' '};
        default:
                break;
        }
        return RTF_RETURN_TYPE{.ok = false, .chr = 0};
}

static OptionalChar_t rtf_default(Regex_tabFilter_t *self, char const chr) {
        switch(self->state) {
        case RTF_TAB:
                self->state = RTF_CHAR;
                return RTF_RETURN_TYPE{.ok = true, .chr = chr};
        case RTF_CHAR:
                return RTF_RETURN_TYPE{.ok = true, .chr = chr};
        default:
                break;
        }
        return RTF_RETURN_TYPE{.ok = false, .chr = 0};
}

OptionalChar_t Regex_tabFilter(Regex_tabFilter_t *self, char const chr) {
        switch(chr) {
        case '\t':
                return rtf_tab(self);
        default:
                break;
        }
        return rtf_default(self, chr);
}

#endif
