// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"
#include "FactoryInterface.h"

#define LOW 0
#define HIGH 1

#define NUMBER_OF_SX1509_DEVICES 4

#define IO_EXPANDER_FOR_MOTORS 0
#define IO_EXPANDER_FOR_LED_ARRAYS 1

typedef enum { BLUE_MOTOR, ORANGE_MOTOR } wall_motor;

class Wall
{
public:
    Wall(FactoryInterface *io);
    bool initialize(void);
    int setMultiplexerI2Cbus(int device);
    bool resetIO(int device);
    void turnOnLEDarray(int led_selector);
    void turnOffLEDarray(int led_selector);
    void setMotorDirectionClockwise(wall_motor motor);
    void setMotorDirectionCounterClockwise(wall_motor motor);
    void setMotorSpeed(wall_motor motor, int speed);
    void stopMotor(wall_motor motor);
    static const int IODeviceBus[NUMBER_OF_SX1509_DEVICES];
    static const int IODeviceAddress[NUMBER_OF_SX1509_DEVICES];

    static int motorControlPin1(wall_motor motor);
    static int motorControlPin2(wall_motor motor);
    static int motorPWMpin(wall_motor motor);

private:
    SX1509 *io_expander[NUMBER_OF_SX1509_DEVICES];

    bool Wall::ledArrayIsActiveLow(int ledSelector);

};