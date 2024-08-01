#ifndef REGEX_H
#define REGEX_H

#ifdef __cplusplus
extern "C" {
#endif

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
        size_t num_of_spaces;
} Regex_CommentBlock_1Line_t;

typedef struct {
        bool found;
        char chr;
} RegexTupel;

RegexTupel Regex_commentBlock_1line(Regex_CommentBlock_1Line_t *self, size_t const idx, char const chr);
 

#ifdef __cplusplus
}
#endif

#endif


#ifdef REGEX_IMPLEMENTAION

static RegexTupel endOfLine(Regex_CommentBlock_1Line_t *self, char const chr) {
        RegexTupel tupel = {
                .found = false,
                .chr = chr,
        };
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_DISCARD:
                return tupel;
        case RCB1_CPP:
        case RCB1_SLASH_0:
                self->state = RCB1_IDLE;
                return tupel;
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                tupel.found = true;
                return tupel;
        case RCB1_ASTERISK_0:
        case RCB1_ASTERISK_1:
        case RCB1_ASTERISK_2:
        case RCB1_CHAR:
        case RCB1_SPACE:
                self->state = RCB1_DISCARD;
                return tupel;
        }
        return tupel;
}

static RegexTupel slash(Regex_CommentBlock_1Line_t *self, char const chr) {
        RegexTupel tupel = {
                .found = false,
                .chr = chr,
        };
        switch(self->state) {
        case RCB1_IDLE:
                self->state = RCB1_SLASH_0;
                return tupel;
        case RCB1_SLASH_0:
                self->state = RCB1_CPP;
                return tupel;
        case RCB1_SLASH_1:
                self->state = RCB1_SLASH_0;
                return tupel;
        case RCB1_ASTERISK_0:
        case RCB1_SPACE:
                self->num_of_spaces = 0;
                self->state = RCB1_CHAR;
                return tupel;
        case RCB1_ASTERISK_1:
                self->state = RCB1_SLASH_1;
                return tupel;
        case RCB1_ASTERISK_2:
                self->state = RCB1_IDLE;
                return tupel;
        case RCB1_CPP:
        case RCB1_CHAR:
        case RCB1_DISCARD:
                return tupel;
        }
        return tupel;
}

static RegexTupel asterisk(Regex_CommentBlock_1Line_t *self, size_t const idx, char const chr) {
        RegexTupel tupel = {
                .found = false,
                .chr = chr,
        };
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_CPP:
        case RCB1_ASTERISK_2:
                return tupel;
        case RCB1_DISCARD:
                self->state = RCB1_ASTERISK_2;
                return tupel;
        case RCB1_SLASH_0:
                self->state = RCB1_ASTERISK_0;
                self->begin = idx;
                self->num_of_spaces = 0;
                return tupel;
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                return tupel;
        case RCB1_ASTERISK_0:
        case RCB1_SPACE:
        case RCB1_CHAR:
                self->state = RCB1_ASTERISK_1;
                return tupel;
        case RCB1_ASTERISK_1:
                self->num_of_spaces = 0;
                return tupel;
        }
        return tupel;
}

static RegexTupel spaces(Regex_CommentBlock_1Line_t *self, char const chr) {
        RegexTupel tupel = {
                .found = false,
                .chr = chr,
        };
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_CPP:
        case RCB1_DISCARD:
                return tupel;
        case RCB1_ASTERISK_0:
        case RCB1_ASTERISK_1:
        case RCB1_CHAR:
                self->state = RCB1_SPACE;
                self->num_of_spaces = 1;
                return tupel;
        case RCB1_SLASH_0:
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                return tupel;
        case RCB1_SPACE:
                self->num_of_spaces++;
                return tupel;
        case RCB1_ASTERISK_2:
                self->state = RCB1_DISCARD;
                return tupel;
        }
        return tupel;
}

static RegexTupel default_char(Regex_CommentBlock_1Line_t *self, char const chr) {
        RegexTupel tupel = {
                .found = false,
                .chr = chr,
        };
        switch(self->state) {
        case RCB1_IDLE:
        case RCB1_CPP:
        case RCB1_DISCARD:
        case RCB1_CHAR:
                return tupel;
        case RCB1_SLASH_0:
        case RCB1_SLASH_1:
                self->state = RCB1_IDLE;
                return tupel;
        case RCB1_ASTERISK_0:
        case RCB1_ASTERISK_1:
        case RCB1_SPACE:
                self->state = RCB1_CHAR;
                return tupel;
        case RCB1_ASTERISK_2:
                self->state = RCB1_DISCARD;
                return tupel;
        }
        return tupel;
}

RegexTupel Regex_commentBlock_1line(Regex_CommentBlock_1Line_t *self, size_t const idx, char const chr) {
        switch(chr) {
        case 0:
        case '\n':
                return endOfLine(self, chr);
        case '/':
                return slash(self, chr);
        case '*':
                return asterisk(self, idx, chr);
        case ' ':
                return spaces(self, chr);
        default:
                return default_char(self, chr);
        }
        RegexTupel tupel = {
                .found = false,
                .chr = chr,
        };
        return tupel;
}

#endif

#undef REGEX_IMPLEMENTAION
