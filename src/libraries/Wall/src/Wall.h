// Wall hardware abstraction layer object
// Forest Park School Wall 
//
#ifndef _WALL_H_
#define _WALL_H_

#include "Wall_IO_pins.h"
#include "FactoryInterface.h"

#define WALL_PWM_FREQUENCY 880.0  // Hz
#define PWM_START_OF_DUTY_CYCLE 0
#define PWM_FULL_DUTY_CYCLE 4096
#define PWM_HALF_DUTY_CYCLE (PWM_FULL_DUTY_CYCLE / 2)
#define PWM_INDICATOR_OFF_VALUE 0
#define PWM_INDICATOR_ON_VALUE (PWM_FULL_DUTY_CYCLE - 1)
#define MAXIMUM_12BIT_VALUE ((1<<12)-1)
#define MINIMUM_ANALOG_OUTPUT_VALUE 0x00
#define MAXIMUM_ANALOG_OUTPUT_VALUE 0xFF

#define FALSE   (0)
#define TRUE    (!FALSE)

#define NUMBER_INPUT_HEXES 6
#define NUMBER_OUTPUT_HEXES 6

typedef enum
{
    KNOB_HEX, 
    SLIDER_HEX, 
    PHOTO_SENSOR_HEX, 
    JOYSTICK_HEX, 
    TOGGLE_SWITCH_HEX, 
    TOUCH_SENSOR_HEX,
    NO_INPUT = -1
} input_hex;
typedef enum
{
    WHITE_LED_HEX, 
    BLUE_MOTOR_HEX, 
    RED_LED_HEX, 
    ORANGE_MOTOR_HEX, 
    GREEN_LED_HEX, 
    TRANSDUCER_HEX,
    NO_OUTPUT = -1
} output_hex;

typedef enum { BLUE_MOTOR, ORANGE_MOTOR } wall_motor;
typedef enum { GREEN_LED, WHITE_LED, RED_LED } led_array;
typedef enum { LEFT_SIDE, RIGHT_SIDE, LOWER_LEFT_SIDE, LOWER_RIGHT_SIDE } led_section;
typedef enum { LEFT_TOGGLE, CENTER_TOGGLE, RIGHT_TOGGLE } toggle_switch;
typedef enum { LEFT_PHOTO, CENTER_PHOTO, RIGHT_PHOTO } photo_sensor;
typedef enum { LEFT_TOUCH, BOTTOM_TOUCH, RIGHT_TOUCH } force_sensor;
typedef enum { BLUE_BUTTON, YELLOW_BUTTON, GREEN_BUTTON, RED_BUTTON, WHITE_BUTTON } large_button;

typedef enum 
{ 
    RED_WIRE_ONE,
    RED_WIRE_TWO,
    GREEN_WIRE_ONE,
    GREEN_WIRE_TWO,
    YELLOW_WIRE,
    WHITE_WIRE,
    BLUE_WIRE_ONE,
    BLUE_WIRE_TWO
} EL_wire;

typedef enum
{
    INDICATE_WHITE_LED,
    INDICATE_BLUE_MOTOR,
    INDICATE_RED_LED,
    INDICATE_ORANGE_MOTOR,
    INDICATE_TRANSDUCER,
    INDICATE_GREEN_LED,
    INDICATE_KNOB,
    INDICATE_SLIDER,
    INDICATE_PHOTO_SENSOR,
    INDICATE_JOYSTICK,
    INDICATE_TOGGLES,
    INDICATE_TOUCH_SENSOR,
    INDICATE_POSITIVE_POLE,
    INDICATE_NEGATIVE_POLE,
    NO_INDICATION = -1
} indicator_led;

