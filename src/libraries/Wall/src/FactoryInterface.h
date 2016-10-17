// Wall hardware device generator interface
// Forest Park School Wall 
//

#ifndef __FactoryInterface_H__
#define __FactoryInterface_H__

class FactoryInterface
{
public:
    virtual SX1509* createSX1509Instance() = 0;
    virtual Adafruit_PWMServoDriver* createPWMinstance(uint8_t addr) = 0;
    virtual ~FactoryInterface() {};
};

#endif
