#include "TrackSwitch.h"

using namespace TRACKSWITCH;

void TrackSwitch_reset(TrackSwitch &m) {
        m.state = INIT;
}

char TrackSwitch_event(TrackSwitch &m, char chr) {
        return chr;
}
