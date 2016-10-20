// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../arduino-mock/Arduino.h"
#include "../../arduino-mock/Wire.h"
#include "MockDeviceFactory.h"
#include "Wall.h"
#include <tuple>

#define WIRE_TRANSMIT_SUCCESS 0

namespace testing {

using testing::StrictMock;

class WallFixture : public Test {
protected:
    
    ArduinoMock *Intel101;
    MockDeviceFactory *io;
    StrictMock<WireMock> *i2c;
    Wall *wall;    

    WallFixture() {
        Intel101 = arduinoMockInstance();
        i2c = static_cast<StrictMock<WireMock> *>(WireMockInstance());
        io = new MockDeviceFactory;
        wall = new Wall(io);
    }

    virtual ~WallFixture() {
        releaseWireMock();
        delete io;
        delete wall;
        releaseArduinoMock();
    }

    void WallFixture::expectMultiplexerSelectedBusforIOexpander(int device);
    void WallFixture::expectMultiplexerSelectedBusforAnalog(int device);
    void WallFixture::expectMultiplexerSelectedBus(int bus);
};

void WallFixture::expectMultiplexerSelectedBusforIOexpander(int device)
{
    expectMultiplexerSelectedBus(Wall::ioDeviceBus[device]);
}
void WallFixture::expectMultiplexerSelectedBusforAnalog(int device)
{
    expectMultiplexerSelectedBus(Wall::analogDeviceBus[device]);
}
void WallFixture::expectMultiplexerSelectedBus(int bus)
{
    int expectedBusVector = 1 << bus;
    InSequence mux_bus_selection;
    EXPECT_CALL(*i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*i2c, write(expectedBusVector));
    EXPECT_CALL(*i2c, endTransmission()).WillOnce(Return(WIRE_TRANSMIT_SUCCESS));
}

// Wall setup and DeviceFactory initialization tests
class InitFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(InitFixture, TestFailedIOExpanderInitialization)
{
    int failingDevice = GetParam(); 
    InSequence initialization;
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
    {
        expectMultiplexerSelectedBusforIOexpander(device);
        EXPECT_CALL(*io->accessMockSX1509(device),
            begin(Wall::ioDeviceAddress[device], SPARKFUN_SX1509_RESET_PIN))
            .WillOnce(Return(device != failingDevice));
        if (device == failingDevice)
            break;
    }
    
    bool noDeviceWillFail = (failingDevice < 0) || (failingDevice >= NUMBER_OF_SX1509_DEVICES);
    ASSERT_EQ(wall->initializeIOexpanders(), noDeviceWillFail);
}
INSTANTIATE_TEST_CASE_P(InitalizationTests, InitFixture, Values(-1, 0, 1, 2, 3));

TEST_F(InitFixture, TestLEDpinModes)
{
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_GREEN_LEFT, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_GREEN_RIGHT, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_WHITE_LEFT, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_WHITE_RIGHT, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_RED_QUAD_1, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_RED_QUAD_2, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_RED_QUAD_3, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        pinMode(OUTPUT_LED_ARRAY_RED_QUAD_4, OUTPUT));

