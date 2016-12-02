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


class TestableWall : public Wall
{
public:
    TestableWall* usingFactory(FactoryInterface *io) override { this->Wall::usingFactory(io); return this; }

    using Wall::initializeIOexpanders;
    using Wall::initializeAnalogExpanders;
    using Wall::initializeLEDarrayOutputs;
    using Wall::initializeMotorOutputs;
    using Wall::initializePWMOutputs;
    using Wall::initializeToggleInputs;
    using Wall::initializeJoystickInputs;
    using Wall::resetDigitalIO;
    using Wall::resetAnalogIO;
    using Wall::initializeButtonInOuts;
    using Wall::initalizeELwireOutputs;
    using Wall::initializeLCD;

    using Wall::setMultiplexerForIOexpander;
    using Wall::setMultiplexerForAnalog;
    using Wall::setMultiplexerI2CBus;

    using Wall::ioDeviceBus;
    using Wall::ioDeviceAddress;
    using Wall::analogDeviceAddress;
    using Wall::analogDeviceBus;

    using Wall::normalizedKnobValue;
    using Wall::normalizedSliderValue;

    using Wall::motorControlPin1;
    using Wall::motorControlPin2;
    using Wall::motorPWMpin;
    using Wall::ledArrayPin;
    using Wall::ledArrayNormalizedValue;
    using Wall::greenLEDarrayPin;
    using Wall::whiteLEDarrayPin;
    using Wall::redLEDarrayPin;
    using Wall::indicatorPin;
    using Wall::toggleSwitchPin;
    using Wall::photoSensorPin;
    using Wall::forceSensorPin;
    using Wall::circuitDevice;
    using Wall::circuitPin;
    using Wall::buttonDevice;
    using Wall::buttonPin;
    using Wall::buttonLEDpin;
    using Wall::elWirePin;
};

class WallFixture : public Test {
protected:
    
    ArduinoMock *Intel101;
    MockDeviceFactory *io;
    StrictMock<WireMock> *i2c;
    TestableWall *wall;

    WallFixture() {
        Intel101 = arduinoMockInstance();
        i2c = static_cast<StrictMock<WireMock> *>(WireMockInstance());
        io = new MockDeviceFactory;
        wall = (new TestableWall)->usingFactory(io);
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
