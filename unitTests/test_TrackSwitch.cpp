#include <gtest/gtest.h>
#include "TrackSwitch.h"

using namespace TRACKSWITCH;

TEST(test, a) {
        TrackSwitch x;
        TrackSwitch_reset(x);
        EXPECT_EQ(x.state, INIT);
        EXPECT_EQ(TrackSwitch_event(x, 'i'), 'i');
}
