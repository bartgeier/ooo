#include <gtest/gtest.h>
#include "TrackSwitch.h"
/* https://www.thepunctuationguide.com/parentheses.html */

using namespace TRACKSWITCH;

TEST(TrackSwitch, init) {
        TrackSwitch x;
        TrackSwitch_init(x);
        EXPECT_EQ(x.state, INIT);
        EXPECT_EQ(trackSwitch(x, 'i'), 'i');

}

TEST(TrackSwitch, reset) {
        TrackSwitch x;
        TrackSwitch_reset(x);
        EXPECT_EQ(x.state, INIT);
        EXPECT_EQ(trackSwitch(x, 'i'), 'i');
}

TEST(TrackSwitch, apostrophe) {
        TrackSwitch x;
        x.state = INIT;
        EXPECT_EQ(trackSwitch(x, '\''), '\'');
        EXPECT_EQ(x.state, CHAR);

        x.state = SLASH;
        EXPECT_EQ(trackSwitch(x, '\''), '\'');
        EXPECT_EQ(x.state, CHAR);

        x.state = CHAR;
        EXPECT_EQ(trackSwitch(x, '\''), '\'');
        EXPECT_EQ(x.state, INIT);

        x.state = STRING;
        EXPECT_EQ(trackSwitch(x, '\''), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(trackSwitch(x, '\''), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(trackSwitch(x, '\''), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(trackSwitch(x, '\''), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ESC_CHAR;
        EXPECT_EQ(trackSwitch(x, '\''), 'o');
        EXPECT_EQ(x.state, ESC_CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(trackSwitch(x, '\''), 'o');
        EXPECT_EQ(x.state, ESC_STRING);
}

TEST(TrackSwitch, quotation_marks) {
        TrackSwitch x;
        x.state = INIT;
        EXPECT_EQ(trackSwitch(x, '"'), '"');
        EXPECT_EQ(x.state, STRING);

        x.state = SLASH;
        EXPECT_EQ(trackSwitch(x, '"'), '"');
        EXPECT_EQ(x.state, STRING);

        x.state = CHAR;
        EXPECT_EQ(trackSwitch(x, '"'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(trackSwitch(x, '"'), '"');
        EXPECT_EQ(x.state, INIT);

        x.state = LINE_COMMENT;
        EXPECT_EQ(trackSwitch(x, '"'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(trackSwitch(x, '"'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(trackSwitch(x, '"'), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ESC_CHAR;
        EXPECT_EQ(trackSwitch(x, '"'), 'o');
        EXPECT_EQ(x.state, ESC_CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(trackSwitch(x, '"'), 'o');
        EXPECT_EQ(x.state, ESC_STRING);
}

TEST(TrackSwitch, slash) {
        TrackSwitch x;
        x.state = INIT;
        EXPECT_EQ(trackSwitch(x, '/'), '/');
        EXPECT_EQ(x.state, SLASH);

        x.state = SLASH;
        EXPECT_EQ(trackSwitch(x, '/'), '/');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = CHAR;
        EXPECT_EQ(trackSwitch(x, '/'), 'o');
        EXPECT_EQ(x.state, ESC_CHAR);

        x.state = STRING;
        EXPECT_EQ(trackSwitch(x, '/'), 'o');
        EXPECT_EQ(x.state, ESC_STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(trackSwitch(x, '/'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(trackSwitch(x, '/'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(trackSwitch(x, '/'), '/');
        EXPECT_EQ(x.state, INIT);

        x.state = ESC_CHAR;
        EXPECT_EQ(trackSwitch(x, '/'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(trackSwitch(x, '/'), 'o');
        EXPECT_EQ(x.state, STRING);
}

TEST(TrackSwitch, asterisk) {
        TrackSwitch x;
        x.state = INIT;
        EXPECT_EQ(trackSwitch(x, '*'), '*');
        EXPECT_EQ(x.state, INIT);

        x.state = SLASH;
        EXPECT_EQ(trackSwitch(x, '*'), '*');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = CHAR;
        EXPECT_EQ(trackSwitch(x, '*'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(trackSwitch(x, '*'), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(trackSwitch(x, '*'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(trackSwitch(x, '*'), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ASTERISK;
        EXPECT_EQ(trackSwitch(x, '*'), 'o');
        EXPECT_EQ(x.state, ASTERISK);

        x.state = ESC_CHAR;
        EXPECT_EQ(trackSwitch(x, '*'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(trackSwitch(x, '*'), 'o');
        EXPECT_EQ(x.state, STRING);
}

TEST(TrackSwitch, line_feed) {
        TrackSwitch x;
        x.state = INIT;
        EXPECT_EQ(trackSwitch(x, '\n'), '\n');
        EXPECT_EQ(x.state, INIT);

        x.state = SLASH;
        EXPECT_EQ(trackSwitch(x, '\n'), '\n');
        EXPECT_EQ(x.state, INIT);

        x.state = CHAR;
        EXPECT_EQ(trackSwitch(x, '\n'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(trackSwitch(x, '\n'), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(trackSwitch(x, '\n'), '\n');
        EXPECT_EQ(x.state, INIT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(trackSwitch(x, '\n'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(trackSwitch(x, '\n'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ESC_CHAR;
        EXPECT_EQ(trackSwitch(x, '\n'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(trackSwitch(x, '\n'), 'o');
        EXPECT_EQ(x.state, STRING);
}

TEST(TrackSwitch, character) {
        TrackSwitch x;
        x.state = INIT;
        EXPECT_EQ(trackSwitch(x, 'g'), 'g');
        EXPECT_EQ(x.state, INIT);

        x.state = SLASH;
        EXPECT_EQ(trackSwitch(x, ' '), ' ');
        EXPECT_EQ(x.state, SLASH);

        x.state = CHAR;
        EXPECT_EQ(trackSwitch(x, 'b'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = STRING;
        EXPECT_EQ(trackSwitch(x, 'i'), 'o');
        EXPECT_EQ(x.state, STRING);

        x.state = LINE_COMMENT;
        EXPECT_EQ(trackSwitch(x, 's'), 'o');
        EXPECT_EQ(x.state, LINE_COMMENT);

        x.state = BLOCK_COMMENT;
        EXPECT_EQ(trackSwitch(x, 'i'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ASTERISK;
        EXPECT_EQ(trackSwitch(x, 'y'), 'o');
        EXPECT_EQ(x.state, BLOCK_COMMENT);

        x.state = ESC_CHAR;
        EXPECT_EQ(trackSwitch(x, 'y'), 'o');
        EXPECT_EQ(x.state, CHAR);

        x.state = ESC_STRING;
        EXPECT_EQ(trackSwitch(x, 'q'), 'o');
        EXPECT_EQ(x.state, STRING);
}
