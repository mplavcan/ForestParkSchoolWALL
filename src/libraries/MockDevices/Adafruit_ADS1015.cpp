// Adafruit_ADS1015.cpp
// Mocked Adafruit ADS1015 Analog Input Library Header File
#include "Adafruit_ADS1015.h"

Adafruit_ADS1015Mock* Adafruit_ADS1015MockInstance(uint8_t i2cAddress)
{
    return new Adafruit_ADS1015Mock();
}

void releaseAdafruit_ADS1015Mock(Adafruit_ADS1015Mock *mock)
{
    delete mock;
}
