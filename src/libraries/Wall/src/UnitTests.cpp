// Unit tests for Wall object
//
#include <gtest/gtest.h>
#include <tuple>
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include "MockDeviceFactory.h"
#include "Wall.h"

#define WIRE_TRANSMIT_SUCCESS 0

namespace testing {

using testing::StrictMock;

class WallFixture : public Test {
protected:
    
    ArduinoMock *Intel101;
    MockDeviceFactory *io;
    StrictMock<WireMock> *i2c;
    WallImplementation *wall;

    WallFixture() {
        Intel101 = arduinoMockInstance();
        i2c = static_cast<StrictMock<WireMock> *>(WireMockInstance());
        io = new MockDeviceFactory;
        wall = new WallImplementation(io);
    }

    virtual ~WallFixture() {
        delete io;
        delete wall;
        releaseWireMock();
        releaseArduinoMock();
    }

    void WallFixture::expectMultiplexerSelectsSX1509(int device);
    void WallFixture::expectMultiplexerSelectsADS1015(int device);
    void WallFixture::expectMultiplexerSelectedBus(int bus);
};

void WallFixture::expectMultiplexerSelectsSX1509(int device)
{
    expectMultiplexerSelectedBus(WallImplementation::ioDeviceBus[device]);
}
void WallFixture::expectMultiplexerSelectsADS1015(int device)
{
    expectMultiplexerSelectedBus(WallImplementation::analogDeviceBus[device]);
}
void WallFixture::expectMultiplexerSelectedBus(int bus)
{
    int expectedBusVector = 1 << bus;
    InSequence mux_bus_selection;
    EXPECT_CALL(*i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*i2c, write(expectedBusVector));
    EXPECT_CALL(*i2c, endTransmission()).WillOnce(Return(WIRE_TRANSMIT_SUCCESS));
}


// Simplification Macros to make tests easier to read
//
#define EXPECT_SX1509_PINMODE(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       pinMode(pin, value));

#define EXPECT_SX1509_DIGITAL_WRITE(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       digitalWrite(pin, value));

#define EXPECT_SX1509_ANALOG_WRITE(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       analogWrite(pin, value));

#define EXPECT_SX1509_DIGITAL_READ(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       digitalRead(pin)).WillOnce(Return(value));

#define EXPECT_ADS1015_ANALOG_READ(device, pin, value) \
   EXPECT_CALL(*io->accessMockADS1015(device), \
       readADC_SingleEnded(pin)).WillOnce(Return(value));


// Wall setup and DeviceFactory initialization tests
class InitFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(InitFixture, TestFailedIOExpanderInitialization)
{
    int failingDevice = GetParam(); 
    InSequence initialization;
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
    {
        expectMultiplexerSelectsSX1509(device);
        EXPECT_CALL(*io->accessMockSX1509(device),
            begin(WallImplementation::ioDeviceAddress[device], SPARKFUN_SX1509_RESET_PIN))
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
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_GREEN_LEFT, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_GREEN_RIGHT, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_WHITE_LEFT, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_WHITE_RIGHT, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_RED_QUAD_1, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_RED_QUAD_2, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_RED_QUAD_3, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_LED_ARRAY_I2C_DEVICE, OUTPUT_LED_ARRAY_RED_QUAD_4, OUTPUT);

