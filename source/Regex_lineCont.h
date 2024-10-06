#ifndef REGEX_LINE_CONT_H
#define REGEX_LINE_CONT_H

#include <stdint.h>

typedef enum {
        LCO_SPACE,
        LCO_CHAR,
        LCO_BACK_SLASH,
        LCO_LINE_FEED,
} Regex_lineCont_e;

typedef struct {
        Regex_lineCont_e state;
} Regex_lineCont_t;

// see diagrams.uxf
void Regex_lineCont_first(Regex_lineCont_t *self, char const chr);
uint32_t Regex_lineCont(Regex_lineCont_t *self, char const chr);

#ifdef REGEX_LINE_CONT_IMPLEMENTATION 

#include <assert.h>

// chr = string[idx - 1]
// watching one before the slice
void Regex_lineCont_first(Regex_lineCont_t *self, char const chr) {
        if (chr == 0 | chr == '\n' | chr == ' ') {
                self->state = LCO_SPACE;
                return;
        }
        self->state = LCO_CHAR;
        return;
}

static uint32_t lco_space_or_endOfStr(Regex_lineCont_t *self) {
        switch(self->state) {
        case LCO_SPACE:
                return 0;
        case LCO_CHAR:
        case LCO_BACK_SLASH:
        case LCO_LINE_FEED:
                self->state = LCO_SPACE;
                return 0;
        default:
                assert(false);
                break;
        }
        return 0;
}

static uint32_t lco_backSlash(Regex_lineCont_t *self) {
        switch(self->state) {
        case LCO_SPACE:
                self->state = LCO_CHAR;
                return 0;
        case LCO_CHAR:
                self->state = LCO_BACK_SLASH;
                return 0;
        case LCO_BACK_SLASH:
                return 0;
        case LCO_LINE_FEED:
                self->state = LCO_BACK_SLASH;
                return 2;
        default:
                assert(false);
        }
        return 0;
}

static uint32_t lco_lineFeed(Regex_lineCont_t *self) {
        switch(self->state) {
        case LCO_SPACE:
                return 0;
        case LCO_CHAR:
                self->state = LCO_SPACE;
                return 0;
        case LCO_BACK_SLASH:
                self->state = LCO_LINE_FEED;
                return 0;
        case LCO_LINE_FEED:
                self->state = LCO_SPACE;
                return 0;
        default:
                assert(false);
        }
        return 0;
}

static uint32_t lco_default(Regex_lineCont_t *self) {
        switch(self->state) {
        case LCO_SPACE:
                self->state = LCO_CHAR;
                return 0;
        case LCO_CHAR:
                return 0;
        case LCO_BACK_SLASH:
                self->state = LCO_CHAR;
                return 0;
        case LCO_LINE_FEED:
                self->state = LCO_CHAR;
                return 2;
        default:
                assert(false);
        }
        return 0;
}

uint32_t Regex_lineCont(Regex_lineCont_t *self, char const chr) {
        switch(chr) {
        case 0: 
        case ' ': return lco_space_or_endOfStr(self);
        case '\n': return lco_lineFeed(self);
        case '\\': return lco_backSlash(self);
        }
        return lco_default(self);
}

#endif
#endif
