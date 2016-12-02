// Timer class for events
//
#include <Arduino.h>
#include "Timer.h"

Timer::Timer(unsigned long futureMillis)
{
    unsigned long now = millis();
    this->expirationTime = now + futureMillis;
}

bool Timer::expired()
{
    unsigned long now = millis();
    return now >= this->expirationTime;
}
