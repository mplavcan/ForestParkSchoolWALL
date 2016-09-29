// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../arduino-mock/Arduino.h"
#include "../../arduino-mock/Wire.h"
#include "../../MockDevices/SparkFunSX1509.h"
#include "Wall.h"

namespace testing {

using testing::StrictMock;

class WallFixture : public Test {
protected:
    
    StrictMock<SX1509Mock> *mock_io_expander;
    StrictMock<WireMock> *mock_i2c;
    Wall *wall;

    WallFixture() {
        mock_i2c = static_cast<StrictMock<WireMock> *>(WireMockInstance());
        mock_io_expander = static_cast<StrictMock<SX1509Mock> *>(SX1509MockInstance());
        wall = new Wall(mock_io_expander);
    }

    virtual ~WallFixture() {
        releaseWireMock();
        releaseSX1509Mock(mock_io_expander);
        delete wall;
    }

    void WallFixture::expect_multiplexer_choice(int choice);
};


// I2C multiplexer select vectors
#define I2C_MUX_BUS_ZERO_SELECTED 0x01
#define I2C_MUX_BUS_ONE_SELECTED 0x02
#define I2C_MUX_BUS_TWO_SELECTED 0x04
#define WIRE_TRANSMIT_SUCCESS 1

TEST_F(WallFixture, TestMultiplexerSelectsBus0)
{
    expect_multiplexer_choice(I2C_MUX_BUS_ZERO_SELECTED);
    wall->set_multiplexer_i2c_bus(0);
}

TEST_F(WallFixture, TestMultiplexerSelectsBus1)
{
    expect_multiplexer_choice(I2C_MUX_BUS_ONE_SELECTED);
    wall->set_multiplexer_i2c_bus(1);
}
TEST_F(WallFixture, TestMultiplexerSelectsBus2)
{
    expect_multiplexer_choice(I2C_MUX_BUS_TWO_SELECTED);
    wall->set_multiplexer_i2c_bus(2);
}

void WallFixture::expect_multiplexer_choice(int choice)
{
    InSequence mux_bus_selection;

    EXPECT_CALL(*mock_i2c,
        beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*mock_i2c,
        write(choice));
    EXPECT_CALL(*mock_i2c,
        endTransmission())
        .WillOnce(Return(WIRE_TRANSMIT_SUCCESS));
}


// Higher-level Wall tests
//
TEST_F(WallFixture, TestWallInitialization)
{
    InSequence init;
    expect_multiplexer_choice(I2C_MUX_BUS_ONE_SELECTED);
 
    EXPECT_CALL(*mock_io_expander, 
        begin(SPARKFUN_SX1509_FIRST_I2C_ADDRESS, SPARKFUN_SX1509_RESET_PIN))
        .WillOnce(Return(true));

    ASSERT_EQ(wall->Initialize(), true);
}

}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
