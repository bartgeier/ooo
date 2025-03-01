#ifndef REGEX_SIGNED_COMMENT_H
#define REGEX_SIGNED_COMMENT_H

#include <stdint.h>
#include <stdbool.h>

#define REGEX_SIGNED_COMMENT_ID "|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|"

typedef enum {
        RSC_IDLE,
        RSC_CPP,
        RSC_SLASH_0,
        RSC_SLASH_1,
        RSC_ASTERISK_0,
        RSC_ASTERISK_1,
        RSC_ASTERISK_2,
        RSC_ID_OK,
        RSC_DISCARD,
} Regex_signedComment_e;

typedef struct {
        Regex_signedComment_e state;
        bool found;
        uint32_t begin;
        uint32_t id_size;
} Regex_signedComment_t;

bool Regex_signedComment(Regex_signedComment_t *self, uint32_t const idx, char const chr);

#endif

#ifdef REGEX_SIGNED_COMMENT_IMPLEMENTATION

static char const ID[] = REGEX_SIGNED_COMMENT_ID;

static bool rsc_endOfLine(Regex_signedComment_t *self) {
        switch (self->state) {
        case RSC_IDLE:
        case RSC_DISCARD:
                return false;
        case RSC_CPP:
        case RSC_SLASH_0:
                self->state = RSC_IDLE;
                return false;
        case RSC_SLASH_1:
                self->state = RSC_IDLE;
                return self->found;
        case RSC_ASTERISK_0:
        case RSC_ASTERISK_1:
        case RSC_ASTERISK_2:
        case RSC_ID_OK:
                self->state = RSC_DISCARD;
                self->found = false;
                return false;
        }
        return false;
}


static bool rsc_asterisk(Regex_signedComment_t *self, uint32_t const idx) {
        switch (self->state) {
        case RSC_IDLE:
        case RSC_CPP:
        case RSC_ASTERISK_2:
                return false;
        case RSC_SLASH_0:
                self->state = RSC_ASTERISK_0;
                self->begin = idx;
                self->found = false;
                self->id_size = 0;
                return false;
        case RSC_SLASH_1:
                self->state = RSC_IDLE;
                return self->found;
        case RSC_ASTERISK_0:
                self->state = RSC_ASTERISK_1;
                return false;
        case RSC_ASTERISK_1:
                self->found = false;
                return false;
        case RSC_DISCARD:
                self->state = RSC_ASTERISK_2;
                return false;
        case RSC_ID_OK:
                self->state = RSC_ASTERISK_1;
                self->id_size += 2;
                return false;
        }
        return false;
}

static void rsc_condition(Regex_signedComment_t *self, char const chr) {
        if (self->id_size >= (sizeof(ID) - 1)) {
                self->id_size = 0;
        }
        if (chr == ID[self->id_size]) {
                self->state = RSC_ID_OK;
                self->id_size++;
                self->found = self->id_size == (sizeof(ID) - 1);
                // sizeOf(ID)-1 == strlen(ID)
                return;
        }
        self->state = RSC_ASTERISK_0;
        self->id_size = 0;
        self->found = false;
        return;
}

static bool rsc_slash(Regex_signedComment_t *self) {
        switch (self->state) {
        case RSC_IDLE:
                self->state = RSC_SLASH_0;
                return false;
        case RSC_SLASH_0:
                self->state = RSC_CPP;
                return false;
        case RSC_SLASH_1:
                self->state = RSC_SLASH_0;
                return self->found;
        case RSC_CPP:
        case RSC_DISCARD:
                return false;
        case RSC_ASTERISK_1:
                self->state = RSC_SLASH_1;
                return false;
        case RSC_ASTERISK_2:
                self->state = RSC_IDLE;
                return false;
        case RSC_ASTERISK_0:
        case RSC_ID_OK:
                self->id_size = 0;
                rsc_condition(self, '/');
                return false;
        }
        return false;
}

static bool rsc_default_char(Regex_signedComment_t *self, char const chr) {
        switch (self->state) {
        case RSC_IDLE:
        case RSC_CPP:
        case RSC_DISCARD:
                return false;
        case RSC_SLASH_0:
                self->state = RSC_IDLE;
                return false;
        case RSC_SLASH_1:
                self->state = RSC_IDLE;
                return self->found;
        case RSC_ASTERISK_1:
                self->id_size = 0;
                rsc_condition(self, chr);
                return false;
        case RSC_ASTERISK_0:
        case RSC_ID_OK:
                rsc_condition(self, chr);
                return false;
        case RSC_ASTERISK_2:
                self->state = RSC_DISCARD;
                return false;
        }
        return false;
}

bool Regex_signedComment(Regex_signedComment_t *self, uint32_t const idx, char const chr) {
        (void)idx;
        switch(chr) {
        case 0:
        case '\n':
                return rsc_endOfLine(self);
        case '/':
                return rsc_slash(self);
        case '*':
                return rsc_asterisk(self, idx);
        default:
                break;
        }
        return rsc_default_char(self, chr);
}

#endif
