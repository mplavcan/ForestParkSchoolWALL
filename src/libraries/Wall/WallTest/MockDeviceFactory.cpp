// Unit tests for Wall object
//
#include "MockDeviceFactory.h"
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include <rgb_lcd.h>


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

Adafruit_ADS1015* MockDeviceFactory::createADS1015Instance(uint8_t addr)
{
    Adafruit_ADS1015Mock* newDevice = Adafruit_ADS1015MockInstance(addr);
    analogExpanderDeviceList[analogExpanderDeviceCount] = static_cast<StrictMock<Adafruit_ADS1015Mock>*>(newDevice);
    analogExpanderDeviceCount++;
    return newDevice;
}

rgb_lcd* MockDeviceFactory::createLCDInstance(void)
{
    lcdDevice = static_cast<StrictMock<GroveLCDMock>*>(GroveLCDMockInstance());
    return lcdDevice;
}
SX1509Mock* MockDeviceFactory::accessMockSX1509(int index)
{
    return ioExpanderDeviceList[index];
}

Adafruit_PWMServoDriverMock* MockDeviceFactory::accessMockPWM(void)
{
    return pwmDevice;
}

Adafruit_ADS1015Mock* MockDeviceFactory::accessMockADS1015(int index)
{
    return analogExpanderDeviceList[index];
}

GroveLCDMock* MockDeviceFactory::accessMockLCD(void)
{
    return lcdDevice;
}

}; // namespace
