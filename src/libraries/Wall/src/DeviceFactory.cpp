// Wall hardware device generator object
// Forest Park School Wall 
//
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include <rgb_lcd.h>
#include "DeviceFactory.h"

SX1509* DeviceFactory::createSX1509Instance(void)
{
    return new SX1509;
}

Adafruit_ADS1015* DeviceFactory::createADS1015Instance(unsigned char addr)
{
    return new Adafruit_ADS1015(addr);
}

Adafruit_PWMServoDriver* DeviceFactory::createPWMinstance(unsigned char addr)
{
    return new Adafruit_PWMServoDriver(addr);
}

rgb_lcd* DeviceFactory::createLCDInstance()
{
    return new rgb_lcd;
}

