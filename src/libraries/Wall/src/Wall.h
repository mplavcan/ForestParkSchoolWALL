// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"
#include "FactoryInterface.h"

#define NUMBER_OF_SX1509_DEVICES 4

#define IO_EXPANDER_FOR_MOTORS 0
#define IO_EXPANDER_FOR_LED_ARRAYS 1
#define PWM_START_OF_DUTY_CYCLE 0
#define PWM_FULL_DUTY_CYCLE 4096
#define PWM_HALF_DUTY_CYCLE (PWM_FULL_DUTY_CYCLE / 2)

#define FALSE   (0)
#define TRUE    (!FALSE)

typedef enum { BLUE_MOTOR, ORANGE_MOTOR } wall_motor;
typedef enum { GREEN_LED, WHITE_LED, RED_LED } led_array;
typedef enum { LEFT_SIDE, RIGHT_SIDE, LOWER_LEFT_SIDE, LOWER_RIGHT_SIDE } led_section;

class Wall
{
public:
    Wall(FactoryInterface *io);
    bool initialize(void);
    bool initializeIOexpanders(void);
    void initializeLEDarrayOutputs(void);
    void initializeMotorOutputs(void);
    void initializePWMOutputs(void);

    int setMultiplexerForIOexpander(int device);
    int setMultiplexerI2CBus(int bus);
    bool resetIO(int device);
    void turnOnLEDarray(led_array array, led_section section);
    void turnOffLEDarray(led_array array, led_section section);
    void setMotorDirectionClockwise(wall_motor motor);
    void setMotorDirectionCounterClockwise(wall_motor motor);
    void setMotorSpeed(wall_motor motor, uint8_t speed);
    void stopMotor(wall_motor motor);
    void turnTransducerOn(void);
    void turnTransducerOff(void);
    static const int IODeviceBus[NUMBER_OF_SX1509_DEVICES];
    static const int IODeviceAddress[NUMBER_OF_SX1509_DEVICES];

    static int motorControlPin1(wall_motor motor);
    static int motorControlPin2(wall_motor motor);
    static int motorPWMpin(wall_motor motor);
    static int ledArrayPin(led_array array, led_section section);
    static int greenLEDarrayPin(led_section section);
    static int whiteLEDarrayPin(led_section section);
    static int redLEDarrayPin(led_section section);

private:
    SX1509 *io_expander[NUMBER_OF_SX1509_DEVICES];
    Adafruit_PWMServoDriver *pwm;

    bool ledArrayIsActiveLow(led_array array);

};