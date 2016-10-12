// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"
#include "DeviceFactory.h"

#define LED_OFF 0
#define LED_ON 1
#define NUMBER_OF_SX1509_DEVICES 4


class Wall
{
public:
    Wall(FactoryInterface *io);
    bool initialize(void);
    int setMultiplexerI2Cbus(uint8_t bus);
    bool resetIO(int device);
    void changeLEDState(int led_selector, int led_on);

    static const int IODeviceBus[NUMBER_OF_SX1509_DEVICES];
    static const int IODeviceAddress[NUMBER_OF_SX1509_DEVICES];
    
private:
    SX1509 *io_expander[NUMBER_OF_SX1509_DEVICES];

};