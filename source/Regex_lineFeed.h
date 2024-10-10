#ifndef REGEX_LINE_FEED_H
#define REGEX_LINE_FEED_H

#include "OptionalChar.h"
#include <stdint.h>

typedef enum {
        RLF_CHAR,
        RLF_RETURN,
        RLF_LINE_FEED,
} Regex_lineFeed_e;

typedef struct {
        Regex_lineFeed_e state;
        uint32_t r;
        uint32_t n;
        uint32_t rn;
        uint32_t nr;
} Regex_lineFeed_t;

OptionalChar_t Regex_lineFeed(Regex_lineFeed_t *self, char const chr);
char Regex_lineFeed_last(Regex_lineFeed_t *self);

#endif

#ifdef REGEX_LINE_FEED_IMPLEMENATATION

static OptionalChar_t rlf_return(Regex_lineFeed_t *self) {
        switch (self->state) {
        case RLF_CHAR:
                self->state = RLF_RETURN;
                return { .ok = true, .chr = '\n' };
        case RLF_RETURN:
                self->state = RLF_CHAR;
                self->r += 2;
                return { .ok = true, .chr = '\n' };
        case RLF_LINE_FEED:
                self->state = RLF_CHAR;
                self->nr += 1;
                return { .ok = false, .chr = 0 };
        default:
                break;
        }
        return { .ok = false, .chr = 0 };
}

static OptionalChar_t rlf_line_feed(Regex_lineFeed_t *self) {
        switch (self->state) {
        case RLF_CHAR:
                self->state = RLF_LINE_FEED;
                return { .ok = true, .chr = '\n' };
        case RLF_RETURN:
                self->state = RLF_CHAR;
                self->rn += 1;
                return { .ok = false, .chr = 0 };
        case RLF_LINE_FEED:
                self->state = RLF_CHAR;
                self->n += 2;
                return { .ok = true, .chr = '\n' };
        default:
                break;
        }
        return { .ok = false, .chr = 0 };
}

static OptionalChar_t rlf_default(Regex_lineFeed_t *self, char const chr) {
        switch (self->state) {
        case RLF_CHAR:
                return { .ok = true, .chr = chr };
        case RLF_RETURN:
                self->state = RLF_CHAR;
                self->r += 1;
                return { .ok = true, .chr = chr };
        case RLF_LINE_FEED:
                self->state = RLF_CHAR;
                self->n += 1;
                return { .ok = true, .chr = chr };
        default:
                break;
        }
        return { .ok = false, .chr = 0 };
}

OptionalChar_t Regex_lineFeed(Regex_lineFeed_t *self, char const chr) {
        switch(chr) {
                case '\r': return rlf_return(self);
                case '\n': return rlf_line_feed(self);
                default: break;
        }
        return rlf_default(self, chr);
}

char Regex_lineFeed_last(Regex_lineFeed_t *self) {
        self->state = RLF_CHAR;
        uint32_t max = self->r;
        if (self->n > max) max = self->n;
        if (self->rn > max) max = self->rn;
        if (self->nr > max) max = self->nr;
        if (max == self->r) return 'r';
        if (max == self->n) return 'n';
        if (max == self->rn) return 'R';
        if (max == self->nr) return 'N';
        assert(false);
        return 0;
}

#endif
