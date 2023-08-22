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
                ASTERIX,
                ESC_CHAR,
                ESC_STRING,
        };
};

struct TrackSwitch {
        TRACKSWITCH::States state;
};


void TrackSwitch_reset(TrackSwitch &m);
char TrackSwitch_event(TrackSwitch &m, char chr);





#endif
