// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"

#define LED_OFF 0
#define LED_ON 1
#define WHITE_LED_ARRAY_LEFT 0
#define WHITE_LED_ARRAY_RIGHT 1


class Wall
{
public:
    Wall::Wall(SX1509* io_one, SX1509* io_two, SX1509* io_three, SX1509* io_four);
    bool Initialize(void);
    int set_multiplexer_i2c_bus(uint8_t bus);
    void ChangeLEDState(int led_selector, int led_on);
private:
    SX1509 *io_expander1;
    SX1509 *io_expander2;
    SX1509 *io_expander3;
    SX1509 *io_expander4;
};