    wall->initializeLEDarrayOutputs();
}
TEST_F(InitFixture, TestMotorPinModes)
{
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        pinMode(OUTPUT_MOTOR1_PWM, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        pinMode(OUTPUT_MOTOR1_IN1, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        pinMode(OUTPUT_MOTOR1_IN2, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        pinMode(OUTPUT_MOTOR2_PWM, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        pinMode(OUTPUT_MOTOR2_IN1, OUTPUT));
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        pinMode(OUTPUT_MOTOR2_IN2, OUTPUT));

    wall->initializeMotorOutputs();
}
TEST_F(InitFixture, TestTogglePinModes)
{
    expectMultiplexerSelectedBusforIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        pinMode(INPUT_TOGGLE_1, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        pinMode(INPUT_TOGGLE_2, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        pinMode(INPUT_TOGGLE_3, INPUT_PULLUP));
    wall->initializeToggleInputs();
}
TEST_F(InitFixture, TestJoystickPinModes)
{
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        pinMode(INPUT_JOYSTICK_DOWN, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        pinMode(INPUT_JOYSTICK_LEFT, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        pinMode(INPUT_JOYSTICK_UP, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        pinMode(INPUT_JOYSTICK_RIGHT, INPUT_PULLUP));
    wall->initializeJoystickInputs();
}
TEST_F(InitFixture, TestLargeButtonPinModes)
{
    InSequence initialize_button_pins;
    expectMultiplexerSelectedBusforIOexpander(BLUE_BUTTON_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(BLUE_BUTTON_I2C_DEVICE),
        pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(BLUE_BUTTON_I2C_DEVICE),
        pinMode(BLUE_LED_PIN, OUTPUT));

    expectMultiplexerSelectedBusforIOexpander(YELLOW_BUTTON_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(YELLOW_BUTTON_I2C_DEVICE),
        pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(YELLOW_BUTTON_I2C_DEVICE),
        pinMode(YELLOW_LED_PIN, OUTPUT));

    expectMultiplexerSelectedBusforIOexpander(GREEN_BUTTON_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(GREEN_BUTTON_I2C_DEVICE),
        pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(GREEN_BUTTON_I2C_DEVICE),
        pinMode(GREEN_LED_PIN, OUTPUT));

    expectMultiplexerSelectedBusforIOexpander(RED_BUTTON_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(RED_BUTTON_I2C_DEVICE),
        pinMode(RED_BUTTON_PIN, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(RED_BUTTON_I2C_DEVICE),
        pinMode(RED_LED_PIN, OUTPUT));

    expectMultiplexerSelectedBusforIOexpander(WHITE_BUTTON_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(WHITE_BUTTON_I2C_DEVICE),
        pinMode(WHITE_BUTTON_PIN, INPUT_PULLUP));
    EXPECT_CALL(*io->accessMockSX1509(WHITE_BUTTON_I2C_DEVICE),
        pinMode(WHITE_LED_PIN, OUTPUT));

    wall->initializeButtonInOuts();
}
TEST_F(InitFixture, TestELwirePinModes)
{
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(RED_WIRE_ONE), OUTPUT)).Times(1);
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(RED_WIRE_TWO), OUTPUT)).Times(1);
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(GREEN_WIRE_ONE), OUTPUT)).Times(1);
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(GREEN_WIRE_TWO), OUTPUT)).Times(1);
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(YELLOW_WIRE), OUTPUT)).Times(1);
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(WHITE_WIRE), OUTPUT)).Times(1);
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(BLUE_WIRE_ONE), OUTPUT)).Times(1);
    EXPECT_CALL(*Intel101, pinMode(Wall::elWirePin(BLUE_WIRE_TWO), OUTPUT)).Times(1);
    wall->initalizeELwireOutputs();
}

// I2C multiplexer select vectors
class MuxFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(MuxFixture, TestMultiplexerSelection)
{
    int deviceIndex = GetParam(); 
    expectMultiplexerSelectedBusforIOexpander(deviceIndex);
    wall->setMultiplexerForIOexpander(deviceIndex);
}
// Due to a bug in the Intel101 I2C protocol handling,it is possible that the
// 101 will not complete the transaction and the Adafruit multiplexer will not
// select the correct bus.  Luckily this is reported by the Wire protocol, and
// a second attempt can be made.  Back-to-back failures have not been observed,
// but must be assumed to be possible.
TEST_F(MuxFixture, TestMuxCommunicationFailure)
{
    int targetBus = 3;
    int expectedBusVector = 1 << targetBus;
    InSequence mux_bus_selection;

    EXPECT_CALL(*i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*i2c, write(expectedBusVector));
    EXPECT_CALL(*i2c, endTransmission()).WillOnce(Return(!WIRE_TRANSMIT_SUCCESS));

    EXPECT_CALL(*i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*i2c, write(expectedBusVector));
    EXPECT_CALL(*i2c, endTransmission()).WillOnce(Return(!WIRE_TRANSMIT_SUCCESS));

    EXPECT_CALL(*i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*i2c, write(expectedBusVector));
    EXPECT_CALL(*i2c, endTransmission()).WillOnce(Return(WIRE_TRANSMIT_SUCCESS));

    wall->setMultiplexerI2CBus(targetBus);
}
INSTANTIATE_TEST_CASE_P(MuxSelectionTests, MuxFixture, Values(0,1,2));

