// Wall hardware device generator object
// Forest Park School Wall 
//
#include "FactoryInterface.h"

#ifndef __DeviceFactory_H__
#define __DeviceFactory_H__

class DeviceFactory : public FactoryInterface
{
    SX1509* createSX1509Instance();
    Adafruit_PWMServoDriver* createPWMinstance(unsigned char addr);
    Adafruit_ADS1015* createADS1015Instance(unsigned char addr);
};

#endif
