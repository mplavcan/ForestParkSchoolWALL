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

    void WallFixture::expectMultiplexerSelectedBus(int choice);
};

void WallFixture::expectMultiplexerSelectedBus(int bus_choice)
{
    int expected_bus_vector = 1 << bus_choice;
    InSequence mux_bus_selection;
    EXPECT_CALL(*mock_i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*mock_i2c, write(expected_bus_vector));
    EXPECT_CALL(*mock_i2c, endTransmission()).WillOnce(Return(WIRE_TRANSMIT_SUCCESS));
}


// Wall setup and DeviceFactory initialization tests
class InitFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(InitFixture, TestMultiplexerSelection)
{
    int failingDevice = GetParam(); 
    InSequence initialization;
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
    {
        expectMultiplexerSelectedBus(Wall::IODeviceBus[device]);
        EXPECT_CALL(*io->accessMockSX1509(device),
            begin(Wall::IODeviceAddress[device], SPARKFUN_SX1509_RESET_PIN))
            .WillOnce(Return(device != failingDevice));
        if (device == failingDevice)
            break;
    }

    bool noDeviceWillFail = (failingDevice < 0) || (failingDevice >= NUMBER_OF_SX1509_DEVICES);
    ASSERT_EQ(wall->initialize(), noDeviceWillFail);
}
INSTANTIATE_TEST_CASE_P(InitalizationTests, InitFixture, Values(-1, 0, 1, 2, 3));

// I2C multiplexer select vectors
class MuxFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(MuxFixture, TestMultiplexerSelection)
{
    int selectedBus = GetParam(); 
    expectMultiplexerSelectedBus(selectedBus);
    wall->setMultiplexerI2Cbus(selectedBus);
}
INSTANTIATE_TEST_CASE_P(MuxSelectionTests, MuxFixture, Values(0,1,2));


// LED array tests
class LEDFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};

TEST_P(LEDFixture, TurnOnLEDArray)
{
    const int ledDevice = 1;
    int ledArray = GetParam();

    InSequence led_change;
    expectMultiplexerSelectedBus(Wall::IODeviceBus[ledDevice]);
    EXPECT_CALL(*io->accessMockSX1509(ledDevice),
        digitalWrite(ledArray, LED_ON)).Times(1);
    wall->turnOnLEDarray(ledArray);
}
TEST_P(LEDFixture, TurnOffLEDArray)
{
    const int ledDevice = 1;
    int ledArray = GetParam();
    
    InSequence led_change;
    expectMultiplexerSelectedBus(ledDevice);
    EXPECT_CALL(*io->accessMockSX1509(ledDevice),
        digitalWrite(ledArray, LED_OFF)).Times(1);
    wall->turnOffLEDarray(ledArray);
}
INSTANTIATE_TEST_CASE_P(LEDArrayTests, LEDFixture, Values(
    OUTPUT_LED_ARRAY_WHITE_LEFT,
    OUTPUT_LED_ARRAY_WHITE_RIGHT, 
    OUTPUT_LED_ARRAY_WHITE_RIGHT,
    OUTPUT_LED_ARRAY_GREEN_LEFT,
    OUTPUT_LED_ARRAY_GREEN_RIGHT,
    OUTPUT_LED_ARRAY_RED_QUAD_1,
    OUTPUT_LED_ARRAY_RED_QUAD_2,
    OUTPUT_LED_ARRAY_RED_QUAD_3,
    OUTPUT_LED_ARRAY_RED_QUAD_4
    )
);


// Motor tests
class MotorFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};

TEST_F(MotorFixture, TestRunFirstMotorClockwise)
{
    const int motorDevice = 0;

    InSequence run_motor;
    expectMultiplexerSelectedBus(Wall::IODeviceBus[motorDevice]);
    EXPECT_CALL(*io->accessMockSX1509(motorDevice),
        digitalWrite(OUTPUT_MOTOR1_IN1, 1)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(motorDevice),
        digitalWrite(OUTPUT_MOTOR1_IN2, 0)).Times(1);
    expectMultiplexerSelectedBus(Wall::IODeviceBus[motorDevice]);
    EXPECT_CALL(*io->accessMockSX1509(motorDevice),
        analogWrite(OUTPUT_MOTOR1_PWM, 255)).Times(1);

    wall->MotorOneClockwise();
    wall->MotorOneSpeed(255);
}
}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
