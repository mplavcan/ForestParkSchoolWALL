// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../arduino-mock/Arduino.h"
#include "../../arduino-mock/Wire.h"
#include "../../MockDevices/SparkFunSX1509.h"
#include "DeviceFactory.h"

#define MAXIMUM_IO_INSTANCES 10  // arbitrary, must be great than any of "NUMBER_OF_xxxx_DEVICES"

namespace testing {

using testing::StrictMock;

class MockDeviceFactory : public FactoryInterface
{
public:
    MockDeviceFactory() {}
    SX1509* createSX1509Instance();
    StrictMock<SX1509Mock>* accessMockSX1509(int index);

    ~MockDeviceFactory()
    {
        for (int device = 0; device < deviceCount; device++)
            releaseSX1509Mock(deviceList[device]);
    }

private:
    int deviceCount = 0;
    SX1509Mock* deviceList[MAXIMUM_IO_INSTANCES];
};

}; // namespace
