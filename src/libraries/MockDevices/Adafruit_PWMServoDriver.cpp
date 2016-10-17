// Adafruit_PWMServoDriver.h
// Mocked Adafruit 16-channel PWM & Servo driver Library

#include "Adafruit_PWMServoDriver.h"

Adafruit_PWMServoDriverMock* Adafruit_PWMServoDriverMockInstance(uint8_t addr)
{
    return new Adafruit_PWMServoDriverMock(addr);
}
void releaseAdafruit_PWMServoDriverMock(Adafruit_PWMServoDriverMock *mock)
{
    delete mock;
}
