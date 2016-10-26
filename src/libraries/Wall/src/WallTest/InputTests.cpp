// Input device tests for Wall object
//
#include "Wallfixture.h"

namespace testing {

class SwitchFixture : public WallFixture, public ::testing::WithParamInterface<toggle_switch> {
};
TEST_P(SwitchFixture, TestReadSwitchOn)
{
    toggle_switch toggle = GetParam();

    InSequence is_switch_on;
    expectMultiplexerSelectsSX1509(INPUT_TOGGLE_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_TOGGLE_I2C_DEVICE,
        WallImplementation::toggleSwitchPin(toggle), LOW);
    ASSERT_TRUE(wall->isToggleOn(toggle));
}
TEST_P(SwitchFixture, TestReadSwitchOff)
{
    toggle_switch toggle = GetParam();

    InSequence is_switch_off;
    expectMultiplexerSelectsSX1509(INPUT_TOGGLE_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_TOGGLE_I2C_DEVICE,
        WallImplementation::toggleSwitchPin(toggle), HIGH);
    ASSERT_FALSE(wall->isToggleOn(toggle));
}
INSTANTIATE_TEST_CASE_P(ToggleSwitchTests, SwitchFixture, Values(
    LEFT_TOGGLE,
    CENTER_TOGGLE,
    RIGHT_TOGGLE)
);

class JoystickFixture : public WallFixture {
};
TEST_F(JoystickFixture, TestReadJoystickIsUp)
{
    InSequence is_joystick_up;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_UP, LOW);
    ASSERT_TRUE(wall->isJoystickUp());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotUp)
{
    InSequence is_joystick_up;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_UP, HIGH);
    ASSERT_FALSE(wall->isJoystickUp());
}
TEST_F(JoystickFixture, TestReadJoystickIsDown)
{
    InSequence is_joystick_down;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_DOWN, LOW);
    ASSERT_TRUE(wall->isJoystickDown());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotDown)
{
    InSequence is_joystick_down;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_DOWN, HIGH);
    ASSERT_FALSE(wall->isJoystickDown());
}
TEST_F(JoystickFixture, TestReadJoystickIsLeft)
{
    InSequence is_joystick_left;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_LEFT, LOW);
    ASSERT_TRUE(wall->isJoystickLeft());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotLeft)
{
    InSequence is_joystick_left;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_LEFT, HIGH);
    ASSERT_FALSE(wall->isJoystickLeft());
}
TEST_F(JoystickFixture, TestReadJoystickIsRight)
{
    InSequence is_joystick_right;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_RIGHT, LOW);
    ASSERT_TRUE(wall->isJoystickRight());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotRight)
{
    InSequence is_joystick_right;
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_READ(INPUT_JOYSTICK_I2C_DEVICE, INPUT_JOYSTICK_RIGHT, HIGH);
    ASSERT_FALSE(wall->isJoystickRight());
}

class PotentiometerFixture : public WallFixture {
};

TEST_F(PotentiometerFixture, TestReadKnobValue)
{
    uint16_t knobPosition = 147;
    InSequence read_knob;
    expectMultiplexerSelectsADS1015(INPUT_ROTARY_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_ROTARY_POT_I2C_DEVICE, INPUT_ROTARY_POT, knobPosition);
    ASSERT_EQ(wall->getKnobPosition(), knobPosition);
}
// When knob is moved all the way left or right, the contact becomes
// disengaged from the track, resulting in a maximum value that is not
// acheivable, even when far right.  If this value is detected, the result
// must a be safe value that is the extreme value for the left side.
TEST_F(PotentiometerFixture, TestRejectIllegalKnobLeftValue)
{
    uint16_t knobPositionTooFarLeft = MAXIMUM_12BIT_VALUE;

    InSequence read_knob;
    expectMultiplexerSelectsADS1015(INPUT_ROTARY_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_ROTARY_POT_I2C_DEVICE, INPUT_ROTARY_POT, knobPositionTooFarLeft);

    ASSERT_EQ(wall->getKnobPosition(), INPUT_ROTARY_POT_LEFT_LIMIT);
}

