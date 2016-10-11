// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"

#define LED_OFF 0
#define LED_ON 1
#define NUMBER_OF_SX1509_DEVICES 4


class Wall
{
public:
    Wall::Wall(SX1509* io_expanders[NUMBER_OF_SX1509_DEVICES]);
    bool Initialize(void);
    int setMultiplexerI2Cbus(uint8_t bus);
    bool resetIO(int device);
    void ChangeLEDState(int led_selector, int led_on);

    static const int IODeviceBus[NUMBER_OF_SX1509_DEVICES];
    static const int IODeviceAddress[NUMBER_OF_SX1509_DEVICES];
    
private:
    SX1509 *io_expander[NUMBER_OF_SX1509_DEVICES];

};