// Initialization tests for Wall object
//
#include "Wallfixture.h"

namespace testing {

using testing::StrictMock;

// Wall setup and DeviceFactory initialization tests
class InitFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(InitFixture, TestFailedIOExpanderInitialization)
{
    const int failingDevice = GetParam(); 
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
    
    const bool noDeviceWillFail = (failingDevice < 0) || (failingDevice >= NUMBER_OF_SX1509_DEVICES);
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
    const int deviceIndex = GetParam(); 
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
    const int targetBus = 3;
    const int expectedBusVector = 1 << targetBus;
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

}; // namespace
