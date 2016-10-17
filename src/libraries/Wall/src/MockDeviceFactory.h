// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../MockDevices/SparkFunSX1509.h"
#include "../../MockDevices/Adafruit_PWMServoDriver.h"
#include "FactoryInterface.h"

#define MAXIMUM_IO_INSTANCES 10  // arbitrary, must be great than any of "NUMBER_OF_xxxx_DEVICES"

namespace testing {

using testing::StrictMock;

class MockDeviceFactory : public FactoryInterface
{
public:
    MockDeviceFactory() {}
    SX1509* createSX1509Instance();
    StrictMock<SX1509Mock>* accessMockSX1509(int index);

    Adafruit_PWMServoDriver* createPWMinstance(uint8_t addr);
    StrictMock<Adafruit_PWMServoDriverMock>* accessPWM();

    ~MockDeviceFactory()
    {
        for (int device = 0; device < deviceCount; device++)
            releaseSX1509Mock(deviceList[device]);
        releaseAdafruit_PWMServoDriverMock(pwm);
    }

private:
    int deviceCount = 0;
    SX1509Mock* deviceList[MAXIMUM_IO_INSTANCES];
    Adafruit_PWMServoDriverMock* pwm;
};


}; // namespace
