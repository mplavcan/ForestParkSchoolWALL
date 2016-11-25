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


class WallInterface
{
public:
    virtual ~WallInterface() {}
    virtual WallInterface* usingFactory(FactoryInterface *io) = 0;
    virtual bool initialize(void) = 0;
    virtual void turnOnLEDarray(led_array array, led_section section) = 0;
    virtual void turnOffLEDarray(led_array array, led_section section) = 0;
    virtual void setLEDarrayBrightness(led_array array, led_section section, unsigned char brightness) = 0;
    virtual void setMotorDirectionClockwise(wall_motor motor) = 0;
    virtual void setMotorDirectionCounterClockwise(wall_motor motor) = 0;
    virtual void setMotorSpeed(wall_motor motor, unsigned char speed) = 0;
    virtual void stopMotor(wall_motor motor) = 0;
    virtual void turnTransducerOn(void) = 0;
    virtual void turnTransducerOff(void) = 0;
    virtual void turnIndicatorOn(indicator_led lamp) = 0;
    virtual void turnIndicatorOff(indicator_led lamp) = 0;
    virtual void setIndicatorBrightness(indicator_led lamp, unsigned int value) = 0;
    virtual bool isToggleOn(toggle_switch toggle) = 0;
    virtual bool isJoystickUp(void) = 0;
    virtual bool isJoystickDown(void) = 0;
    virtual bool isJoystickLeft(void) = 0;
    virtual bool isJoystickRight(void) = 0;
    virtual unsigned int getKnobPosition(void) = 0;
    virtual unsigned int getSliderPosition(void) = 0;
    virtual unsigned int getPhotoSensorValue(photo_sensor sensor) = 0;
    virtual unsigned int getTouchSensorValue(force_sensor sensor) = 0;
    virtual int readCircuitState(circuit_end end) = 0;
    virtual void resetCircuitInputs(void) = 0;
    virtual void setCircuitAsInput(circuit_end end) = 0;
    virtual void setCircuitAsOutput(circuit_end end) = 0;
    virtual bool isCircuitConnected(circuit_end A, circuit_end B) = 0;
    virtual bool isButtonDepressed(large_button button) = 0;
    virtual void illuminateButton(large_button button) = 0;
    virtual void extinguishButton(large_button button) = 0;
    virtual void illuminateELWire(EL_wire line) = 0;
    virtual void extinguishELWire(EL_wire line) = 0;
    virtual void lcdPrintAt(unsigned char column, unsigned char row, const char buf[]) = 0;
    virtual void lcdSetBacklightColor(unsigned char red, unsigned char green, unsigned char blue) = 0;
    virtual void clearLCDscreen(void) = 0;

    virtual indicator_led indicatorforInput(input_hex hex) = 0;
    virtual indicator_led indicatorForOutput(output_hex hex) = 0;
    virtual circuit_end leftCircuitForInput(input_hex hex) = 0;
    virtual circuit_end rightCircuitForInput(input_hex hex) = 0;
    virtual circuit_end leftCircuitForOutput(output_hex hex) = 0;
    virtual circuit_end rightCircuitForOutput(output_hex hex) = 0;
};

class Wall : public WallInterface
{
public:
    Wall() : pwm(nullptr),lcd(nullptr),lastSliderPosition(1024) {}
    Wall* usingFactory(FactoryInterface *io) override;
    bool initialize(void) override;
protected:
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
    
public:
    void turnOnLEDarray(led_array array, led_section section) override;
    void turnOffLEDarray(led_array array, led_section section) override;
    void setLEDarrayBrightness(led_array array, led_section section, unsigned char brightness) override;
    void setMotorDirectionClockwise(wall_motor motor) override;
    void setMotorDirectionCounterClockwise(wall_motor motor) override;
    void setMotorSpeed(wall_motor motor, unsigned char speed) override;
    void stopMotor(wall_motor motor) override;
    void turnTransducerOn(void) override;
    void turnTransducerOff(void) override;
    void turnIndicatorOn(indicator_led lamp) override;
    void turnIndicatorOff(indicator_led lamp) override;
    void setIndicatorBrightness(indicator_led lamp, unsigned int value) override;
    bool isToggleOn(toggle_switch toggle) override;
    bool isJoystickUp(void) override;
    bool isJoystickDown(void) override;
    bool isJoystickLeft(void) override;
    bool isJoystickRight(void) override;
    unsigned int getKnobPosition(void) override;
    unsigned int getSliderPosition(void) override;
    unsigned int getPhotoSensorValue(photo_sensor sensor) override;
    unsigned int getTouchSensorValue(force_sensor sensor) override;
    int readCircuitState(circuit_end end) override;
    void resetCircuitInputs(void) override;
    void setCircuitAsInput(circuit_end end) override;
    void setCircuitAsOutput(circuit_end end) override;
    bool isCircuitConnected(circuit_end A, circuit_end B) override;
    bool isButtonDepressed(large_button button) override;
    void illuminateButton(large_button button) override;
    void extinguishButton(large_button button) override;
    void illuminateELWire(EL_wire line) override;
    void extinguishELWire(EL_wire line) override;
    void lcdPrintAt(unsigned char column, unsigned char row, const char buf[]) override;
    void lcdSetBacklightColor(unsigned char red, unsigned char green, unsigned char blue) override;
    void clearLCDscreen(void) override;

    indicator_led indicatorforInput(input_hex hex) override;
    indicator_led indicatorForOutput(output_hex hex) override;
    circuit_end leftCircuitForInput(input_hex hex) override;
    circuit_end rightCircuitForInput(input_hex hex) override;
    circuit_end leftCircuitForOutput(output_hex hex) override;
    circuit_end rightCircuitForOutput(output_hex hex) override;

protected:
    static const int ioDeviceBus[NUMBER_OF_SX1509_DEVICES];
    static const int ioDeviceAddress[NUMBER_OF_SX1509_DEVICES];
    static const int analogDeviceAddress[NUMBER_OF_ADS1015_DEVICES];
    static const int analogDeviceBus[NUMBER_OF_ADS1015_DEVICES];

    static unsigned int normalizedKnobValue(unsigned int sliderValue);
    unsigned int normalizedSliderValue(unsigned int sliderValue);

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

    unsigned int lastSliderPosition;
    
    int writeMultiplexerForBus(int bus);
};

#endif // _WALL_H_
