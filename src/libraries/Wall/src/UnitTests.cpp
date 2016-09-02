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
    
    SX1509Mock* mock_io_expander;
    WireMock* mock_i2c;
    Wall *wall;

    WallFixture() {
        mock_i2c = WireMockInstance();
        mock_io_expander = SX1509MockInstance();
        wall = new Wall(mock_io_expander);
    }

    virtual ~WallFixture() {
        releaseSX1509Mock();
    }
};


TEST_F(WallFixture, TestInitializeI2CDevices)
{
    EXPECT_CALL(*mock_i2c,
        beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*mock_i2c,
        write(SPARKFUN_SX1509_FIRST_I2C_BUS));
    EXPECT_CALL(*mock_i2c,
        endTransmission())
        .WillOnce(Return(1));

    EXPECT_CALL(*mock_io_expander, 
        begin(SPARKFUN_SX1509_FIRST_I2C_ADDRESS, 255))
        .WillOnce(Return(1));
    ASSERT_EQ(wall->Initialize(), true);
}

}; // namespace

   // Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
