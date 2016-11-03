// Unit test for Timer class
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Arduino.h>
#include "Timer.h"

namespace testing {

using testing::StrictMock;

class TimerFixture : public Test {
protected:
    ArduinoMock *arduino;

    TimerFixture() {
        arduino = arduinoMockInstance();
    }

    virtual ~TimerFixture() {
        releaseArduinoMock();
    }

    void helper(void) {}
};

// Raw Timer tests
TEST_F(TimerFixture, TestTimerNotExpired)
{
    InSequence unexpired_timer;
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(0));
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(50));
    Timer *event = new Timer(100);
    ASSERT_FALSE(event->expired());
}

TEST_F(TimerFixture, TestTimerLongExpired)
{
    InSequence long_expired_timer;
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(0));
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(100));
    Timer *event = new Timer(1);
    ASSERT_TRUE(event->expired());
}

TEST_F(TimerFixture, TestTimerJustExpired)
{
    InSequence just_expired_timer;
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(50));
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(150));
    Timer *event = new Timer(100);
    ASSERT_TRUE(event->expired());
}

TEST_F(TimerFixture, TestTimerNotQuiteExpired)
{
    InSequence almost_expired_timer;
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(50));
    EXPECT_CALL(*arduino, millis()).WillOnce(Return(149));
    Timer *event = new Timer(100);
    ASSERT_FALSE(event->expired());
}



}; // namespace

   // Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
