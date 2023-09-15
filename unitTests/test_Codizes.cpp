#include <gtest/gtest.h>
#include "Gold.h"
#include "Codizes.h"
/* https://www.thepunctuationguide.com/parentheses.html */

using namespace CODIZES;

TEST(Codizes, init) {
        Gold::Gold::Codizes x;
        Codizes_init(x);
        EXPECT_EQ(x.state, INIT);
        EXPECT_EQ(codizes(x, 'i'), 'i');

}

TEST(Codizes, reset) {
        Gold::Codizes x;
        Codizes_reset(x);
        EXPECT_EQ(x.state, INIT);
        EXPECT_EQ(codizes(x, 'i'), 'i');
}

TEST(Codizes, apostrophe) {
        Gold::Codizes x;
        x.state = INIT;
        EXPECT_EQ(codizes(x, '\''), '\'');
        EXPECT_EQ(x.state, CHAR);

        x.state = SLASH;
        EXPECT_EQ(codizes(x, '\''), '\'');
        EXPECT_EQ(x.state, CHAR);

        x.state = CHAR;
        EXPECT_EQ(codizes(x, '\''), '\'');
        EXPECT_EQ(x.state, INIT);

        x.state = STRING;
        EXPECT_EQ(codizes(x, '\''), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(codizes(x, '\''), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(codizes(x, '\''), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(codizes(x, '\''), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ESC_CHAR;
        EXPECT_EQ(codizes(x, '\''), 'o');
        EXPECT_EQ(x.state, ESC_CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(codizes(x, '\''), 'o');
        EXPECT_EQ(x.state, ESC_STRING);
}

TEST(Codizes, quotation_marks) {
        Gold::Codizes x;
        x.state = INIT;
        EXPECT_EQ(codizes(x, '"'), '"');
        EXPECT_EQ(x.state, STRING);

        x.state = SLASH;
        EXPECT_EQ(codizes(x, '"'), '"');
        EXPECT_EQ(x.state, STRING);

        x.state = CHAR;
        EXPECT_EQ(codizes(x, '"'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(codizes(x, '"'), '"');
        EXPECT_EQ(x.state, INIT);

        x.state = LINE_COMMENT;
        EXPECT_EQ(codizes(x, '"'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(codizes(x, '"'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(codizes(x, '"'), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ESC_CHAR;
        EXPECT_EQ(codizes(x, '"'), 'o');
        EXPECT_EQ(x.state, ESC_CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(codizes(x, '"'), 'o');
        EXPECT_EQ(x.state, ESC_STRING);
}

TEST(Codizes, slash) {
        Gold::Codizes x;
        x.state = INIT;
        EXPECT_EQ(codizes(x, '/'), '/');
        EXPECT_EQ(x.state, SLASH);

        x.state = SLASH;
        EXPECT_EQ(codizes(x, '/'), '/');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = CHAR;
        EXPECT_EQ(codizes(x, '/'), 'o');
        EXPECT_EQ(x.state, ESC_CHAR);

        x.state = STRING;
        EXPECT_EQ(codizes(x, '/'), 'o');
        EXPECT_EQ(x.state, ESC_STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(codizes(x, '/'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(codizes(x, '/'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(codizes(x, '/'), '/');
        EXPECT_EQ(x.state, INIT);

        x.state = ESC_CHAR;
        EXPECT_EQ(codizes(x, '/'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(codizes(x, '/'), 'o');
        EXPECT_EQ(x.state, STRING);
}

TEST(Codizes, asterisk) {
        Gold::Codizes x;
        x.state = INIT;
        EXPECT_EQ(codizes(x, '*'), '*');
        EXPECT_EQ(x.state, INIT);

        x.state = SLASH;
        EXPECT_EQ(codizes(x, '*'), '*');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = CHAR;
        EXPECT_EQ(codizes(x, '*'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(codizes(x, '*'), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(codizes(x, '*'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(codizes(x, '*'), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ASTERISK;
        EXPECT_EQ(codizes(x, '*'), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ESC_CHAR;
        EXPECT_EQ(codizes(x, '*'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(codizes(x, '*'), 'o');
        EXPECT_EQ(x.state, STRING);
}

TEST(Codizes, line_feed) {
        Gold::Codizes x;
        x.state = INIT;
        EXPECT_EQ(codizes(x, '\n'), '\n');
        EXPECT_EQ(x.state, INIT);

        x.state = SLASH;
        EXPECT_EQ(codizes(x, '\n'), '\n');
        EXPECT_EQ(x.state, INIT);

        x.state = CHAR;
        EXPECT_EQ(codizes(x, '\n'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(codizes(x, '\n'), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(codizes(x, '\n'), '\n');
        EXPECT_EQ(x.state, INIT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(codizes(x, '\n'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(codizes(x, '\n'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ESC_CHAR;
        EXPECT_EQ(codizes(x, '\n'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(codizes(x, '\n'), 'o');
        EXPECT_EQ(x.state, STRING);
}

TEST(Codizes, character) {
        Gold::Codizes x;
        x.state = INIT;
        EXPECT_EQ(codizes(x, 'g'), 'g');
        EXPECT_EQ(x.state, INIT);

        x.state = SLASH;
        EXPECT_EQ(codizes(x, ' '), ' ');
        EXPECT_EQ(x.state, SLASH);

        x.state = CHAR;
        EXPECT_EQ(codizes(x, 'b'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(codizes(x, 'i'), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(codizes(x, 's'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(codizes(x, 'i'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(codizes(x, 'y'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ESC_CHAR;
        EXPECT_EQ(codizes(x, 'y'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(codizes(x, 'q'), 'o');
        EXPECT_EQ(x.state, STRING);
}
