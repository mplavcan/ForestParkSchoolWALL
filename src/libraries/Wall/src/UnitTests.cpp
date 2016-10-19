// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../arduino-mock/Arduino.h"
#include "../../arduino-mock/Wire.h"
#include "MockDeviceFactory.h"
#include "Wall.h"
#include <tuple>

namespace testing {

using testing::StrictMock;

class WallFixture : public Test {
protected:
    
    MockDeviceFactory *io;
    StrictMock<WireMock> *mock_i2c;
    Wall *wall;    

    WallFixture() {
        mock_i2c = static_cast<StrictMock<WireMock> *>(WireMockInstance());
        io = new MockDeviceFactory;
        wall = new Wall(io);
    }

    virtual ~WallFixture() {
        releaseWireMock();
        delete io;
        delete wall;
    }

    void WallFixture::expectMultiplexerSelectedBusforIOexpander(int device);
    void WallFixture::expectMultiplexerSelectedBus(int bus);
};

void WallFixture::expectMultiplexerSelectedBusforIOexpander(int device)
{
    expectMultiplexerSelectedBus(Wall::IODeviceBus[device]);
}
void WallFixture::expectMultiplexerSelectedBus(int bus)
{
    int expected_bus_vector = 1 << bus;
    InSequence mux_bus_selection;
    EXPECT_CALL(*mock_i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*mock_i2c, write(expected_bus_vector));
    EXPECT_CALL(*mock_i2c, endTransmission()).WillOnce(Return(WIRE_TRANSMIT_SUCCESS));
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
            begin(Wall::IODeviceAddress[device], SPARKFUN_SX1509_RESET_PIN))
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

// I2C multiplexer select vectors
class MuxFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(MuxFixture, TestMultiplexerSelection)
{
    int deviceIndex = GetParam(); 
    expectMultiplexerSelectedBusforIOexpander(deviceIndex);
    wall->setMultiplexerForIOexpander(deviceIndex);
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
TEST_P(IndicatorFixture, TurnOnIndicator)
{
    indicator_led lamp = GetParam();

    InSequence lamp_on;
    expectMultiplexerSelectedBus(ADAFRUIT_PWM_I2C_BUS);
    EXPECT_CALL(*io->accessMockPWM(),
        setPin(lamp, PWM_INDICATOR_ON_VALUE, FALSE));
    wall->turnIndicatorOn(lamp);
}
TEST_P(IndicatorFixture, TurnOffIndicator)
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
TEST_P(SwitchFixture, ReadSwitchOn)
{
    toggle_switch toggle = GetParam();

    InSequence is_switch_on;
    expectMultiplexerSelectedBusforIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    EXPECT_CALL(*io->accessMockSX1509(INPUT_TOGGLE_I2C_DEVICE),
        digitalRead(Wall::toggleSwitchPin(toggle))).WillOnce(Return(LOW));
    ASSERT_TRUE(wall->isToggleOn(toggle));
}
TEST_P(SwitchFixture, ReadSwitchOff)
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

}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