TEST_F(PotentiometerFixture, TestReadSliderValue)
{
    uint16_t sliderPosition = 926;
    InSequence read_slider;
    expectMultiplexerSelectsADS1015(INPUT_LINEAR_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_LINEAR_POT_I2C_DEVICE, INPUT_LINEAR_POT, sliderPosition);
    ASSERT_EQ(wall->getSliderPosition(), sliderPosition);
}
// When slider is moved all the way left or right, the contact becomes
// disengaged from the track, resulting in a maximum value that is not
// acheivable, even when far right.  If this value is detected, the result
// must a be safe value that is the extreme vale for the side last observed.
TEST_F(PotentiometerFixture, TestRejectIllegalSliderLeftValue)
{
    uint16_t sliderPositionTooFarLeft = MAXIMUM_12BIT_VALUE;
    uint16_t sliderPositionSmallLeftValue = 45;

    InSequence read_slider;
    expectMultiplexerSelectsADS1015(INPUT_LINEAR_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_LINEAR_POT_I2C_DEVICE, INPUT_LINEAR_POT, sliderPositionSmallLeftValue);

    expectMultiplexerSelectsADS1015(INPUT_LINEAR_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_LINEAR_POT_I2C_DEVICE, INPUT_LINEAR_POT, sliderPositionTooFarLeft);

    wall->getSliderPosition();
    ASSERT_EQ(wall->getSliderPosition(), INPUT_LINEAR_POT_LEFT_LIMIT);
}
TEST_F(PotentiometerFixture, TestRejectIllegalSliderRightValue)
{
    uint16_t sliderPositionTooFarRight = MAXIMUM_12BIT_VALUE;
    uint16_t sliderPositionMostlytoTheRight = 876;

    InSequence read_slider;
    expectMultiplexerSelectsADS1015(INPUT_LINEAR_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_LINEAR_POT_I2C_DEVICE, INPUT_LINEAR_POT, sliderPositionMostlytoTheRight);

    expectMultiplexerSelectsADS1015(INPUT_LINEAR_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_LINEAR_POT_I2C_DEVICE, INPUT_LINEAR_POT, sliderPositionTooFarRight);

    wall->getSliderPosition();
    ASSERT_EQ(wall->getSliderPosition(), INPUT_LINEAR_POT_RIGHT_LIMIT);
}


class LightSensorFixture : public WallFixture, public ::testing::WithParamInterface<photo_sensor> {
};
TEST_P(LightSensorFixture, TestReadBrightnessValue)
{
    photo_sensor sensor = GetParam();
    uint16_t brightness = 133 * sensor;

    InSequence read_photo_sensor;
    expectMultiplexerSelectsADS1015(INPUT_PHOTO_SENSOR_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_PHOTO_SENSOR_I2C_DEVICE,
        WallImplementation::photoSensorPin(sensor), brightness);
    ASSERT_EQ(wall->getPhotoSensorValue(sensor), brightness);
}
INSTANTIATE_TEST_CASE_P(PhotoresistorTests, LightSensorFixture, Values(
    LEFT_PHOTO,
    CENTER_PHOTO,
    RIGHT_PHOTO
    )
);

class TouchSensorFixture : public WallFixture, public ::testing::WithParamInterface<force_sensor> {
};
TEST_P(TouchSensorFixture, TestReadTOUCHValue)
{
    force_sensor sensor = GetParam();
    uint16_t force = 186 * sensor;

    InSequence read_force_sensor;
    expectMultiplexerSelectsADS1015(INPUT_FORCE_SENSOR_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_FORCE_SENSOR_I2C_DEVICE,
        WallImplementation::forceSensorPin(sensor), force);
    ASSERT_EQ(wall->getTouchSensorValue(sensor), force);
}
INSTANTIATE_TEST_CASE_P(TOUCHTests, TouchSensorFixture, Values(
    LEFT_TOUCH,
    BOTTOM_TOUCH,
    RIGHT_TOUCH
    )    
);

class ButtonFixture : public WallFixture, public ::testing::WithParamInterface<large_button> {
};
TEST_P(ButtonFixture, TestButtonDepressed)
{
    large_button button = GetParam();

    InSequence check_button_depressed;
    int device = WallImplementation::buttonDevice(button);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_READ(device, WallImplementation::buttonPin(button), LOW);
    ASSERT_TRUE(wall->isButtonDepressed(button));
}
TEST_P(ButtonFixture, TestButtonNotDepressed)
{
    large_button button = GetParam();

    InSequence check_button_depressed;
    int device = WallImplementation::buttonDevice(button);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_READ(device, WallImplementation::buttonPin(button), HIGH);
    ASSERT_FALSE(wall->isButtonDepressed(button));
}
TEST_P(ButtonFixture, TestButtonIlluminated)
{
    large_button button = GetParam();

    InSequence illuminate_button;
    int device = WallImplementation::buttonDevice(button);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_WRITE(device, WallImplementation::buttonLEDpin(button), HIGH);
    wall->illuminateButton(button);
}
TEST_P(ButtonFixture, TestButtonDarkened)
{
    large_button button = GetParam();

    InSequence darken_button;
    int device = WallImplementation::buttonDevice(button);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_WRITE(device, WallImplementation::buttonLEDpin(button), LOW);
    wall->extinguishButton(button);
}
INSTANTIATE_TEST_CASE_P(ButtonTests, ButtonFixture, Values(
    BLUE_BUTTON,
    YELLOW_BUTTON,
    GREEN_BUTTON,
    RED_BUTTON,
    WHITE_BUTTON
    )
);

}; // namespace
