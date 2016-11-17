// Unit tests for Wall object
//
#include "Wallfixture.h"
#include <Wall.h>

namespace testing {

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
    const int expectedBusVector = 1 << bus;
    InSequence mux_bus_selection;
    EXPECT_CALL(*i2c, beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS));
    EXPECT_CALL(*i2c, write(expectedBusVector));
    EXPECT_CALL(*i2c, endTransmission()).WillOnce(Return(WIRE_TRANSMIT_SUCCESS));
}

}; // namespace

// Entry point for Google Test
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
