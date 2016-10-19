// Wall hardware device generator object
// Forest Park School Wall 
//
#include "FactoryInterface.h"

#ifndef __DeviceFactory_H__
#define __DeviceFactory_H__

class DeviceFactory : public FactoryInterface
{
    SX1509* createSX1509Instance();
    Adafruit_PWMServoDriver* createPWMinstance(uint8_t addr);
    Adafruit_ADS1015* DeviceFactory::createADS1015Instance(uint8_t addr);
};

#endif
