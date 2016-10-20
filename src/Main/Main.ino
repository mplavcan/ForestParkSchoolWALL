// Main program for Forest Park School Wall
//
#include <Arduino.h>
#include <Wire.h>
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include "DeviceFactory.h"
#include "Wall.h"

Wall *wall;

void setup() {
    DeviceFactory *io = new DeviceFactory();
    wall = new Wall(io);
}

void loop() {
  
}
