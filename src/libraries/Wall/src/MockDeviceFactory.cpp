// Unit tests for Wall object
//
#include <gtest/gtest.h>
#include "MockDeviceFactory.h"

#define MAXIMUM_IO_INSTANCES 10  // arbitrary, must be great than any of "NUMBER_OF_xxxx_DEVICES"

namespace testing  {

SX1509* MockDeviceFactory::createSX1509Instance()
{
    StrictMock<SX1509Mock>* newDevice = static_cast<StrictMock<SX1509Mock>*>(SX1509MockInstance());
    deviceList[deviceCount] = newDevice;
    deviceCount++;
    return static_cast<SX1509*>(newDevice);
}

StrictMock<SX1509Mock>* MockDeviceFactory::accessMockSX1509(int index)
{
    return static_cast<StrictMock<SX1509Mock>*>(deviceList[index]);
}

Adafruit_PWMServoDriver* MockDeviceFactory::createPWMinstance(uint8_t addr)
{
    StrictMock<Adafruit_PWMServoDriverMock>* newDevice =
        static_cast<StrictMock<Adafruit_PWMServoDriverMock>*>(Adafruit_PWMServoDriverMockInstance(addr));
    pwm = newDevice;
    return static_cast<Adafruit_PWMServoDriver*>(newDevice);
}

StrictMock<Adafruit_PWMServoDriverMock>* MockDeviceFactory::accessPWM()
{
    return static_cast<StrictMock<Adafruit_PWMServoDriverMock>*>(pwm);
}

}; // namespace
