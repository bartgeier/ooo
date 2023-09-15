#include "Codizes.h"
#include <assert.h>

using namespace CODIZES;

static void set(Gold::Codizes &m, CODIZES::States state) {
        m.state = state;
}

void Codizes_init(Gold::Codizes &m) {
        m.state = INIT;
}

void Codizes_reset(Gold::Codizes &m) {
        Codizes_init(m);
}

static char apostroph(Gold::Codizes &m) {
        switch (m.state) {
        case INIT:
        case SLASH:
                set(m, CHAR);
                return '\'';
        case CHAR:
                set(m, INIT);
                return '\'';
        case STRING:
        case LINE_COMMENT:
        case BLOCK_COMMENT:
        case ASTERISK:
        case ESC_CHAR:
        case ESC_STRING:
                return 'o';
        }
        assert(false);
}

static char quotation_marks(Gold::Codizes &m) {
        switch (m.state) {
        case INIT:
        case SLASH:
                set(m, STRING);
                return '"';
        case STRING:
                set(m, INIT);
                return '"';
        case CHAR:
        case LINE_COMMENT:
        case BLOCK_COMMENT:
        case ASTERISK:
        case ESC_CHAR:
        case ESC_STRING:
                return 'o';
        }
        assert(false);
}

static char slash(Gold::Codizes &m) {
        switch (m.state) {
        case INIT:
                set(m, SLASH);
                return '/';
        case SLASH:
                set(m, LINE_COMMENT);
                return '/';
        case CHAR:
                set(m, ESC_CHAR);
                return 'o';
        case STRING:
                set(m, ESC_STRING);
                return 'o';
        case LINE_COMMENT:
        case BLOCK_COMMENT:
                return 'o';
        case ASTERISK:
                set(m, INIT);
                return '/';
        case ESC_CHAR:
                set(m, CHAR);
                return 'o';
        case ESC_STRING:
                set(m, STRING);
                return 'o';
        }
        assert(false);
}

static char asterisk(Gold::Codizes &m) {
        switch (m.state) {
        case INIT:
                return '*';
        case SLASH:
                set(m, BLOCK_COMMENT);
                return '*';
        case CHAR:
        case STRING:
        case LINE_COMMENT:
                return 'o';
        case BLOCK_COMMENT:
                set(m, ASTERISK);
                return 'o';
        case ASTERISK:
                return 'o';
        case ESC_CHAR:
                set(m, CHAR);
                return 'o';
        case ESC_STRING:
                set(m, STRING);
                return 'o';
        }
        assert(false);
}

static char line_feed(Gold::Codizes &m) {
        switch (m.state) {
        case INIT:
                return '\n';
        case SLASH:
        case LINE_COMMENT:
                set(m, INIT);
                return '\n';
        case CHAR:
        case STRING:
        case BLOCK_COMMENT:
                return 'o';
        case ASTERISK:
                set(m, BLOCK_COMMENT);
                return 'o';
        case ESC_CHAR:
                set(m, CHAR);
                return 'o';
        case ESC_STRING:
                set(m, STRING);
                return 'o';
        }
        assert(false);
}

static char character(Gold::Codizes &m, char const chr) {
        switch (m.state) {
        case INIT:
        case SLASH:
                return chr;
        case CHAR:
        case STRING:
        case LINE_COMMENT:
        case BLOCK_COMMENT:
                return 'o';
        case ASTERISK:
                set(m, BLOCK_COMMENT);
                return 'o';
        case ESC_CHAR:
                set(m, CHAR);
                return 'o';
        case ESC_STRING:
                set(m, STRING);
                return 'o';
        }
        assert(false);
}

char codizes(Gold::Codizes &m, char const chr) {
        switch (chr) {
        case '\'':
                return apostroph(m);
        case '"':
                return quotation_marks(m);
        case '/':
                return slash(m);
        case '*':
                return asterisk(m);
        case '\n':
                return line_feed(m);
        default:
                return character(m, chr);
        };
        assert(false);
}
