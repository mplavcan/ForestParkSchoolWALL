// Unit tests for Wall object
//
#include <gtest/gtest.h>

#include "../../MockDevices/SparkFunSX1509.h"
#include "../../MockDevices/Adafruit_PWMServoDriver.h"
#include "../../MockDevices/Adafruit_ADS1015.h"
#include "FactoryInterface.h"

#define MAXIMUM_IO_INSTANCES 10  // arbitrary, must be great than any of "NUMBER_OF_xxxx_DEVICES"

namespace testing {

using testing::StrictMock;

class MockDeviceFactory : public FactoryInterface
{
public:
    MockDeviceFactory() {}
    SX1509* createSX1509Instance(void);
    Adafruit_ADS1015* createADS1015Instance(uint8_t addr);
    Adafruit_PWMServoDriver* createPWMinstance(uint8_t addr);

    SX1509Mock* accessMockSX1509(int index);
    Adafruit_ADS1015Mock* accessMockADS1015(int index);
    Adafruit_PWMServoDriverMock* accessMockPWM(void);

    ~MockDeviceFactory()
    {
        for (int device = 0; device < ioExpanderDeviceCount; device++)
            releaseSX1509Mock(ioExpanderDeviceList[device]);
        for (int device = 0; device < analogExpanderDeviceCount; device++)
            releaseAdafruit_ADS1015Mock(analogExpanderDeviceList[device]);
        releaseAdafruit_PWMServoDriverMock(pwmDevice);
    }

private:
    int ioExpanderDeviceCount = 0;
    StrictMock<SX1509Mock>* ioExpanderDeviceList[MAXIMUM_IO_INSTANCES];

    int analogExpanderDeviceCount = 0;
    StrictMock<Adafruit_ADS1015Mock>* analogExpanderDeviceList[MAXIMUM_IO_INSTANCES];

    StrictMock<Adafruit_PWMServoDriverMock>* pwmDevice;
};


}; // namespace
