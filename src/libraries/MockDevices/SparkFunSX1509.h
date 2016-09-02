// SparkFunSX1509.h
// Mocked SparkFun SX1509 I/O Expander Library Header File

#ifndef SparkFunSX1509_H
#define SparkFunSX1509_H

#include <gmock/gmock.h>

#define RECEIVE_TIMEOUT_VALUE 1000  
#define LINEAR      0   
#define LOGARITHMIC 1
#define INTERNAL_CLOCK_2MHZ 2
#define EXTERNAL_CLOCK  1
#define SOFTWARE_RESET 0
#define HARDWARE_RESET 1
#define ANALOG_OUTPUT 0x3

typedef uint8_t boolean;
typedef uint8_t byte;

// Only declare the methods that the Wall object will use
class SX1509
{
  public:
    SX1509() {}
    virtual byte begin(byte address = 0x3E, byte resetPin = 0xFF) = 0;
    virtual void reset(bool hardware) = 0;
    virtual void pinMode(byte pin, byte inOut) = 0;
    virtual void digitalWrite(byte pin, byte highLow) = 0;
    virtual byte digitalRead(byte pin) = 0;
    virtual void ledDriverInit(byte pin, byte freq = 1, bool log = false) = 0;
    virtual void analogWrite(byte pin, byte iOn) = 0;
    virtual void setupBlink(byte pin, byte tOn, byte toff, byte onIntensity = 255, byte offIntensity = 0, byte tRise = 0, byte tFall = 0, bool log = false) = 0;
    virtual void blink(byte pin, unsigned long tOn, unsigned long tOff, byte onIntensity = 255, byte offIntensity = 0) = 0;
    virtual void breathe(byte pin, unsigned long tOn, unsigned long tOff, unsigned long rise, unsigned long fall, byte onInt = 255, byte offInt = 0, bool log = LINEAR) = 0;
    virtual void sync(void) = 0;
};

// Only mock the methods that the Wall object will use
class SX1509Mock : public SX1509
{
public:
    MOCK_METHOD2(begin, byte(byte, byte));
    MOCK_METHOD1(reset, void (bool));
    MOCK_METHOD2(pinMode, void (byte, byte));
    MOCK_METHOD2(digitalWrite, void (byte, byte));
    MOCK_METHOD1(digitalRead, byte (byte));
    MOCK_METHOD3(ledDriverInit, void (byte, byte, bool));
    MOCK_METHOD2(analogWrite, void (byte, byte));
    MOCK_METHOD8(setupBlink, void (byte, byte, byte, byte, byte, byte, byte, bool));
    MOCK_METHOD5(blink, void (byte, unsigned long, unsigned long, byte, byte));
    MOCK_METHOD8(breathe, void (byte, unsigned long, unsigned long, unsigned long, unsigned long, byte, byte, bool));
    MOCK_METHOD0(sync, void());
};  

SX1509Mock* SX1509MockInstance();
void releaseSX1509Mock();

typedef SX1509 sx1509Class;

#endif  // SX1509_library_H
