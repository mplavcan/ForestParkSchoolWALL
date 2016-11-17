// Unit tests for Wall object
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include "MockDeviceFactory.h"
#include <Wall.h>

#define WIRE_TRANSMIT_SUCCESS 0

namespace testing {

using testing::StrictMock;

class WallFixture : public Test {
protected:
    
    ArduinoMock *Intel101;
    MockDeviceFactory *io;
    StrictMock<WireMock> *i2c;
    WallImplementation *wall;

    WallFixture() {
        Intel101 = arduinoMockInstance();
        i2c = static_cast<StrictMock<WireMock> *>(WireMockInstance());
        io = new MockDeviceFactory;
        wall = new WallImplementation(io);
    }

    virtual ~WallFixture() {
        delete io;
        delete wall;
        releaseWireMock();
        releaseArduinoMock();
    }

    void expectMultiplexerSelectsSX1509(int device);
    void expectMultiplexerSelectsADS1015(int device);
    void expectMultiplexerSelectedBus(int bus);
};


// Simplification Macros to make tests easier to read
//
#define EXPECT_SX1509_PINMODE(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       pinMode(pin, value));

#define EXPECT_SX1509_DIGITAL_WRITE(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       digitalWrite(pin, value));

#define EXPECT_SX1509_ANALOG_WRITE(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       analogWrite(pin, value));

#define EXPECT_SX1509_DIGITAL_READ(device, pin, value) \
   EXPECT_CALL(*io->accessMockSX1509(device), \
       digitalRead(pin)).WillOnce(Return(value));

#define EXPECT_ADS1015_ANALOG_READ(device, pin, value) \
   EXPECT_CALL(*io->accessMockADS1015(device), \
       readADC_SingleEnded(pin)).WillOnce(Return(value));

}; // namespace