    wall->initializeLEDarrayOutputs();
}
TEST_F(InitFixture, TestMotorPinModes)
{
    expectMultiplexerSelectsSX1509(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(OUTPUT_MOTOR_I2C_DEVICE, OUTPUT_MOTOR1_PWM, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_MOTOR_I2C_DEVICE, OUTPUT_MOTOR1_IN1, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_MOTOR_I2C_DEVICE, OUTPUT_MOTOR1_IN2, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_MOTOR_I2C_DEVICE, OUTPUT_MOTOR2_PWM, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_MOTOR_I2C_DEVICE, OUTPUT_MOTOR2_IN1, OUTPUT);
    EXPECT_SX1509_PINMODE(OUTPUT_MOTOR_I2C_DEVICE, OUTPUT_MOTOR2_IN2, OUTPUT);

    wall->initializeMotorOutputs();
}
TEST_F(InitFixture, TestTogglePinModes)
{
    expectMultiplexerSelectsSX1509(INPUT_TOGGLE_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(INPUT_TOGGLE_I2C_DEVICE, INPUT_TOGGLE_1, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(INPUT_TOGGLE_I2C_DEVICE, INPUT_TOGGLE_2, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(INPUT_TOGGLE_I2C_DEVICE, INPUT_TOGGLE_3, INPUT_PULLUP);
    wall->initializeToggleInputs();
}
TEST_F(InitFixture, TestJoystickPinModes)
{
    expectMultiplexerSelectsSX1509(INPUT_JOYSTICK_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(INPUT_TOGGLE_I2C_DEVICE, INPUT_JOYSTICK_DOWN, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(INPUT_TOGGLE_I2C_DEVICE, INPUT_JOYSTICK_LEFT, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(INPUT_TOGGLE_I2C_DEVICE, INPUT_JOYSTICK_UP, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(INPUT_TOGGLE_I2C_DEVICE, INPUT_JOYSTICK_RIGHT, INPUT_PULLUP);
    wall->initializeJoystickInputs();
}
TEST_F(InitFixture, TestLargeButtonPinModes)
{
    InSequence initialize_button_pins;
    expectMultiplexerSelectsSX1509(BLUE_BUTTON_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(BLUE_BUTTON_I2C_DEVICE, BLUE_BUTTON_PIN, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(BLUE_BUTTON_I2C_DEVICE, BLUE_LED_PIN, OUTPUT);

    expectMultiplexerSelectsSX1509(YELLOW_BUTTON_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(YELLOW_BUTTON_I2C_DEVICE, YELLOW_BUTTON_PIN, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(YELLOW_BUTTON_I2C_DEVICE, YELLOW_LED_PIN, OUTPUT);

    expectMultiplexerSelectsSX1509(GREEN_BUTTON_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(GREEN_BUTTON_I2C_DEVICE, GREEN_BUTTON_PIN, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(GREEN_BUTTON_I2C_DEVICE, GREEN_LED_PIN, OUTPUT);

    expectMultiplexerSelectsSX1509(RED_BUTTON_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(RED_BUTTON_I2C_DEVICE, RED_BUTTON_PIN, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(RED_BUTTON_I2C_DEVICE, RED_LED_PIN, OUTPUT);

    expectMultiplexerSelectsSX1509(WHITE_BUTTON_I2C_DEVICE);
    EXPECT_SX1509_PINMODE(WHITE_BUTTON_I2C_DEVICE, WHITE_BUTTON_PIN, INPUT_PULLUP);
    EXPECT_SX1509_PINMODE(WHITE_BUTTON_I2C_DEVICE, WHITE_LED_PIN, OUTPUT);

    wall->initializeButtonInOuts();
}
TEST_F(InitFixture, TestELwirePinModes)
{
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(RED_WIRE_ONE), OUTPUT));
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(RED_WIRE_TWO), OUTPUT));
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(GREEN_WIRE_ONE), OUTPUT));
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(GREEN_WIRE_TWO), OUTPUT));
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(YELLOW_WIRE), OUTPUT));
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(WHITE_WIRE), OUTPUT));
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(BLUE_WIRE_ONE), OUTPUT));
    EXPECT_CALL(*Intel101, pinMode(WallImplementation::elWirePin(BLUE_WIRE_TWO), OUTPUT));
    wall->initalizeELwireOutputs();
}

// I2C multiplexer select vectors
class MuxFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(MuxFixture, TestMultiplexerSelection)
{
    int deviceIndex = GetParam(); 
    expectMultiplexerSelectsSX1509(deviceIndex);
    wall->setMultiplexerForIOexpander(deviceIndex);
}
// Due to a bug in the Intel101 I2C protocol handling, it is possible that the
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
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), HIGH);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDHighFixture, TurnOffLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), LOW);
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
class LEDLowFixture : public WallFixture,
    public ::testing::WithParamInterface<tuple<led_array, led_section>> {
};
TEST_P(LEDLowFixture, TurnOnLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_on;
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), LOW);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDLowFixture, TurnOffLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), HIGH);
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
    expectMultiplexerSelectsSX1509(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorControlPin1(motor), HIGH);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorControlPin2(motor), LOW);
    expectMultiplexerSelectsSX1509(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_SX1509_ANALOG_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorPWMpin(motor), speed);

    wall->setMotorDirectionClockwise(motor);
    wall->setMotorSpeed(motor, speed);
}
TEST_P(MotorFixture, TestRunMotorCounterClockwise)
{
    wall_motor motor = GetParam();
    int speed = 522;

    InSequence run_motor;
    expectMultiplexerSelectsSX1509(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorControlPin1(motor), LOW);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorControlPin2(motor), HIGH);
    expectMultiplexerSelectsSX1509(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_SX1509_ANALOG_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorPWMpin(motor), speed);

    wall->setMotorDirectionCounterClockwise(motor);
    wall->setMotorSpeed(motor, speed);
}
TEST_P(MotorFixture, TestStopMotor)
{
    wall_motor motor = GetParam();

    InSequence stop_motor;
    expectMultiplexerSelectsSX1509(OUTPUT_MOTOR_I2C_DEVICE);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorControlPin1(motor), LOW);
    EXPECT_SX1509_DIGITAL_WRITE(OUTPUT_MOTOR_I2C_DEVICE,
        WallImplementation::motorControlPin2(motor), LOW);

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
    EXPECT_CALL(*io->accessMockPWM(), setPin(lamp, PWM_INDICATOR_ON_VALUE, FALSE));
    wall->turnIndicatorOn(lamp);
}
TEST_P(IndicatorFixture, TestTurnOffIndicator)
{
    indicator_led lamp = GetParam();

    InSequence lamp_off;
    expectMultiplexerSelectsSX1509(ADAFRUIT_PWM_I2C_BUS);
    EXPECT_CALL(*io->accessMockPWM(), setPin(lamp, PWM_INDICATOR_OFF_VALUE, FALSE));
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
TEST_F(PotentiometerFixture, TestReadSliderValue)
{
    uint16_t sliderPosition = 926;
    InSequence read_slider;
    expectMultiplexerSelectsADS1015(INPUT_LINEAR_POT_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_LINEAR_POT_I2C_DEVICE, INPUT_LINEAR_POT, sliderPosition);
    ASSERT_EQ(wall->getSliderPosition(), sliderPosition);
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
TEST_P(TouchSensorFixture, TestReadPressureValue)
{
    force_sensor sensor = GetParam();
    uint16_t force = 186 * sensor;

    InSequence read_force_sensor;
    expectMultiplexerSelectsADS1015(INPUT_FORCE_SENSOR_I2C_DEVICE);
    EXPECT_ADS1015_ANALOG_READ(INPUT_FORCE_SENSOR_I2C_DEVICE,
        WallImplementation::forceSensorPin(sensor), force);
    ASSERT_EQ(wall->getTouchSensorValue(sensor), force);
}
INSTANTIATE_TEST_CASE_P(PressureTests, TouchSensorFixture, Values(
    LEFT_PRESSURE,
    BOTTOM_PRESSURE,
    RIGHT_PRESSURE
    )    
);

class CircuitConnectionFixture : public WallFixture,
    public ::testing::WithParamInterface<circuit_end> {
protected:
    void expectCircuitIsInput(circuit_end end);
    void expectCircuitIsOutput(circuit_end end);
};
void CircuitConnectionFixture::expectCircuitIsInput(circuit_end end)
{
    int device = WallImplementation::circuitDevice(end);

    InSequence set_to_input;
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_PINMODE(device, WallImplementation::circuitPin(end), INPUT_PULLUP);
}
void CircuitConnectionFixture::expectCircuitIsOutput(circuit_end end)
{
    int device = WallImplementation::circuitDevice(end);
    int pin = WallImplementation::circuitPin(end);

    InSequence set_to_output;
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_PINMODE(device, pin, OUTPUT);
    EXPECT_SX1509_DIGITAL_WRITE(device, pin, LOW);
}

TEST_P(CircuitConnectionFixture, TestCircuitIdle)
{
    circuit_end end = GetParam();
 
    InSequence read_connection_value;
    int device = WallImplementation::circuitDevice(end);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_READ(device, WallImplementation::circuitPin(end), HIGH);
    ASSERT_EQ(wall->readCircuitState(end), HIGH);
}
TEST_P(CircuitConnectionFixture, TestCircuitEnergized)
{
    circuit_end end = GetParam();

    InSequence read_connection_value;
    int device = WallImplementation::circuitDevice(end);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_READ(device, WallImplementation::circuitPin(end), LOW);
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
    int sink_device = WallImplementation::circuitDevice(sink);
    
    expectCircuitIsOutput(source);
    expectMultiplexerSelectsSX1509(sink_device);
    EXPECT_SX1509_DIGITAL_READ(sink_device, WallImplementation::circuitPin(sink), LOW);
    expectCircuitIsInput(source);

    ASSERT_TRUE(wall->isCircuitConnected(source, sink));
}
TEST_F(CircuitConnectionFixture, TestCircuitsNotConnected)
{
    circuit_end sink = CIRCUIT_JOYSTICK_LEFT;
    circuit_end source = CIRCUIT_POSITIVE_POLE;

    InSequence check_connection;
    int sink_device = WallImplementation::circuitDevice(sink);

    expectCircuitIsOutput(source);
    expectMultiplexerSelectsSX1509(sink_device);
    EXPECT_SX1509_DIGITAL_READ(sink_device, WallImplementation::circuitPin(sink), HIGH);
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

class WireFixture : public WallFixture, public ::testing::WithParamInterface<EL_wire> {
};
TEST_P(WireFixture, TestWireIlluminated)
{
    EL_wire line = GetParam();

    InSequence illuminate_wire;
    EXPECT_CALL(*Intel101, digitalWrite(WallImplementation::elWirePin(line), HIGH));
    wall->illuminateELWire(line);
}
TEST_P(WireFixture, TestWireDarkened)
{
    EL_wire line = GetParam();

    InSequence illuminate_wire;
    EXPECT_CALL(*Intel101, digitalWrite(WallImplementation::elWirePin(line), LOW));
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

class DisplayFixture: public WallFixture {};
TEST_F(DisplayFixture, TestPrintAtLocation)
{
    int row = 1;
    int col = 0;
    char string[] = "Hello World!";
    expectMultiplexerSelectedBus(GROVE_LCD_I2C_BUS);
    EXPECT_CALL(*io->accessMockLCD(), setCursor(col, row));
    EXPECT_CALL(*io->accessMockLCD(), print(string));

    wall->printAt(col, row, string);
}

}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
