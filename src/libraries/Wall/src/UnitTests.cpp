// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../arduino-mock/Arduino.h"
#include "../../arduino-mock/Wire.h"
#include "../../MockDevices/SparkFunSX1509.h"
#include "Wall.h"
#include <tuple>

namespace testing {

using testing::StrictMock;

class WallFixture : public Test {
protected:
    
    StrictMock<SX1509Mock> 
        *mock_io_expander1, 
        *mock_io_expander2, 
        *mock_io_expander3, 
        *mock_io_expander4 ;
    StrictMock<WireMock> *mock_i2c;
    Wall *wall;

    WallFixture() {
        mock_i2c = static_cast<StrictMock<WireMock> *>(WireMockInstance());
        mock_io_expander1 = static_cast<StrictMock<SX1509Mock> *>(SX1509MockInstance());
        mock_io_expander2 = static_cast<StrictMock<SX1509Mock> *>(SX1509MockInstance());
        mock_io_expander3 = static_cast<StrictMock<SX1509Mock> *>(SX1509MockInstance());
        mock_io_expander4 = static_cast<StrictMock<SX1509Mock> *>(SX1509MockInstance());
        wall = new Wall(mock_io_expander1, mock_io_expander2, mock_io_expander3, mock_io_expander4);
    }

    virtual ~WallFixture() {
        releaseWireMock();
        releaseSX1509Mock(mock_io_expander1);
        releaseSX1509Mock(mock_io_expander2);
        releaseSX1509Mock(mock_io_expander3);
        releaseSX1509Mock(mock_io_expander4);
        delete wall;
    }

    void WallFixture::ExpectMultiplexerSelectedBus(int choice);
};


// I2C multiplexer select vectors
class MuxFixture : public WallFixture, public ::testing::WithParamInterface<int> {
};
TEST_P(MuxFixture, TestMultiplexerSelection)
{
    int bus_selection = GetParam(); 
    ExpectMultiplexerSelectedBus(bus_selection);
    wall->set_multiplexer_i2c_bus(bus_selection);
}
INSTANTIATE_TEST_CASE_P(MuxSelectionTests, MuxFixture, Values(0,1,2));

void WallFixture::ExpectMultiplexerSelectedBus(int bus_choice)
{
    int expected_bus_vector = 1 << bus_choice;
    InSequence mux_bus_selection;
    EXPECT_CALL(*mock_i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*mock_i2c, write(expected_bus_vector));
    EXPECT_CALL(*mock_i2c, endTransmission()).WillOnce(Return(WIRE_TRANSMIT_SUCCESS));
}


// Higher-level Wall tests
//
TEST_F(WallFixture, TestWallInitialization)
{
    InSequence init;
    ExpectMultiplexerSelectedBus(1);
 
    EXPECT_CALL(*mock_io_expander1, 
        begin(SPARKFUN_SX1509_FIRST_I2C_ADDRESS, SPARKFUN_SX1509_RESET_PIN))
        .WillOnce(Return(true));

    ASSERT_EQ(wall->Initialize(), true);
}

// LED array tests
//
class LEDFixture : public WallFixture, public ::testing::WithParamInterface<tuple<int,int>> {
};

TEST_P(LEDFixture, ChangeLedState)
{
    int led_array, led_state;
    std::tie<int,int>(led_array, led_state) = GetParam();
    
    InSequence led_change;
    ExpectMultiplexerSelectedBus(1);
    EXPECT_CALL(*mock_io_expander2,
        digitalWrite(led_array, led_state)).Times(1);
    wall->ChangeLEDState(led_array, led_state);
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
