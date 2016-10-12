// Wall hardware device generator object
// Forest Park School Wall 
//
#include "DeviceFactory.h"

SX1509* DeviceFactory::createSX1509Instance(void)
{
    return new SX1509;
}
