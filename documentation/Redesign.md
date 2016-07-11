2016 Board Redesign
===================

Over the last two years, the Wall had fallen into disrepair. 
Forest Park Elementary approached the Intel Oregon Makers group in early 2016 to repair the Wall.

With very little circuit documentation, and a large amount of wiring to sort through and test, the group opted to redesign the electrical portions of the Wall.  A quick overview of the previous design:

I/O was distributed through six Arduinos:
* One had a motor shield to handle the motors
* One had a Sparkfun El Escudo Dos to control the EL wire
* Four others handled miscellaneous I/O

The replacement design consolidated the control into a single Arduino that kept the El Escudo Dos for EL control, and handled all other I/O via I2C expansion boards:
* Four Sparkfun SX1509 16-channel digital I/O
* Two Adafruit ADS1015 4-channel 12-bit analog inputs
* One Adafruit TCA9685 16-channel PWM outputs
* One Adafruit TB6612 2-channel motor controller

An Adafruit TCA9548 I2C multiplexer was necessary to avoid address conflicts on the I2C bus.  The group added a SeeedStudio Grove I2C RGB LCD display under a clear hex in the upper left corner of the Wall to provide additional visual feedback.

The consolidation of Arduino code 