// Active-High LED array tests
class LEDHighFixture : public WallFixture, 
    public ::testing::WithParamInterface<tuple<led_array, led_section>> {
};
TEST_P(LEDHighFixture, TurnOnLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_on;
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        digitalWrite(Wall::ledArrayPin(array, section), HIGH)).Times(1);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDHighFixture, TurnOffLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        digitalWrite(Wall::ledArrayPin(array, section), LOW)).Times(1);
    wall->turnOffLEDarray(array, section);
}
INSTANTIATE_TEST_CASE_P(LEDArrayTests, LEDHighFixture, Values(
    make_tuple(WHITE_LED, LEFT_SIDE),
    make_tuple(WHITE_LED, RIGHT_SIDE),
    make_tuple(GREEN_LED, LEFT_SIDE),
    make_tuple(GREEN_LED, RIGHT_SIDE)
    )
);

// Active Low LED array tests
class LEDLowFixture : public WallFixture, public ::testing::WithParamInterface<tuple<led_array, led_section>> {
};
TEST_P(LEDLowFixture, TurnOnLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_on;
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        digitalWrite(Wall::ledArrayPin(array, section), LOW)).Times(1);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDLowFixture, TurnOffLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE),
        digitalWrite(Wall::ledArrayPin(array, section), HIGH)).Times(1);
    wall->turnOffLEDarray(array, section);
}
INSTANTIATE_TEST_CASE_P(LEDArrayTests, LEDLowFixture, Values(
    make_tuple(RED_LED, LEFT_SIDE),
    make_tuple(RED_LED, RIGHT_SIDE),
    make_tuple(RED_LED, LOWER_LEFT_SIDE),
    make_tuple(RED_LED, LOWER_RIGHT_SIDE)
    )
);

// Motor tests
class MotorFixture : public WallFixture, public ::testing::WithParamInterface<wall_motor> {
};
TEST_P(MotorFixture, TestRunMotorClockwise)
{
    wall_motor motor = GetParam();
    int speed = 247;

    InSequence run_motor;
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        digitalWrite(Wall::motorControlPin1(motor), HIGH)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        digitalWrite(Wall::motorControlPin2(motor), LOW)).Times(1);
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        analogWrite(Wall::motorPWMpin(motor), speed)).Times(1);

    wall->setMotorDirectionClockwise(motor);
    wall->setMotorSpeed(motor, speed);
}
TEST_P(MotorFixture, TestRunMotorCounterClockwise)
{
    wall_motor motor = GetParam();
    int speed = 522;

    InSequence run_motor;
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        digitalWrite(Wall::motorControlPin1(motor), LOW)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        digitalWrite(Wall::motorControlPin2(motor), HIGH)).Times(1);
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        analogWrite(Wall::motorPWMpin(motor), speed)).Times(1);

    wall->setMotorDirectionCounterClockwise(motor);
    wall->setMotorSpeed(motor, speed);
}
TEST_P(MotorFixture, TestStopMotor)
{
    wall_motor motor = GetParam();

    InSequence stop_motor;
    expectMultiplexerSelectedBusforIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        digitalWrite(Wall::motorControlPin1(motor), LOW)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(OUTPUT_MOTOR_I2C_DEVICE),
        digitalWrite(Wall::motorControlPin2(motor), LOW)).Times(1);

    wall->stopMotor(motor);
}
INSTANTIATE_TEST_CASE_P(MotorTests, MotorFixture, Values(
    BLUE_MOTOR,
    ORANGE_MOTOR)
);

