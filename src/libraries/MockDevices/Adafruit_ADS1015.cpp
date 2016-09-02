// Adafruit_ADS1015.cpp
// Mocked Adafruit ADS1015 Analog Input Library Header File
#include "Adafruit_ADS1015.h"

MockAdafruit_ADS1015* Adafruit_ADS1015MockInstance(uint8_t i2cAddress)
{
    return new MockAdafruit_ADS1015();
}

void releaseAdafruit_ADS1015Mock()
{
    
}
