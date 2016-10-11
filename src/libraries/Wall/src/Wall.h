// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"

#define LED_OFF 0
#define LED_ON 1


class Wall
{
public:
    Wall::Wall(SX1509* io_one, SX1509* io_two, SX1509* io_three, SX1509* io_four);
    bool Initialize(void);
    int set_multiplexer_i2c_bus(uint8_t bus);
    bool resetIO(int device);
    void ChangeLEDState(int led_selector, int led_on);
private:
    SX1509 *io_expander1;
    SX1509 *io_expander2;
    SX1509 *io_expander3;
    SX1509 *io_expander4;

    const int IODeviceBus[4] = {
        SPARKFUN_SX1509_FIRST_I2C_BUS,
        SPARKFUN_SX1509_SECOND_I2C_BUS,
        SPARKFUN_SX1509_THIRD_I2C_BUS,
        SPARKFUN_SX1509_FOURTH_I2C_BUS
    };
    const int IODeviceAddress[4] = {
        SPARKFUN_SX1509_FIRST_I2C_ADDRESS,
        SPARKFUN_SX1509_SECOND_I2C_ADDRESS,
        SPARKFUN_SX1509_THIRD_I2C_ADDRESS,
        SPARKFUN_SX1509_FOURTH_I2C_ADDRESS
    };
};