// Transducer tests
class SoundFixture : public WallFixture {
};
TEST_F(SoundFixture, TestTransducerOn)
{
    InSequence make_sound;
    expectMultiplexerSelectedBus(ADAFRUIT_PWM_I2C_BUS);
    EXPECT_CALL(*io->accessMockPWM(),
        setPWM(OUTPUT_TRANSDUCER, PWM_START_OF_DUTY_CYCLE, PWM_HALF_DUTY_CYCLE-1));
    wall->turnTransducerOn();
}
TEST_F(SoundFixture, TestTransducerOff)
{
    InSequence no_sound;
    expectMultiplexerSelectedBus(ADAFRUIT_PWM_I2C_BUS);
    EXPECT_CALL(*io->accessMockPWM(),
        setPWM(OUTPUT_TRANSDUCER, PWM_START_OF_DUTY_CYCLE, PWM_FULL_DUTY_CYCLE));
    wall->turnTransducerOff();
}

class IndicatorFixture : public WallFixture, public ::testing::WithParamInterface<indicator_led> {
};
TEST_P(IndicatorFixture, TestTurnOnIndicator)
{
    indicator_led lamp = GetParam();

    InSequence lamp_on;
    expectMultiplexerSelectedBus(ADAFRUIT_PWM_I2C_BUS);
    EXPECT_CALL(*io->accessMockPWM(),
        setPin(lamp, PWM_INDICATOR_ON_VALUE, FALSE));
    wall->turnIndicatorOn(lamp);
}
TEST_P(IndicatorFixture, TestTurnOffIndicator)
{
    indicator_led lamp = GetParam();

    InSequence lamp_off;
    expectMultiplexerSelectedBusforIOexpander(ADAFRUIT_PWM_I2C_BUS);
    EXPECT_CALL(*io->accessMockPWM(),
        setPin(lamp, PWM_INDICATOR_OFF_VALUE, FALSE));
    wall->turnIndicatorOff(lamp);
} 
INSTANTIATE_TEST_CASE_P(IndicatorTests, IndicatorFixture, Values(
    INDICATE_WHITE_LED,
    INDICATE_RED_LED,
    INDICATE_GREEN_LED,
    INDICATE_BLUE_MOTOR,
    INDICATE_ORANGE_MOTOR,
    INDICATE_TRANSDUCER,
    INDICATE_TOGGLES,
    INDICATE_JOYSTICK,
    INDICATE_KNOB,
    INDICATE_SLIDER,
    INDICATE_PHOTO_SENSOR,
    INDICATE_PRESSURE_SENSOR,
    INDICATE_POSITIVE_POLE,
    INDICATE_NEGATIVE_POLE
    )
);

