// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#include "Wall_IO_pins.h"
#include "FactoryInterface.h"

#define PWM_FREQUENCY 2000.0  // Hz
#define PWM_START_OF_DUTY_CYCLE 0
#define PWM_FULL_DUTY_CYCLE 4096
#define PWM_HALF_DUTY_CYCLE (PWM_FULL_DUTY_CYCLE / 2)
#define PWM_INDICATOR_OFF_VALUE 0
#define PWM_INDICATOR_ON_VALUE (PWM_FULL_DUTY_CYCLE - 1)

#define FALSE   (0)
#define TRUE    (!FALSE)

typedef enum { BLUE_MOTOR, ORANGE_MOTOR } wall_motor;
typedef enum { GREEN_LED, WHITE_LED, RED_LED } led_array;
typedef enum { LEFT_SIDE, RIGHT_SIDE, LOWER_LEFT_SIDE, LOWER_RIGHT_SIDE } led_section;
typedef enum
{
    INDICATE_WHITE_LED       = INDICATOR_LED_ARRAY_WHITE,
    INDICATE_BLUE_MOTOR      = INDICATOR_MOTOR_2,
    INDICATE_RED_LED         = INDICATOR_LED_ARRAY_RED,
    INDICATE_ORANGE_MOTOR    = INDICATOR_MOTOR_1,
    INDICATE_TRANSDUCER      = INDICATOR_TRANSDUCER,
    INDICATE_GREEN_LED       = INDICATOR_LED_ARRAY_GREEN,
    INDICATE_NEGATIVE_POLE   = INDICATOR_BATTERY_NEGATIVE,
    INDICATE_KNOB            = INDICATOR_ROTARY_POT,
    INDICATE_SLIDER          = INDICATOR_LINEAR_POT,
    INDICATE_PHOTO_SENSOR    = INDICATOR_PHOTO_SENSOR,
    INDICATE_JOYSTICK        = INDICATOR_JOYSTICK,
    INDICATE_TOGGLES         = INDICATOR_TOGGLE_SWITCH,
    INDICATE_PRESSURE_SENSOR = INDICATOR_FORCE_SENSOR,
    INDICATE_POSITIVE_POLE   = INDICATOR_BATTERY_POSITIVE
} indicator_led;

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
    void turnIndicatorOn(indicator_led lamp);
    void turnIndicatorOff(indicator_led lamp);
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