typedef enum
{
    CIRCUIT_KNOB_LEFT,
    CIRCUIT_KNOB_RIGHT,
    CIRCUIT_SLIDER_LEFT,
    CIRCUIT_SLIDER_RIGHT,
    CIRCUIT_PHOTO_LEFT,
    CIRCUIT_PHOTO_RIGHT,
    CIRCUIT_JOYSTICK_LEFT,
    CIRCUIT_JOYSTICK_RIGHT,
    CIRCUIT_TOGGLE_LEFT,
    CIRCUIT_TOGGLE_RIGHT,
    CIRCUIT_TOUCH_LEFT,
    CIRCUIT_TOUCH_RIGHT,
    CIRCUIT_BLUE_MOTOR_LEFT,
    CIRCUIT_BLUE_MOTOR_RIGHT,
    CIRCUIT_ORANGE_MOTOR_LEFT,
    CIRCUIT_ORANGE_MOTOR_RIGHT,
    CIRCUIT_TRANSDUCER_LEFT,
    CIRCUIT_TRANSDUCER_RIGHT,
    CIRCUIT_WHITE_LED_LEFT,
    CIRCUIT_WHITE_LED_RIGHT,
    CIRCUIT_GREEN_LED_LEFT,
    CIRCUIT_GREEN_LED_RIGHT,
    CIRCUIT_RED_LED_LEFT,
    CIRCUIT_RED_LED_RIGHT,
    CIRCUIT_POSITIVE_POLE,
    CIRCUIT_NEGATIVE_POLE,
    NO_CIRCUIT = -1
} circuit_end;


class WallImplementation
{
public:
    WallImplementation(FactoryInterface *io);
    bool initialize(void);
    bool initializeIOexpanders(void);
    void initializeAnalogExpanders(void);
    void initializeLEDarrayOutputs(void);
    void initializeMotorOutputs(void);
    void initializePWMOutputs(void);
    void initializeToggleInputs(void);  
    void initializeJoystickInputs(void);  
    bool resetDigitalIO(int device);
    void resetAnalogIO(int device);
    void initializeButtonInOuts(void);
    void initalizeELwireOutputs(void);
    void initializeLCD(void);

    void setMultiplexerForIOexpander(int device);
    void setMultiplexerForAnalog(int device);
    void setMultiplexerI2CBus(int bus);
    
    void turnOnLEDarray(led_array array, led_section section);
    void turnOffLEDarray(led_array array, led_section section);
    void setLEDarrayBrightness(led_array array, led_section section, unsigned char brightness);
    void setMotorDirectionClockwise(wall_motor motor);
    void setMotorDirectionCounterClockwise(wall_motor motor);
    void setMotorSpeed(wall_motor motor, uint8_t speed);
    void stopMotor(wall_motor motor);
    void turnTransducerOn(void);
    void turnTransducerOff(void);
    void turnIndicatorOn(indicator_led lamp);
    void turnIndicatorOff(indicator_led lamp);
    void setIndicatorBrightness(indicator_led lamp, uint16_t value);
    bool isToggleOn(toggle_switch toggle);
    bool isJoystickUp(void);
    bool isJoystickDown(void);
    bool isJoystickLeft(void);
    bool isJoystickRight(void);
    uint16_t getKnobPosition(void);
    uint16_t getSliderPosition(void);
    uint16_t getPhotoSensorValue(photo_sensor sensor);
    uint16_t getTouchSensorValue(force_sensor sensor);
    int readCircuitState(circuit_end end);
    void resetCircuitInputs(void);
    void setCircuitAsInput(circuit_end end);
    void setCircuitAsOutput(circuit_end end);
    bool isCircuitConnected(circuit_end A, circuit_end B);
    bool isButtonDepressed(large_button button);
    void illuminateButton(large_button button);
    void extinguishButton(large_button button);
    void illuminateELWire(EL_wire line);
    void extinguishELWire(EL_wire line);
    void lcdPrintAt(uint8_t column, uint8_t row, const char buf[]);
    void lcdSetBacklightColor(uint8_t red, uint8_t green, uint8_t blue);
    void clearLCDscreen(void);

    static indicator_led indicatorforInput(input_hex hex);
    static indicator_led indicatorForOutput(output_hex hex);
    static circuit_end leftCircuitForInput(input_hex hex);
    static circuit_end rightCircuitForInput(input_hex hex);
    static circuit_end leftCircuitForOutput(output_hex hex);
    static circuit_end rightCircuitForOutput(output_hex hex);

