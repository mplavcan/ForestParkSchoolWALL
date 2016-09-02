// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"

class Wall
{
public:
    Wall(SX1509 *io_one);
    bool Initialize(void);
    int set_multiplexer_i2c_bus(uint8_t bus);

private:
    SX1509 *io_expander;
};