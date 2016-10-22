// Wall hardware device generator interface
// Forest Park School Wall 
//

#ifndef __FactoryInterface_H__
#define __FactoryInterface_H__

class SX1509;
class Adafruit_ADS1015;
class Adafruit_PWMServoDriver;
class rgb_lcd;

class FactoryInterface
{
public:
    virtual SX1509* createSX1509Instance() = 0;
    virtual Adafruit_ADS1015* createADS1015Instance(unsigned char addr) = 0;
    virtual Adafruit_PWMServoDriver* createPWMinstance(unsigned char addr) = 0;
    virtual rgb_lcd* createLCDInstance(void) = 0;
    virtual ~FactoryInterface() {};
};

#endif
