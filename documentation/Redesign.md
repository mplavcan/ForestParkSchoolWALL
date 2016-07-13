2016 Board Redesign
===================

Over the last two years, the Wall had fallen into disrepair. 
Forest Park Elementary approached the Intel Oregon Makers group in early 2016 to repair the Wall.

With very little circuit documentation, and a large amount of wiring to sort through and test, the group opted to redesign the electrical portions of the Wall.  A quick overview of the previous design:

I/O was distributed through six Arduinos:
* One had a motor shield to handle the motors
* One had a Sparkfun El Escudo Dos to control the EL wire
* Four others handled miscellaneous I/O

Lots of this:
[Bird's nest of Wire](/pictures/old_wiring.jpg)

The replacement design consolidated the control into a single Arduino101, keeping the Sparkfun board for EL control, and handling the other I/O via expansion boards:
* Four Sparkfun SX1509 I2C 16-channel digital I/O
* Two Adafruit ADS1015 I2C 4-channel 12-bit analog inputs
* One Adafruit TCA9685 I2C 16-channel PWM driver outputs
* One Adafruit TB6612 2-channel motor controller
* One Adafruit TCA9548 I2C multiplexer (to avoid multiple devices with the same address conflicting on the I2C bus)

[Reverse side of Wall showing boards](/pictures/Wall_reverse_side_boards.tiff)

The group added a SeeedStudio Grove I2C RGB LCD display under a clear hex in the upper left corner of the Wall to provide additional visual feedback.

The consolidation of Arduino code into a single device allows the easy development a single Wall object in the C++ code that programs can interact with. 