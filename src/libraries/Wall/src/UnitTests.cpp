// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../arduino-mock/Arduino.h"
#include "../../arduino-mock/Wire.h"
#include "../../MockDevices/SparkFunSX1509.h"
#include "Wall.h"

// I2C multiplexer select vectors
#define I2C_MUX_BUS_ZERO_SELECTED 0x01
#define I2C_MUX_BUS_ONE_SELECTED 0x02
#define WIRE_TRANSMIT_SUCCESS 1

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
};


TEST_F(WallFixture, TestInitializeI2CDevices)
{
  InSequence init;

  EXPECT_CALL(*mock_i2c,
        beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*mock_i2c,
        write(I2C_MUX_BUS_ONE_SELECTED));
    EXPECT_CALL(*mock_i2c,
        endTransmission())
        .WillOnce(Return(WIRE_TRANSMIT_SUCCESS));

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
