// Input device tests for Wall object
//
#include "Wallfixture.h"
#include <Wall.h>
#include <tuple>
#include <Arduino.h>

namespace testing {

using std::tuple;
using std::make_tuple;
using std::get;

// Active-High LED array tests
class LEDHighFixture : public WallFixture,
    public ::testing::WithParamInterface<tuple<led_array, led_section>> {
};
TEST_P(LEDHighFixture, TurnOnLEDArray)
{
    const led_array array = get<0>(GetParam());
    const led_section section = get<1>(GetParam());

    InSequence led_on;
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_ANALOG_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), MINIMUM_ANALOG_OUTPUT_VALUE);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDHighFixture, TurnOffLEDArray)
{
    const led_array array = get<0>(GetParam());
    const led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_ANALOG_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), MAXIMUM_ANALOG_OUTPUT_VALUE);
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
    const led_array array = get<0>(GetParam());
    const led_section section = get<1>(GetParam());

    InSequence led_on;
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_ANALOG_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), MAXIMUM_ANALOG_OUTPUT_VALUE);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDLowFixture, TurnOffLEDArray)
{
    const led_array array = get<0>(GetParam());
    const led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectsSX1509(OUTPUT_LED_ARRAY_I2C_DEVICE);
    EXPECT_SX1509_ANALOG_WRITE(OUTPUT_LED_ARRAY_I2C_DEVICE,
        WallImplementation::ledArrayPin(array, section), MINIMUM_ANALOG_OUTPUT_VALUE);
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
    const wall_motor motor = GetParam();
    const uint8_t speed = 247;

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
    const wall_motor motor = GetParam();
    const uint8_t speed = 122;

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
    const wall_motor motor = GetParam();

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

// Electroluminescent wire tests
class WireFixture : public WallFixture, public ::testing::WithParamInterface<EL_wire> {
};
TEST_P(WireFixture, TestWireIlluminated)
{
    const EL_wire line = GetParam();

    InSequence illuminate_wire;
    EXPECT_CALL(*Intel101, digitalWrite(WallImplementation::elWirePin(line), HIGH));
    wall->illuminateELWire(line);
}
TEST_P(WireFixture, TestWireDarkened)
{
    const EL_wire line = GetParam();

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

// LCD tests
class DisplayFixture : public WallFixture {};
TEST_F(DisplayFixture, TestLCDprintAtLocation)
{
    const int row = 1;
    const int col = 0;
    char string[] = "Hello World!";
    expectMultiplexerSelectedBus(GROVE_LCD_I2C_BUS);
    EXPECT_CALL(*io->accessMockLCD(), setCursor(col, row));
    EXPECT_CALL(*io->accessMockLCD(), print(string));

    wall->lcdPrintAt(col, row, string);
}

TEST_F(DisplayFixture, TestSetLCDbackgroundColor)
{
    const int red = 145;
    const int green = 35;
    const int blue = 227;
    expectMultiplexerSelectedBus(GROVE_LCD_I2C_BUS);
    EXPECT_CALL(*io->accessMockLCD(), clear());
    
    wall->clearLCDscreen();
}

TEST_F(DisplayFixture, TestSetBackgroundColor)
{
    const int red = 145;
    const int green = 35;
    const int blue = 227;
    expectMultiplexerSelectedBus(GROVE_LCD_I2C_BUS);
    EXPECT_CALL(*io->accessMockLCD(), setRGB(red, green, blue));

    wall->lcdSetBacklightColor(red, green, blue);
}

}; // namespace
