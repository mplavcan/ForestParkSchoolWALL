Programming
============

Arduino and Friends
-------------------
The Wall is powered by a single [Intel Arduino 101 "Curie"](https://www.arduino.cc/en/Guide/Arduino101)
 assisted by several I/O expansion boards.  The libraries for these  will need to be included in any sketch in order for the Wall to function correctly:
* [Sparkfun SX1509 IO Expander](https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library)
* [Adafruit ADS1X15](https://github.com/nipoutch/Adafruit_ADS1015)
* [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)
* [Seeedstudio Grove I2C](https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight)

Toolchain
---------
While the stock Arduino IDE can be used, the code in this repository was developed using the following tools:
* [Microsoft Visual Studio Community Edition](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx)
* [Visual Micro](http://www.visualmicro.com/)
* [Google Test](https://github.com/google/googletest)
* [Jetbrains Resharper C++](https://www.jetbrains.com/resharper-cpp/)


Repository Setup
----------------
The tests also use the [Arduino-mock library](https://github.com/balp/arduino-mock), which is installed as a Git sub-repository in this repo.  To install it, perform these steps after cloning:

```
   git submodule init
   git submodule update
```


