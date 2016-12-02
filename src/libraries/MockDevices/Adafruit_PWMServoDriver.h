// Adafruit_PWMServoDriver.h
// Mocked Adafruit 16-channel PWM & Servo driver Library Header file

#ifndef _ADAFRUIT_PWMServoDriver_H
#define _ADAFRUIT_PWMServoDriver_H

#include <gmock/gmock.h>

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

class Adafruit_PWMServoDriver 
{
public:
    Adafruit_PWMServoDriver(uint8_t addr = 0x40) {}
    virtual void begin(void) = 0;
    virtual void reset(void) = 0;
    virtual void setPWMFreq(float freq) = 0;
    virtual void setPWM(uint8_t num, uint16_t on, uint16_t off) = 0;
    virtual void setPin(uint8_t num, uint16_t val, bool invert = false) = 0;
};


class Adafruit_PWMServoDriverMock : public Adafruit_PWMServoDriver 
{
public:
    Adafruit_PWMServoDriverMock(uint8_t addr) {}
    MOCK_METHOD0(begin, void (void));
    MOCK_METHOD0(reset, void (void));
    MOCK_METHOD1(setPWMFreq, void (float freq));
    MOCK_METHOD3(setPWM, void (uint8_t num, uint16_t on, uint16_t off));
    MOCK_METHOD3(setPin, void (uint8_t num, uint16_t val, bool invert));
};


Adafruit_PWMServoDriverMock* Adafruit_PWMServoDriverMockInstance(uint8_t addr);
void releaseAdafruit_PWMServoDriverMock(Adafruit_PWMServoDriverMock *mock);

#endif
