#ifndef TRACK_SWITCH
#define TRACK_SWITCH

namespace TRACKSWITCH {
        enum States {    /* visible because unittests */
                INIT,
                SLASH,
                CHAR,
                STRING,
                LINE_COMMENT,
                BLOCK_COMMENT,
                ASTERISK,
                ESC_CHAR,
                ESC_STRING,
        };
};

struct TrackSwitch {
        TRACKSWITCH::States state;
};

void TrackSwitch_init(TrackSwitch &m);
void TrackSwitch_reset(TrackSwitch &m);
char trackSwitch(TrackSwitch &m, char const chr);





#endif
