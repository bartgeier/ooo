#ifndef REGEX_H
#define REGEX_H

typedef enum {
        RCB1_IDLE,
        RCB1_CPP,
        RCB1_SLASH_0,
        RCB1_SLASH_1,
        RCB1_ASTERISK_0,
        RCB1_ASTERISK_1,
        RCB1_ASTERISK_2,
        RCB1_CHAR,
        RCB1_SPACE,
        RCB1_DISCARD,
} Regex_CommentBlock_1Line_e;

typedef struct {
        Regex_CommentBlock_1Line_e state;
        size_t begin;
        size_t num_of_truncate;
} Regex_CommentBlock_1Line_t;

bool Regex_commentBlock_1line(Regex_CommentBlock_1Line_t *self, size_t const idx, char const chr);

#endif


#ifdef REGEX_IMPLEMENTATION

/* return true if regex found */
static bool endOfLine(Regex_CommentBlock_1Line_t *self) {
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_DISCARD:
                return false;
        case RCB1_CPP:
        case RCB1_SLASH_0:
                self->state = RCB1_IDLE;
                return false;
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                return true;
        case RCB1_ASTERISK_0:
        case RCB1_ASTERISK_1:
        case RCB1_ASTERISK_2:
        case RCB1_CHAR:
        case RCB1_SPACE:
                self->state = RCB1_DISCARD;
                return false;
        }
        return false;
}

static void slash(Regex_CommentBlock_1Line_t *self) {
        switch(self->state) {
        case RCB1_IDLE:
                self->state = RCB1_SLASH_0;
                return;
        case RCB1_SLASH_0:
                self->state = RCB1_CPP;
                return;
        case RCB1_SLASH_1:
                self->state = RCB1_SLASH_0;
                return;
        case RCB1_ASTERISK_0:
        case RCB1_SPACE:
                self->state = RCB1_CHAR;
                self->num_of_truncate = 0;
                return;
        case RCB1_ASTERISK_1:
                self->state = RCB1_SLASH_1;
                self->num_of_truncate += 2;
                return;
        case RCB1_ASTERISK_2:
                self->state = RCB1_IDLE;
                return;
        case RCB1_CPP:
        case RCB1_CHAR:
        case RCB1_DISCARD:
                return;
        }
}

static void asterisk(Regex_CommentBlock_1Line_t *self, size_t const idx) {
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_CPP:
        case RCB1_ASTERISK_2:
                return;
        case RCB1_DISCARD:
                self->state = RCB1_ASTERISK_2;
                return;
        case RCB1_SLASH_0:
                self->state = RCB1_ASTERISK_0;
                self->begin = idx;
                self->num_of_truncate = 0;
                return;
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                return;
        case RCB1_ASTERISK_0:
        case RCB1_SPACE:
        case RCB1_CHAR:
                self->state = RCB1_ASTERISK_1;
                return;
        case RCB1_ASTERISK_1:
                self->num_of_truncate = 0;
                return;
        }
        return;
}

static void spaces(Regex_CommentBlock_1Line_t *self) {
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_CPP:
        case RCB1_DISCARD:
                return;
        case RCB1_ASTERISK_0:
        case RCB1_ASTERISK_1:
        case RCB1_CHAR:
                self->state = RCB1_SPACE;
                self->num_of_truncate = 1;
                return;
        case RCB1_SLASH_0:
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                return;
        case RCB1_SPACE:
                self->num_of_truncate++;
                return;
        case RCB1_ASTERISK_2:
                self->state = RCB1_DISCARD;
                return;
        }
        return;
}

static void default_char(Regex_CommentBlock_1Line_t *self) {
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_CPP:
        case RCB1_DISCARD:
        case RCB1_CHAR:
                return;
        case RCB1_SLASH_0:
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                return;
        case RCB1_ASTERISK_0:
        case RCB1_ASTERISK_1:
        case RCB1_SPACE:
                self->state = RCB1_CHAR;
                return;
        case RCB1_ASTERISK_2:
                self->state = RCB1_DISCARD;
                return;
        }
        return;
}

/* return true if regex found */
bool Regex_commentBlock_1line(Regex_CommentBlock_1Line_t *self, size_t const idx, char const chr) {
        switch(chr) {
        case 0:
        case '\n':
                return endOfLine(self);
        case '/':
                slash(self);
                return false;
        case '*':
                asterisk(self, idx);
                return false;
        case ' ':
                spaces(self);
                return false;
        default:
                default_char(self);
                return false;
        }
        return false;
}

#endif
// #undef REGEX_IMPLEMENTATION
