// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#pragma once
#include "Wall_IO_pins.h"
#include "../../MockDevices/SparkFunSX1509.h"

class Wall
{
public:
    Wall(SX1509 *io_one);
    bool Initialize(void);

private:

    SX1509 *io_expander;
};