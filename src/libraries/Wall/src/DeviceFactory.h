// Wall hardware device generator object
// Forest Park School Wall 
//
#include "FactoryInterface.h"

#ifndef __DeviceFactory_H__
#define __DeviceFactory_H__

class DeviceFactory : public FactoryInterface
{
    SX1509* createSX1509Instance();
};

#endif