    static const int ioDeviceBus[NUMBER_OF_SX1509_DEVICES];
    static const int ioDeviceAddress[NUMBER_OF_SX1509_DEVICES];
    static const int analogDeviceAddress[NUMBER_OF_ADS1015_DEVICES];
    static const int analogDeviceBus[NUMBER_OF_ADS1015_DEVICES];

    static uint16_t normalizedKnobValue(uint16_t sliderValue);
    uint16_t normalizedSliderValue(uint16_t sliderValue);

    static int motorControlPin1(wall_motor motor);
    static int motorControlPin2(wall_motor motor);
    static int motorPWMpin(wall_motor motor);
    static int ledArrayPin(led_array array, led_section section);
    static unsigned char ledArrayNormalizedValue(led_array array, unsigned char value);
    static int greenLEDarrayPin(led_section section);
    static int whiteLEDarrayPin(led_section section);
    static int redLEDarrayPin(led_section section);
    static int indicatorPin(indicator_led lamp);
    static int toggleSwitchPin(toggle_switch toggle);
    static int photoSensorPin(photo_sensor sensor);
    static int forceSensorPin(force_sensor sensor);
    static int circuitDevice(circuit_end end);
    static int circuitPin(circuit_end end);
    static int buttonDevice(large_button button);
    static int buttonPin(large_button button);
    static int buttonLEDpin(large_button button);
    static int elWirePin(EL_wire line);
 
private:
    SX1509 *io_expander[NUMBER_OF_SX1509_DEVICES];
    Adafruit_ADS1015 *analog_expander[NUMBER_OF_ADS1015_DEVICES];
    Adafruit_PWMServoDriver *pwm;
    rgb_lcd *lcd;

    uint16_t lastSliderPosition;
    
    int writeMultiplexerForBus(int bus);
};


class Wall: private WallImplementation
{
public:
    Wall(FactoryInterface *io): WallImplementation(io) {}
    using WallImplementation::initialize;
    using WallImplementation::turnOnLEDarray;
    using WallImplementation::turnOffLEDarray;
    using WallImplementation::setLEDarrayBrightness;
    using WallImplementation::setMotorDirectionClockwise;
    using WallImplementation::setMotorDirectionCounterClockwise;
    using WallImplementation::setMotorSpeed;
    using WallImplementation::stopMotor;
    using WallImplementation::turnTransducerOn;
    using WallImplementation::turnTransducerOff;
    using WallImplementation::turnIndicatorOn;
    using WallImplementation::turnIndicatorOff;
    using WallImplementation::setIndicatorBrightness;
    using WallImplementation::isToggleOn;
    using WallImplementation::isJoystickUp;
    using WallImplementation::isJoystickDown;
    using WallImplementation::isJoystickLeft;
    using WallImplementation::isJoystickRight;
    using WallImplementation::getKnobPosition;
    using WallImplementation::getSliderPosition;
    using WallImplementation::getPhotoSensorValue;
    using WallImplementation::getTouchSensorValue;
    using WallImplementation::readCircuitState;
    using WallImplementation::resetCircuitInputs;
    using WallImplementation::setCircuitAsInput;
    using WallImplementation::setCircuitAsOutput;
    using WallImplementation::isCircuitConnected;
    using WallImplementation::isButtonDepressed;
    using WallImplementation::illuminateButton;
    using WallImplementation::extinguishButton;
    using WallImplementation::illuminateELWire;
    using WallImplementation::extinguishELWire;
    using WallImplementation::lcdPrintAt;
    using WallImplementation::lcdSetBacklightColor;
    using WallImplementation::clearLCDscreen;

    using WallImplementation::indicatorforInput;
    using WallImplementation::indicatorForOutput;
    using WallImplementation::leftCircuitForInput;
    using WallImplementation::rightCircuitForInput;
    using WallImplementation::leftCircuitForOutput;
    using WallImplementation::rightCircuitForOutput;
};

#endif // _WALL_H_