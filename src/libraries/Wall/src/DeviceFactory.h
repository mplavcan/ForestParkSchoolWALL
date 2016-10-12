// Wall hardware device generator object
// Forest Park School Wall 
//

#ifndef __DeviceFactory_H__
#define __DeviceFactory_H__

class FactoryInterface
{
public:
    virtual SX1509* createSX1509Instance() = 0;
    virtual ~FactoryInterface() {};
};


class DeviceFactory : public FactoryInterface
{
    SX1509* createSX1509Instance();
};

#endif