class SwitchFixture : public WallFixture, public ::testing::WithParamInterface<toggle_switch> {
};
TEST_P(SwitchFixture, TestReadSwitchOn)
{
    toggle_switch toggle = GetParam();

    InSequence is_switch_on;
    expectMultiplexerSelectedBusforIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        digitalRead(Wall::toggleSwitchPin(toggle))).WillOnce(Return(LOW));
    ASSERT_TRUE(wall->isToggleOn(toggle));
}
TEST_P(SwitchFixture, TestReadSwitchOff)
{
    toggle_switch toggle = GetParam();

    InSequence is_switch_off;
    expectMultiplexerSelectedBusforIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        digitalRead(Wall::toggleSwitchPin(toggle))).WillOnce(Return(HIGH));
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
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_UP)).WillOnce(Return(LOW));
    ASSERT_TRUE(wall->isJoystickUp());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotUp)
{
    InSequence is_joystick_up;
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_UP)).WillOnce(Return(HIGH));
    ASSERT_FALSE(wall->isJoystickUp());
}
TEST_F(JoystickFixture, TestReadJoystickIsDown)
{
    InSequence is_joystick_down;
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_DOWN)).WillOnce(Return(LOW));
    ASSERT_TRUE(wall->isJoystickDown());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotDown)
{
    InSequence is_joystick_down;
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_DOWN)).WillOnce(Return(HIGH));
    ASSERT_FALSE(wall->isJoystickDown());
}
TEST_F(JoystickFixture, TestReadJoystickIsLeft)
{
    InSequence is_joystick_left;
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_LEFT)).WillOnce(Return(LOW));
    ASSERT_TRUE(wall->isJoystickLeft());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotLeft)
{
    InSequence is_joystick_left;
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_LEFT)).WillOnce(Return(HIGH));
    ASSERT_FALSE(wall->isJoystickLeft());
}
TEST_F(JoystickFixture, TestReadJoystickIsRight)
{
    InSequence is_joystick_right;
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_RIGHT)).WillOnce(Return(LOW));
    ASSERT_TRUE(wall->isJoystickRight());
}
TEST_F(JoystickFixture, TestReadJoystickIsNotRight)
{
    InSequence is_joystick_right;
    expectMultiplexerSelectedBusforIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_JOYSTICK_I2C_DEVICE),
        digitalRead(INPUT_JOYSTICK_RIGHT)).WillOnce(Return(HIGH));
    ASSERT_FALSE(wall->isJoystickRight());
}

class PotentiometerFixture : public WallFixture {
};
TEST_F(PotentiometerFixture, TestReadKnobValue)
{
    uint16_t knobPosition = 147;
    InSequence read_knob;
    expectMultiplexerSelectedBusforAnalog(INPUT_ROTARY_POT_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockADS1015(INPUT_ROTARY_POT_I2C_DEVICE),
        readADC_SingleEnded(INPUT_ROTARY_POT)).WillOnce(Return(knobPosition));
    ASSERT_EQ(wall->getKnobPosition(), knobPosition);
}
TEST_F(PotentiometerFixture, TestReadSliderValue)
{
    uint16_t sliderPosition = 926;
    InSequence read_slider;
    expectMultiplexerSelectedBusforAnalog(INPUT_LINEAR_POT_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockADS1015(INPUT_LINEAR_POT_I2C_DEVICE),
        readADC_SingleEnded(INPUT_LINEAR_POT)).WillOnce(Return(sliderPosition));
    ASSERT_EQ(wall->getSliderPosition(), sliderPosition);
}

class LightSensorFixture : public WallFixture, public ::testing::WithParamInterface<photo_sensor> {
};
TEST_P(LightSensorFixture, TestReadBrightnessValue)
{
    photo_sensor sensor = GetParam();
    uint16_t brightness = 133 * sensor;

    InSequence read_photo_sensor;
    expectMultiplexerSelectedBusforAnalog(INPUT_PHOTO_SENSOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockADS1015(INPUT_PHOTO_SENSOR_I2C_DEVICE),
        readADC_SingleEnded(Wall::photoSensorPin(sensor))).WillOnce(Return(brightness));
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
TEST_P(TouchSensorFixture, TestReadPressureValue)
{
    force_sensor sensor = GetParam();
    uint16_t force = 186 * sensor;

    InSequence read_force_sensor;
    expectMultiplexerSelectedBusforAnalog(INPUT_FORCE_SENSOR_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockADS1015(INPUT_FORCE_SENSOR_I2C_DEVICE),
        readADC_SingleEnded(Wall::forceSensorPin(sensor))).WillOnce(Return(force));
    ASSERT_EQ(wall->getTouchSensorValue(sensor), force);
}
INSTANTIATE_TEST_CASE_P(PressureTests, TouchSensorFixture, Values(
    LEFT_PRESSURE,
    BOTTOM_PRESSURE,
    RIGHT_PRESSURE
    )    
);

class CircuitConnectionFixture : public WallFixture, public ::testing::WithParamInterface<circuit_end> {
protected:
    void expectCircuitIsInput(circuit_end end);
    void expectCircuitIsOutput(circuit_end end);
};
void CircuitConnectionFixture::expectCircuitIsInput(circuit_end end)
{
    int device = Wall::circuitDevice(end);

    InSequence set_to_input;
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device),
        pinMode(Wall::circuitPin(end), INPUT_PULLUP)).Times(1);
}
void CircuitConnectionFixture::expectCircuitIsOutput(circuit_end end)
{
    int device = Wall::circuitDevice(end);
    int pin = Wall::circuitPin(end);

    InSequence set_to_output;
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device), pinMode(pin, OUTPUT)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(device), digitalWrite(pin, LOW)).Times(1);
}

