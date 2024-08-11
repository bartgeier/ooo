#include "Regex_commentOpen.h"

static char const ID[] = "|VB5FNX7iQCFJBz2Ka0mUzYGYgCvtEQ1SNmXPZ54e|";

static void endOfLine(Regex_commentOpen_t *self) {
        switch (self->state) {
                case REGEX1_IDLE:
                case REGEX1_DISCARD:
                        return;
                case REGEX1_CPP:
                case REGEX1_SLASH_0:
                        self->state = REGEX1_IDLE;
                        return;
                case REGEX1_SLASH_1:
                        self->state = REGEX1_IDLE;
                        return;
                case REGEX1_ASTERISK_0:
                case REGEX1_ASTERISK_1:
                case REGEX1_ASTERISK_2:
                case REGEX1_ID_OK:
                        self->state = REGEX1_DISCARD;
                        self->found = false;
                        return;
        }
        return;
}


static void asterisk(Regex_commentOpen_t *self, size_t const idx) {
        switch (self->state) {
        case REGEX1_IDLE:
        case REGEX1_CPP:
        case REGEX1_ASTERISK_2:
                return;
        case REGEX1_SLASH_0:
                self->state = REGEX1_ASTERISK_0;
                self->begin = idx;
                self->found = false;
                self->id_size = 0;
                return;
        case REGEX1_SLASH_1:
                self->state = REGEX1_IDLE;
                return;
        case REGEX1_ASTERISK_0:
                self->state = REGEX1_ASTERISK_1;
                return;
        case REGEX1_ASTERISK_1:
                self->found = false;
                break;
        case REGEX1_DISCARD:
                self->state = REGEX1_ASTERISK_2;
                return;
        case REGEX1_ID_OK:
                self->state = REGEX1_ASTERISK_1;
                self->id_size += 2;
                return;
        }
        return;
}

static void condition(Regex_commentOpen_t *self, char const chr) {
                if (self->id_size >= (sizeof(ID) - 1)) {
                        self->id_size = 0;
                }
                if (chr == ID[self->id_size]) {
                        self->state = REGEX1_ID_OK;
                        self->id_size++;
                        self->found = self->id_size == (sizeof(ID) - 1);
                        return;
                }
                self->state = REGEX1_ASTERISK_0;
                self->id_size = 0;
                self->found = false;
                return;
}

static bool slash(Regex_commentOpen_t *self) {
        switch (self->state) {
        case REGEX1_IDLE:
                self->state = REGEX1_SLASH_0;
                return false;
        case REGEX1_SLASH_0:
                self->state = REGEX1_CPP;
                return false;
        case REGEX1_SLASH_1:
                self->state = REGEX1_SLASH_0;
                return self->found;
        case REGEX1_CPP:
        case REGEX1_DISCARD:
                return false;
        case REGEX1_ASTERISK_1:
                self->state = REGEX1_SLASH_1;
                return false;
        case REGEX1_ASTERISK_2:
                self->state = REGEX1_IDLE;
                return false;
        case REGEX1_ASTERISK_0:
        case REGEX1_ID_OK:
                self->id_size = 0;
                condition(self, '/');
                return false;
        }
        return false;
}

static void default_char(Regex_commentOpen_t *self, char const chr) {
        switch (self->state) {
        case REGEX1_IDLE:
        case REGEX1_CPP:
        case REGEX1_DISCARD:
                return;
        case REGEX1_SLASH_0:
        case REGEX1_SLASH_1:
                self->state = REGEX1_IDLE;
                return;
        case REGEX1_ASTERISK_1:
                self->id_size = 0;
                condition(self, chr);
                return;
        case REGEX1_ASTERISK_0:
        case REGEX1_ID_OK:
                condition(self, chr);
                return;
        case REGEX1_ASTERISK_2:
                self->state = REGEX1_DISCARD;
                return;
        }
        return;
}

bool Regex_commentOpen(Regex_commentOpen_t *self, size_t const idx, char const chr) {
        (void)idx;
        switch(chr) {
        case 0:
        case '\n':
                endOfLine(self);
                return self->found;
        case '/':
                return slash(self);
        case '*':
                asterisk(self, idx);
                return false;
        default:
                default_char(self, chr);
        }
        return false;
}

