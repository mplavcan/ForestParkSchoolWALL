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

void WallFixture::expectMultiplexerSelectedBus(int device)
{
    int expected_bus_vector = 1 << Wall::IODeviceBus[device];
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
        expectMultiplexerSelectedBus(device);
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

// Active-High LED array tests
class LEDHighFixture : public WallFixture, 
    public ::testing::WithParamInterface<tuple<led_array, led_section>> {
};
TEST_P(LEDHighFixture, TurnOnLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_on;
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_LED_ARRAYS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_LED_ARRAYS),
        digitalWrite(Wall::ledArrayPin(array, section), HIGH)).Times(1);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDHighFixture, TurnOffLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_LED_ARRAYS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_LED_ARRAYS),
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
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_LED_ARRAYS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_LED_ARRAYS),
        digitalWrite(Wall::ledArrayPin(array, section), LOW)).Times(1);
    wall->turnOnLEDarray(array, section);
}
TEST_P(LEDLowFixture, TurnOffLEDArray)
{
    led_array array = get<0>(GetParam());
    led_section section = get<1>(GetParam());

    InSequence led_off;
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_LED_ARRAYS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_LED_ARRAYS),
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
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_MOTORS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        digitalWrite(Wall::motorControlPin1(motor), HIGH)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        digitalWrite(Wall::motorControlPin2(motor), LOW)).Times(1);
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_MOTORS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        analogWrite(Wall::motorPWMpin(motor), speed)).Times(1);

    wall->setMotorDirectionClockwise(motor);
    wall->setMotorSpeed(motor, speed);
}
TEST_P(MotorFixture, TestRunMotorCounterClockwise)
{
    wall_motor motor = GetParam();
    int speed = 522;

    InSequence run_motor;
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_MOTORS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        digitalWrite(Wall::motorControlPin1(motor), LOW)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        digitalWrite(Wall::motorControlPin2(motor), HIGH)).Times(1);
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_MOTORS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        analogWrite(Wall::motorPWMpin(motor), speed)).Times(1);

    wall->setMotorDirectionCounterClockwise(motor);
    wall->setMotorSpeed(motor, speed);
}
TEST_P(MotorFixture, TestStopMotor)
{
    wall_motor motor = GetParam();

    InSequence stop_motor;
    expectMultiplexerSelectedBus(IO_EXPANDER_FOR_MOTORS);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        digitalWrite(Wall::motorControlPin1(motor), LOW)).Times(1);
    EXPECT_CALL(*io->accessMockSX1509(IO_EXPANDER_FOR_MOTORS),
        digitalWrite(Wall::motorControlPin2(motor), LOW)).Times(1);

    wall->stopMotor(motor);
}
INSTANTIATE_TEST_CASE_P(MotorTests, MotorFixture, Values(
    BLUE_MOTOR,
    ORANGE_MOTOR)
);

}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