TEST_P(CircuitConnectionFixture, TestCircuitIdle)
{
    circuit_end end = GetParam();
 
    InSequence read_connection_value;
    int device = Wall::circuitDevice(end);
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device),
        digitalRead(Wall::circuitPin(end))).WillOnce(Return(HIGH));
    ASSERT_EQ(wall->readCircuitState(end), HIGH);
}
TEST_P(CircuitConnectionFixture, TestCircuitEnergized)
{
    circuit_end end = GetParam();

    InSequence read_connection_value;
    int device = Wall::circuitDevice(end);
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device),
        digitalRead(Wall::circuitPin(end))).WillOnce(Return(LOW));
    ASSERT_EQ(wall->readCircuitState(end), LOW);
}
TEST_P(CircuitConnectionFixture, TestCircuitInput)
{
    circuit_end end = GetParam();
    expectCircuitIsInput(end);
    wall->setCircuitAsInput(end);
}
TEST_P(CircuitConnectionFixture, TestCircuitOutput)
{
    circuit_end end = GetParam();
    expectCircuitIsOutput(end);
    wall->setCircuitAsOutput(end);
}
TEST_F(CircuitConnectionFixture, TestCircuitsConnected)
{
    circuit_end sink = CIRCUIT_BLUE_MOTOR_LEFT;
    circuit_end source = CIRCUIT_NEGATIVE_POLE;

    InSequence check_connection;
    int sink_device = Wall::circuitDevice(sink);
    
    expectCircuitIsOutput(source);
    expectMultiplexerSelectedBusforIOexpander(sink_device);
    EXPECT_CALL(*io->accessMockSX1509(sink_device),
        digitalRead(Wall::circuitPin(sink))).WillOnce(Return(LOW));
    expectCircuitIsInput(source);

    ASSERT_TRUE(wall->isCircuitConnected(source, sink));
}
TEST_F(CircuitConnectionFixture, TestCircuitsNotConnected)
{
    circuit_end sink = CIRCUIT_JOYSTICK_LEFT;
    circuit_end source = CIRCUIT_POSITIVE_POLE;

    InSequence check_connection;
    int sink_device = Wall::circuitDevice(sink);

    expectCircuitIsOutput(source);
    expectMultiplexerSelectedBusforIOexpander(sink_device);
    EXPECT_CALL(*io->accessMockSX1509(sink_device),
        digitalRead(Wall::circuitPin(sink))).WillOnce(Return(HIGH));
    expectCircuitIsInput(source);

    ASSERT_FALSE(wall->isCircuitConnected(source, sink));
}

