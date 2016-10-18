// Unit tests for Wall object
//
#include <gtest/gtest.h>
#include "MockDeviceFactory.h"

#define MAXIMUM_IO_INSTANCES 10  // arbitrary, must be great than any of "NUMBER_OF_xxxx_DEVICES"

namespace testing  {

using testing::StrictMock;
    
SX1509* MockDeviceFactory::createSX1509Instance(void)
{
    SX1509Mock* newDevice = SX1509MockInstance();
    ioExpanderDeviceList[ioExpanderDeviceCount] = static_cast<StrictMock<SX1509Mock>*>(newDevice);
    ioExpanderDeviceCount++;
    return newDevice;
}

Adafruit_PWMServoDriver* MockDeviceFactory::createPWMinstance(uint8_t addr)
{
    pwmDevice = static_cast<StrictMock<Adafruit_PWMServoDriverMock>*>(Adafruit_PWMServoDriverMockInstance(addr));
    return pwmDevice;
}

SX1509Mock* MockDeviceFactory::accessMockSX1509(int index)
{
    return ioExpanderDeviceList[index];
}

Adafruit_PWMServoDriverMock* MockDeviceFactory::accessMockPWM(void)
{
    return pwmDevice;
}

}; // namespace
