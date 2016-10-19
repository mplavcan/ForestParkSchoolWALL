// Wall hardware device generator object
// Forest Park School Wall 
//
#include "DeviceFactory.h"

SX1509* DeviceFactory::createSX1509Instance(void)
{
    return new SX1509;
}

Adafruit_ADS1015* DeviceFactory::createADS1015Instance(uint8_t addr)
{
    return new Adafruit_PWMServoDriver(addr);
}

Adafruit_PWMServoDriver* DeviceFactory::createPWMinstance(uint8_t addr)
{
    return new Adafruit_PWMServoDriver(addr);
}
