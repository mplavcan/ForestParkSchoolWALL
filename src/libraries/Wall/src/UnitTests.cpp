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
    EXPECT_CALL(*mock_i2c, endTransmission()).WillRepeatedly(Return(WIRE_TRANSMIT_SUCCESS));
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
class LEDFixture : public WallFixture, public ::testing::WithParamInterface<tuple<int,int>> {
};

TEST_P(LEDFixture, ChangeLedState)
{
    int ledArray, ledState;
    const int ledDevice = 1;
    std::tie<int,int>(ledArray, ledState) = GetParam();
    
    InSequence led_change;
    expectMultiplexerSelectedBus(ledDevice);
    EXPECT_CALL(*io->accessMockSX1509(ledDevice),
        digitalWrite(ledArray, ledState)).Times(1);
    wall->changeLEDState(ledArray, ledState);
}
INSTANTIATE_TEST_CASE_P(LEDArrayTests, LEDFixture, Values(
    std::make_tuple(OUTPUT_LED_ARRAY_WHITE_LEFT, LED_ON),
    std::make_tuple(OUTPUT_LED_ARRAY_WHITE_RIGHT, LED_ON),
    std::make_tuple(OUTPUT_LED_ARRAY_WHITE_RIGHT, LED_OFF),
    std::make_tuple(OUTPUT_LED_ARRAY_GREEN_LEFT, LED_ON),
    std::make_tuple(OUTPUT_LED_ARRAY_GREEN_RIGHT, LED_OFF),
    std::make_tuple(OUTPUT_LED_ARRAY_RED_QUAD_2, LED_ON)
    )
);

}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
