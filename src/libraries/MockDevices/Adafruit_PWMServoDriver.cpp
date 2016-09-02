// Adafruit_PWMServoDriver.h
// Mocked Adafruit 16-channel PWM & Servo driver Library

#include "Adafruit_PWMServoDriver.h"

MockAdafruit_PWMServoDriver* Adafruit_PWMServoDriverMockInstance(uint8_t addr)
{
    return new MockAdafruit_PWMServoDriver();
}
void releaseAdafruit_PWMServoDriverMock()
{
    
}
