// Main program for Forest Park School Wall
//
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include "DeviceFactory.h"
#include "Wall.h"
#include "FunWithCircuits.h"

#define ONE_TENTH_SECOND 100
#define ONE_HUNDREDTH_SECOND 10
#define ONE_SECOND 1000

Wall *wall;
FunWithCircuits *app;

unsigned long lightsOnTime = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting setup()");
    Serial.println("Forest Park Circuit Wall starting up");

    wall = (new Wall)->usingFactory(new DeviceFactory);
    app = new FunWithCircuits((new Wall)->usingFactory(new DeviceFactory));

    if (!app->setup())
    {
        while (true)
        {
            Serial.println("Initialization of I2C devices failed");
            delay(ONE_SECOND);
        }
    }
    Serial.println("Exiting setup()");
}

void loop(void)
{
    app->loop(millis());
    delay(ONE_HUNDREDTH_SECOND);
}
