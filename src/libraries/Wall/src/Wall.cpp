// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//

#include "Wall.h"

Wall::Wall(SX1509 *io_one)
{
    this->io_expander = io_one;
}

bool Wall::Initialize()
{
    return io_expander->begin(SPARKFUN_SX1509_FIRST_I2C_ADDRESS);
}