INSTANTIATE_TEST_CASE_P(ConnectionTests, CircuitConnectionFixture, Values(
    CIRCUIT_KNOB_LEFT,
    CIRCUIT_KNOB_RIGHT,
    CIRCUIT_SLIDER_LEFT,
    CIRCUIT_SLIDER_RIGHT,
    CIRCUIT_PHOTO_LEFT,
    CIRCUIT_PHOTO_RIGHT,
    CIRCUIT_JOYSTICK_LEFT,
    CIRCUIT_JOYSTICK_RIGHT,
    CIRCUIT_TOGGLE_LEFT,
    CIRCUIT_TOGGLE_RIGHT,
    CIRCUIT_TOUCH_LEFT,
    CIRCUIT_TOUCH_RIGHT,
    CIRCUIT_BLUE_MOTOR_LEFT,
    CIRCUIT_BLUE_MOTOR_RIGHT,
    CIRCUIT_ORANGE_MOTOR_LEFT,
    CIRCUIT_ORANGE_MOTOR_RIGHT,
    CIRCUIT_TRANSDUCER_LEFT,
    CIRCUIT_TRANSDUCER_RIGHT,
    CIRCUIT_WHITE_LED_LEFT,
    CIRCUIT_WHITE_LED_RIGHT,
    CIRCUIT_GREEN_LED_LEFT,
    CIRCUIT_GREEN_LED_RIGHT,
    CIRCUIT_RED_LED_LEFT,
    CIRCUIT_RED_LED_RIGHT,
    CIRCUIT_POSITIVE_POLE,
    CIRCUIT_NEGATIVE_POLE
    )
);

class ButtonFixture : public WallFixture, public ::testing::WithParamInterface<large_button> {
};
TEST_P(ButtonFixture, TestButtonDepressed)
{
    large_button button = GetParam();

    InSequence check_button_depressed;
    int device = Wall::buttonDevice(button);
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device),
        digitalRead(Wall::buttonPin(button))).WillOnce(Return(LOW));
    ASSERT_TRUE(wall->isButtonDepressed(button));
}
TEST_P(ButtonFixture, TestButtonNotDepressed)
{
    large_button button = GetParam();

    InSequence check_button_depressed;
    int device = Wall::buttonDevice(button);
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device),
        digitalRead(Wall::buttonPin(button))).WillOnce(Return(HIGH));
    ASSERT_FALSE(wall->isButtonDepressed(button));
}
TEST_P(ButtonFixture, TestButtonIlluminated)
{
    large_button button = GetParam();

    InSequence illuminate_button;
    int device = Wall::buttonDevice(button);
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device),
        digitalWrite(Wall::buttonLEDpin(button), HIGH)).Times(1);
    wall->illuminateButton(button);
}
TEST_P(ButtonFixture, TestButtonDarkened)
{
    large_button button = GetParam();

    InSequence darken_button;
    int device = Wall::buttonDevice(button);
    expectMultiplexerSelectedBusforIOexpander(device);
    EXPECT_CALL(*io->accessMockSX1509(device),
        digitalWrite(Wall::buttonLEDpin(button), LOW)).Times(1);
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

class WireFixture : public WallFixture, public ::testing::WithParamInterface<EL_wire> {
};
TEST_P(WireFixture, TestWireIlluminated)
{
    EL_wire line = GetParam();

    InSequence illuminate_wire;
    EXPECT_CALL(*Intel101,
        digitalWrite(Wall::elWirePin(line), HIGH)).Times(1);
    wall->illuminateELWire(line);
}
TEST_P(WireFixture, TestWireDarkened)
{
    EL_wire line = GetParam();

    InSequence illuminate_wire;
    EXPECT_CALL(*Intel101,
        digitalWrite(Wall::elWirePin(line), LOW)).Times(1);
    wall->extinguishELWire(line);
}
INSTANTIATE_TEST_CASE_P(WireTests, WireFixture, Values(
    RED_WIRE_ONE,
    RED_WIRE_TWO,
    GREEN_WIRE_ONE,
    GREEN_WIRE_TWO,
    YELLOW_WIRE,
    WHITE_WIRE,
    BLUE_WIRE_ONE,
    BLUE_WIRE_TWO
    )
);

}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
