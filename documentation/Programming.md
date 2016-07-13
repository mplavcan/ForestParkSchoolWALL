Programming
============

Arduino and Friends
-------------------
The Wall is powered by a single [Intel Arduino 101 "Curie"](https://www.arduino.cc/en/Guide/Arduino101)
 assisted by several I/O expansion boards.  The libraries for these  will need to be included in any sketch in order for the Wall to function correctly:
* Sparkfun SX1509 IO Expander
* Adafruit ADS1X15
* Adafruit PWM Servo Driver Library
* Seeedstudio Grove I2C (It is not currently downloadable in the Libary manager, so the files for are included in libraries directory of this repo.


Toolchain
---------
While the stock Arduino IDE can be used, the code in this repository was developed using the following tools:
* [Microsoft Visual Studio Community Edition](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx)
* [Visual Micro](http://www.visualmicro.com/)
* [Google Test](https://github.com/google/googletest)
* [Jetbrains Resharper C++](https://www.jetbrains.com/resharper-cpp/)




