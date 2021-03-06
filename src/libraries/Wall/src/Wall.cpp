// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include <rgb_lcd.h>
#include <Arduino.h>
#include <Wire.h>
#include "Wall.h"

const int Wall::ioDeviceBus[NUMBER_OF_SX1509_DEVICES] = {
    SPARKFUN_SX1509_FIRST_I2C_BUS,
    SPARKFUN_SX1509_SECOND_I2C_BUS,
    SPARKFUN_SX1509_THIRD_I2C_BUS,
    SPARKFUN_SX1509_FOURTH_I2C_BUS
};
const int Wall::ioDeviceAddress[NUMBER_OF_SX1509_DEVICES] = {
    SPARKFUN_SX1509_FIRST_I2C_ADDRESS,
    SPARKFUN_SX1509_SECOND_I2C_ADDRESS,
    SPARKFUN_SX1509_THIRD_I2C_ADDRESS,
    SPARKFUN_SX1509_FOURTH_I2C_ADDRESS
};
const int Wall::analogDeviceAddress[NUMBER_OF_ADS1015_DEVICES] = {
    ADAFRUIT_ANALOG_FIRST_I2C_ADDRESS,
    ADAFRUIT_ANALOG_SECOND_I2C_ADDRESS
};
const int Wall::analogDeviceBus[NUMBER_OF_ADS1015_DEVICES] = {
    ADAFRUIT_ANALOG_FIRST_I2C_BUS,
    ADAFRUIT_ANALOG_SECOND_I2C_BUS
};


Wall* Wall::usingFactory(FactoryInterface *io) {
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        this->io_expander[device] = io->createSX1509Instance();
    for (int device = 0; device < NUMBER_OF_ADS1015_DEVICES; device++)
        this->analog_expander[device] = io->createADS1015Instance(analogDeviceAddress[device]);
    this->pwm = io->createPWMinstance(ADAFRUIT_PWM_I2C_ADDRESS);
    this->lcd = io->createLCDInstance();
    return this;
}

void Wall::setMultiplexerForIOexpander(int device) {
    setMultiplexerI2CBus(ioDeviceBus[device]);
}

void Wall::setMultiplexerForAnalog(int device) {
    setMultiplexerI2CBus(analogDeviceBus[device]);
}

// Due to a bug in the Intel101 I2C protocol handling,it is possible that the
// 101 will not complete the transaction and the Adafruit multiplexer will not
// select the correct bus.  Luckily this is reported by the Wire protocol, and
// a second attempt can be made.  Back-to-back failures have not been observed,
// but must be assumed to be possible.  Loop until successful.
void Wall::setMultiplexerI2CBus(int bus) {
    while (writeMultiplexerForBus(bus));
}

int Wall::writeMultiplexerForBus(int bus)
{
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << bus);
    return Wire.endTransmission();
}

bool Wall::resetDigitalIO(int device)
{
    setMultiplexerForIOexpander(device);
    return (io_expander[device]->begin(ioDeviceAddress[device]) != 0);
}
void Wall::resetAnalogIO(int device)
{
    setMultiplexerForAnalog(device);
    analog_expander[device]->begin();
}

bool Wall::initialize(void)
{
    Wire.begin();
    initializeLCD(); // also calls Wire.begin();
    if(!initializeIOexpanders())
        return false;
    initializeAnalogExpanders();
    initializeLEDarrayOutputs();
    initializeMotorOutputs();
    initializePWMOutputs();
    initializeToggleInputs();
    initializeJoystickInputs();
    initializeButtonInOuts();
    initalizeELwireOutputs();
    initializePWMOutputs();
    resetCircuitInputs();
    return true;
}

bool Wall::initializeIOexpanders(void)
{
    bool result = true;
    for(int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        result = result && resetDigitalIO(device);
    return result;
}

void Wall::initializeAnalogExpanders(void)
{
    for (int device = 0; device < NUMBER_OF_ADS1015_DEVICES; device++)
        resetAnalogIO(device);
}

void Wall::initializeLEDarrayOutputs(void)
{
    setMultiplexerForIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_GREEN_LEFT, ANALOG_OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_GREEN_RIGHT, ANALOG_OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_WHITE_LEFT, ANALOG_OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_WHITE_RIGHT, ANALOG_OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_1, ANALOG_OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_2, ANALOG_OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_3, ANALOG_OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_4, ANALOG_OUTPUT);

    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_GREEN_LEFT, ledArrayNormalizedValue(GREEN_LED, 0));
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_GREEN_RIGHT, ledArrayNormalizedValue(GREEN_LED, 0));
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_WHITE_LEFT, ledArrayNormalizedValue(WHITE_LED, 0));
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_WHITE_RIGHT, ledArrayNormalizedValue(WHITE_LED, 0));
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_RED_QUAD_1, ledArrayNormalizedValue(RED_LED, 0));
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_RED_QUAD_2, ledArrayNormalizedValue(RED_LED, 0));
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_RED_QUAD_3, ledArrayNormalizedValue(RED_LED, 0));
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(OUTPUT_LED_ARRAY_RED_QUAD_4, ledArrayNormalizedValue(RED_LED, 0));
}

void Wall::initializeMotorOutputs(void)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR1_PWM, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR1_IN1, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR1_IN2, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR2_PWM, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR2_IN1, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR2_IN2, OUTPUT);
}

void Wall::initializePWMOutputs(void)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->begin();
    pwm->setPWMFreq(WALL_PWM_FREQUENCY);

    pwm->setPin(INDICATOR_LED_ARRAY_WHITE, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_LED_ARRAY_RED, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_LED_ARRAY_GREEN, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_MOTOR_2, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_MOTOR_1, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_TRANSDUCER, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_TOGGLE_SWITCH, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_JOYSTICK, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_ROTARY_POT, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_LINEAR_POT, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_PHOTO_SENSOR, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_FORCE_SENSOR, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_BATTERY_POSITIVE, PWM_INDICATOR_OFF_VALUE, FALSE);
    pwm->setPin(INDICATOR_BATTERY_NEGATIVE, PWM_INDICATOR_OFF_VALUE, FALSE);

    pwm->setPWM(OUTPUT_TRANSDUCER, PWM_START_OF_DUTY_CYCLE, PWM_FULL_DUTY_CYCLE);
}

void Wall::initializeToggleInputs(void)
{
    setMultiplexerForIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    io_expander[INPUT_TOGGLE_I2C_DEVICE]->pinMode(LEFT_TOGGLE, INPUT_PULLUP);
    io_expander[INPUT_TOGGLE_I2C_DEVICE]->pinMode(CENTER_TOGGLE, INPUT_PULLUP);
    io_expander[INPUT_TOGGLE_I2C_DEVICE]->pinMode(RIGHT_TOGGLE, INPUT_PULLUP);
}

void Wall::initializeJoystickInputs(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_DOWN, INPUT_PULLUP);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_UP, INPUT_PULLUP);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_LEFT, INPUT_PULLUP);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_RIGHT, INPUT_PULLUP);
}

void Wall::initializeButtonInOuts(void)
{
    for(int b = BLUE_BUTTON; b <= WHITE_BUTTON; b++)
    {
        large_button button = static_cast<large_button>(b);
        int device = buttonDevice(button);
        setMultiplexerForIOexpander(device);
        io_expander[device]->pinMode(buttonPin(button), INPUT_PULLUP);
        io_expander[device]->pinMode(buttonLEDpin(button), OUTPUT);
        io_expander[device]->digitalWrite(buttonLEDpin(button), LOW);
    }
}

void Wall::initalizeELwireOutputs(void)
{
    for (int el = RED_WIRE_ONE; el <= BLUE_WIRE_TWO; el++)
    {
        int wire = elWirePin(static_cast<EL_wire>(el));
        pinMode(wire, OUTPUT);
        digitalWrite(wire, LOW);
    }
}

int Wall::greenLEDarrayPin(led_section section)
{
    switch (section)
    {
        case LEFT_SIDE: return OUTPUT_LED_ARRAY_GREEN_LEFT;
        case RIGHT_SIDE: return OUTPUT_LED_ARRAY_GREEN_RIGHT;
        default: return 0;
    }
}
int Wall::whiteLEDarrayPin(led_section section)
{
    switch (section)
    {
        case LEFT_SIDE: return OUTPUT_LED_ARRAY_WHITE_LEFT;
        case RIGHT_SIDE: return OUTPUT_LED_ARRAY_WHITE_RIGHT;
        default: return 0;
    }
}
int Wall::redLEDarrayPin(led_section section)
{
    switch (section)
    {
        case LEFT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_1;
        case RIGHT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_2;
        case LOWER_RIGHT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_3;
        case LOWER_LEFT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_4;
        default: return 0;
    }
}
int Wall::ledArrayPin(led_array array, led_section section)
{
    if (array == GREEN_LED)
        return greenLEDarrayPin(section);
    if (array == WHITE_LED)
        return whiteLEDarrayPin(section);
    if (array == RED_LED)
        return redLEDarrayPin(section);
    return 0;
}

// The SX1509 "analog" PWM pulse assumes that a LED is attached from Vcc to the output, such
// that a low output turns on the LED (active low).  The green and white arrays are in fact
// wired active high, so they must be adjusted to the opposite value
//
unsigned char Wall::ledArrayNormalizedValue(led_array array, unsigned char value)
{
    return (array == RED_LED) ? value : MAXIMUM_ANALOG_OUTPUT_VALUE - value;
}


void Wall::setLEDarrayBrightness(led_array array, led_section section, unsigned char brightness)
{
    setMultiplexerForIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->analogWrite(
        Wall::ledArrayPin(array, section), ledArrayNormalizedValue(array, brightness));
}

void Wall::turnOnLEDarray(led_array array, led_section section)
{
    setLEDarrayBrightness(array, section, MAXIMUM_ANALOG_OUTPUT_VALUE);
}
void Wall::turnOffLEDarray(led_array array, led_section section)
{
    setLEDarrayBrightness(array, section, MINIMUM_ANALOG_OUTPUT_VALUE);
}

int Wall::motorControlPin1(wall_motor motor)
{
    return (motor == BLUE_MOTOR) ? OUTPUT_MOTOR2_IN1 : OUTPUT_MOTOR1_IN1;
}
int Wall::motorControlPin2(wall_motor motor)
{
    return (motor == BLUE_MOTOR) ? OUTPUT_MOTOR2_IN2 : OUTPUT_MOTOR1_IN2;
}
int Wall::motorPWMpin(wall_motor motor)
{
    return (motor == BLUE_MOTOR) ? OUTPUT_MOTOR2_PWM : OUTPUT_MOTOR1_PWM;
}

// Motor Truth Table (from TB6612 documentation):
//s
//   PIN1  PIN2    Motor behavior   
//  ================================
//   LOW   LOW     Stop  
//   LOW   HIGH    Counter-clockwise
//   HIGH  LOW     Clockwise
//   HIGH  HIGH    Brake
//
void Wall::setMotorDirectionClockwise(wall_motor motor)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin1(motor), HIGH);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin2(motor), LOW);
}
void Wall::setMotorDirectionCounterClockwise(wall_motor motor)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin1(motor), LOW);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin2(motor), HIGH);
}
void Wall::stopMotor(wall_motor motor)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin1(motor), LOW);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin2(motor), LOW);
}
void Wall::setMotorSpeed(wall_motor motor, unsigned char speed)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->analogWrite(motorPWMpin(motor), speed);
}

void Wall::turnTransducerOn(void)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPWM(OUTPUT_TRANSDUCER, PWM_START_OF_DUTY_CYCLE, PWM_HALF_DUTY_CYCLE - 1);
}
void Wall::turnTransducerOff(void)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPWM(OUTPUT_TRANSDUCER, PWM_START_OF_DUTY_CYCLE, PWM_FULL_DUTY_CYCLE);
}

int Wall::indicatorPin(indicator_led lamp)
{
    switch(lamp)
    {
        case INDICATE_WHITE_LED: return INDICATOR_LED_ARRAY_WHITE;
        case INDICATE_BLUE_MOTOR: return INDICATOR_MOTOR_2;
        case INDICATE_RED_LED: return INDICATOR_LED_ARRAY_RED;
        case INDICATE_ORANGE_MOTOR: return INDICATOR_MOTOR_1;
        case INDICATE_TRANSDUCER: return INDICATOR_TRANSDUCER;
        case INDICATE_GREEN_LED: return INDICATOR_LED_ARRAY_GREEN;
        case INDICATE_NEGATIVE_POLE: return INDICATOR_BATTERY_NEGATIVE;
        case INDICATE_KNOB: return INDICATOR_ROTARY_POT;
        case INDICATE_SLIDER: return INDICATOR_LINEAR_POT;
        case INDICATE_PHOTO_SENSOR: return INDICATOR_PHOTO_SENSOR;
        case INDICATE_JOYSTICK: return INDICATOR_JOYSTICK;
        case INDICATE_TOGGLES: return INDICATOR_TOGGLE_SWITCH;
        case INDICATE_TOUCH_SENSOR: return INDICATOR_FORCE_SENSOR;
        case INDICATE_POSITIVE_POLE: return INDICATOR_BATTERY_POSITIVE;
        default: return -1;
    }
}
void Wall::turnIndicatorOn(indicator_led lamp)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(indicatorPin(lamp), PWM_INDICATOR_ON_VALUE, FALSE);
}
void Wall::turnIndicatorOff(indicator_led lamp)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(indicatorPin(lamp), PWM_INDICATOR_OFF_VALUE, FALSE);
}
void Wall::setIndicatorBrightness(indicator_led lamp, unsigned int value)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(indicatorPin(lamp), value % PWM_FULL_DUTY_CYCLE, FALSE);
}

int Wall::toggleSwitchPin(toggle_switch toggle)
{
    switch (toggle)
    {
        case LEFT_TOGGLE:   return INPUT_TOGGLE_1;
        case CENTER_TOGGLE: return INPUT_TOGGLE_2;
        case RIGHT_TOGGLE:  return INPUT_TOGGLE_3;
        default: return 0;
    }
}

// Toggle switches are active low: 
//    Input pins have internal pullup, and toggle connect them to ground
bool Wall::isToggleOn(toggle_switch toggle)
{
    setMultiplexerForIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    return (io_expander[INPUT_TOGGLE_I2C_DEVICE]->digitalRead(toggleSwitchPin(toggle)) == LOW);
}

// Joystick directions are active low: 
//    Input pins have internal pullup, and switches connect them to ground
bool Wall::isJoystickUp(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_UP) == LOW);
}
bool Wall::isJoystickDown(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_DOWN) == LOW);
}
bool Wall::isJoystickLeft(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_LEFT) == LOW);
}
bool Wall::isJoystickRight(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_RIGHT) == LOW);
}

// When knob is moved all the way left or right, the contact becomes
// disengaged from the track, resulting in a maximum value that is not
// acheivable, even when far right.  If this value is detected, the result
// must a be safe value that is the extreme value for the left side.
unsigned int Wall::normalizedKnobValue(unsigned int rawKnobValue)
{
    return (rawKnobValue > INPUT_ROTARY_POT_RIGHT_LIMIT) ? INPUT_ROTARY_POT_LEFT_LIMIT : rawKnobValue;
}

unsigned int Wall::getKnobPosition(void)
{
    setMultiplexerForAnalog(INPUT_ROTARY_POT_I2C_DEVICE);
    return normalizedKnobValue(analog_expander[INPUT_ROTARY_POT_I2C_DEVICE]->
        readADC_SingleEnded(INPUT_ROTARY_POT));
}

// When slider is moved all the way left or right, the contact becomes
// disengaged from the track, resulting in a maximum value that is not
// acheivable, even when far right.  If this value is detected, the result
// must a be safe value that is the extreme value for the side last observed.
unsigned int Wall::normalizedSliderValue(unsigned int rawSliderValue)
{
    const unsigned int extremePosition = (this->lastSliderPosition < INPUT_LINEAR_POT_MIDDLE_POSITION) ?
        INPUT_LINEAR_POT_LEFT_LIMIT :
        INPUT_LINEAR_POT_RIGHT_LIMIT;
    return (rawSliderValue > INPUT_LINEAR_POT_RIGHT_LIMIT) ? 
        extremePosition : rawSliderValue;
}

unsigned int Wall::getSliderPosition(void)
{
    setMultiplexerForAnalog(INPUT_LINEAR_POT_I2C_DEVICE);
    this->lastSliderPosition = normalizedSliderValue(analog_expander[INPUT_LINEAR_POT_I2C_DEVICE]->
        readADC_SingleEnded(INPUT_LINEAR_POT));
    return this->lastSliderPosition;
}

int Wall::photoSensorPin(photo_sensor sensor)
{
    switch (sensor)
    {
        case LEFT_PHOTO:   return INPUT_PHOTO_SENSOR_1;
        case CENTER_PHOTO: return INPUT_PHOTO_SENSOR_2;
        case RIGHT_PHOTO:  return INPUT_PHOTO_SENSOR_3;
        default: return 0;
    }
}

unsigned int Wall::getPhotoSensorValue(photo_sensor sensor)
{
    setMultiplexerForAnalog(INPUT_PHOTO_SENSOR_I2C_DEVICE);
    return analog_expander[INPUT_PHOTO_SENSOR_I2C_DEVICE]->readADC_SingleEnded(photoSensorPin(sensor));
}

int Wall::forceSensorPin(force_sensor sensor)
{
    switch (sensor)
    {
        case LEFT_TOUCH:   return INPUT_FORCE_SENSOR_2;
        case BOTTOM_TOUCH: return INPUT_FORCE_SENSOR_3;
        case RIGHT_TOUCH:  return INPUT_FORCE_SENSOR_1;
        default: return 0;
    }
}

unsigned int Wall::getTouchSensorValue(force_sensor sensor)
{
    setMultiplexerForAnalog(INPUT_FORCE_SENSOR_I2C_DEVICE);
    return analog_expander[INPUT_FORCE_SENSOR_I2C_DEVICE]->readADC_SingleEnded(forceSensorPin(sensor));
}

int Wall::circuitDevice(circuit_end end)
{
    switch(end)
    {
        case CIRCUIT_KNOB_LEFT: return GRID_ROTARY_POT_I2C_DEVICE;
        case CIRCUIT_KNOB_RIGHT: return GRID_ROTARY_POT_I2C_DEVICE;
        case CIRCUIT_SLIDER_LEFT: return GRID_LINEAR_POT_I2C_DEVICE;
        case CIRCUIT_SLIDER_RIGHT: return GRID_LINEAR_POT_I2C_DEVICE;
        case CIRCUIT_PHOTO_LEFT: return GRID_PHOTO_SENSOR_I2C_DEVICE;
        case CIRCUIT_PHOTO_RIGHT: return GRID_PHOTO_SENSOR_I2C_DEVICE;
        case CIRCUIT_JOYSTICK_LEFT: return GRID_JOYSTICK_I2C_DEVICE;
        case CIRCUIT_JOYSTICK_RIGHT: return GRID_JOYSTICK_I2C_DEVICE;
        case CIRCUIT_TOGGLE_LEFT: return GRID_TOGGLE_SWITCH_I2C_DEVICE;
        case CIRCUIT_TOGGLE_RIGHT: return GRID_TOGGLE_SWITCH_I2C_DEVICE;
        case CIRCUIT_TOUCH_LEFT: return GRID_FORCE_SENSOR_I2C_DEVICE;
        case CIRCUIT_TOUCH_RIGHT: return GRID_FORCE_SENSOR_I2C_DEVICE;
        case CIRCUIT_BLUE_MOTOR_LEFT: return GRID_MOTOR_2_I2C_DEVICE;
        case CIRCUIT_BLUE_MOTOR_RIGHT: return GRID_MOTOR_2_I2C_DEVICE;
        case CIRCUIT_ORANGE_MOTOR_LEFT: return GRID_MOTOR_1_I2C_DEVICE;
        case CIRCUIT_ORANGE_MOTOR_RIGHT: return GRID_MOTOR_1_I2C_DEVICE;
        case CIRCUIT_TRANSDUCER_LEFT: return GRID_TRANSDUCER_I2C_DEVICE;
        case CIRCUIT_TRANSDUCER_RIGHT: return GRID_TRANSDUCER_I2C_DEVICE;
        case CIRCUIT_WHITE_LED_LEFT: return GRID_LED_ARRAY_WHITE_I2C_DEVICE;
        case CIRCUIT_WHITE_LED_RIGHT: return GRID_LED_ARRAY_WHITE_I2C_DEVICE;
        case CIRCUIT_GREEN_LED_LEFT: return GRID_LED_ARRAY_GREEN_I2C_DEVICE;
        case CIRCUIT_GREEN_LED_RIGHT: return GRID_LED_ARRAY_GREEN_I2C_DEVICE;
        case CIRCUIT_RED_LED_LEFT: return GRID_LED_ARRAY_RED_I2C_DEVICE;
        case CIRCUIT_RED_LED_RIGHT: return GRID_LED_ARRAY_RED_I2C_DEVICE;
        case CIRCUIT_POSITIVE_POLE: return GRID_BATTERY_I2C_DEVICE;
        case CIRCUIT_NEGATIVE_POLE: return GRID_BATTERY_I2C_DEVICE;
        default: return 0;
    };
}
int Wall::circuitPin(circuit_end end)
{
    switch (end)
    {
        case CIRCUIT_KNOB_LEFT: return GRID_ROTARY_POT_LEFT;
        case CIRCUIT_KNOB_RIGHT: return GRID_ROTARY_POT_RIGHT;
        case CIRCUIT_SLIDER_LEFT: return GRID_LINEAR_POT_LEFT;
        case CIRCUIT_SLIDER_RIGHT: return GRID_LINEAR_POT_RIGHT;
        case CIRCUIT_PHOTO_LEFT: return GRID_PHOTO_SENSOR_LEFT;
        case CIRCUIT_PHOTO_RIGHT: return GRID_PHOTO_SENSOR_RIGHT;
        case CIRCUIT_JOYSTICK_LEFT: return GRID_JOYSTICK_LEFT;
        case CIRCUIT_JOYSTICK_RIGHT: return GRID_JOYSTICK_RIGHT;
        case CIRCUIT_TOGGLE_LEFT: return GRID_TOGGLE_SWITCH_LEFT;
        case CIRCUIT_TOGGLE_RIGHT: return GRID_TOGGLE_SWITCH_RIGHT;
        case CIRCUIT_TOUCH_LEFT: return GRID_FORCE_SENSOR_LEFT;
        case CIRCUIT_TOUCH_RIGHT: return GRID_FORCE_SENSOR_RIGHT;
        case CIRCUIT_BLUE_MOTOR_LEFT: return GRID_MOTOR_2_LEFT;
        case CIRCUIT_BLUE_MOTOR_RIGHT: return GRID_MOTOR_2_RIGHT;
        case CIRCUIT_ORANGE_MOTOR_LEFT: return GRID_MOTOR_1_LEFT;
        case CIRCUIT_ORANGE_MOTOR_RIGHT: return GRID_MOTOR_1_RIGHT;
        case CIRCUIT_TRANSDUCER_LEFT: return GRID_TRANSDUCER_LEFT;
        case CIRCUIT_TRANSDUCER_RIGHT: return GRID_TRANSDUCER_RIGHT;
        case CIRCUIT_WHITE_LED_LEFT: return GRID_LED_ARRAY_WHITE_LEFT;
        case CIRCUIT_WHITE_LED_RIGHT: return GRID_LED_ARRAY_WHITE_RIGHT;
        case CIRCUIT_GREEN_LED_LEFT: return GRID_LED_ARRAY_GREEN_LEFT;
        case CIRCUIT_GREEN_LED_RIGHT: return GRID_LED_ARRAY_GREEN_RIGHT;
        case CIRCUIT_RED_LED_LEFT: return GRID_LED_ARRAY_RED_LEFT;
        case CIRCUIT_RED_LED_RIGHT: return GRID_LED_ARRAY_RED_RIGHT;
        case CIRCUIT_POSITIVE_POLE: return GRID_BATTERY_POSTIVE;
        case CIRCUIT_NEGATIVE_POLE: return GRID_BATTERY_NEGATIVE;
        default: return 0;
    };
}

int Wall::readCircuitState(circuit_end end)
{
    const int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    return io_expander[device]->digitalRead(circuitPin(end));
}

void Wall::setCircuitAsInput(circuit_end end)
{
    const int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    io_expander[device]->pinMode(circuitPin(end), INPUT_PULLUP);
}
void Wall::setCircuitAsOutput(circuit_end end)
{
    const int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    io_expander[device]->pinMode(circuitPin(end), OUTPUT);
    io_expander[device]->digitalWrite(circuitPin(end), LOW);
}
void Wall::resetCircuitInputs(void)
{
    for (int circuit = CIRCUIT_KNOB_LEFT; circuit <= CIRCUIT_NEGATIVE_POLE; circuit++)
        setCircuitAsInput(static_cast<circuit_end>(circuit));
}

bool Wall::isCircuitConnected(circuit_end A, circuit_end B)
{
    setCircuitAsOutput(A);
    bool areConnected = (readCircuitState(B) == LOW);
    setCircuitAsInput(A);
    return areConnected;
}

circuit_end Wall::leftCircuitForInput(input_hex hex)
{
    switch (hex)
    {
        case KNOB_HEX: return CIRCUIT_KNOB_LEFT;
        case SLIDER_HEX: return CIRCUIT_SLIDER_LEFT;
        case PHOTO_SENSOR_HEX: return CIRCUIT_PHOTO_LEFT;
        case JOYSTICK_HEX: return CIRCUIT_JOYSTICK_LEFT;
        case TOGGLE_SWITCH_HEX: return CIRCUIT_TOGGLE_LEFT;
        case TOUCH_SENSOR_HEX: return CIRCUIT_TOUCH_LEFT;
        default: return NO_CIRCUIT;
    };
}
circuit_end Wall::rightCircuitForInput(input_hex hex)
{
    switch (hex)
    {
        case KNOB_HEX: return CIRCUIT_KNOB_RIGHT;
        case SLIDER_HEX: return CIRCUIT_SLIDER_RIGHT;
        case PHOTO_SENSOR_HEX: return CIRCUIT_PHOTO_RIGHT;
        case JOYSTICK_HEX: return CIRCUIT_JOYSTICK_RIGHT;
        case TOGGLE_SWITCH_HEX: return CIRCUIT_TOGGLE_RIGHT;
        case TOUCH_SENSOR_HEX: return CIRCUIT_TOUCH_RIGHT;
        default: return NO_CIRCUIT;
    };
}
circuit_end Wall::leftCircuitForOutput(output_hex hex)
{
    switch (hex)
    {
        case BLUE_MOTOR_HEX: return CIRCUIT_BLUE_MOTOR_LEFT;
        case ORANGE_MOTOR_HEX: return CIRCUIT_ORANGE_MOTOR_LEFT;
        case TRANSDUCER_HEX: return CIRCUIT_TRANSDUCER_LEFT;
        case WHITE_LED_HEX: return CIRCUIT_WHITE_LED_LEFT;
        case GREEN_LED_HEX: return CIRCUIT_GREEN_LED_LEFT;
        case RED_LED_HEX: return CIRCUIT_RED_LED_LEFT;
        default: return NO_CIRCUIT;
    };
}
circuit_end Wall::rightCircuitForOutput(output_hex hex)
{
    switch (hex)
    {
        case BLUE_MOTOR_HEX: return CIRCUIT_BLUE_MOTOR_RIGHT;
        case ORANGE_MOTOR_HEX: return CIRCUIT_ORANGE_MOTOR_RIGHT;
        case TRANSDUCER_HEX: return CIRCUIT_TRANSDUCER_RIGHT;
        case WHITE_LED_HEX: return CIRCUIT_WHITE_LED_RIGHT;
        case GREEN_LED_HEX: return CIRCUIT_GREEN_LED_RIGHT;
        case RED_LED_HEX: return CIRCUIT_RED_LED_RIGHT;
        default: return NO_CIRCUIT;
    };
}

indicator_led Wall::indicatorforInput(input_hex hex)
{
    switch (hex)
    {
        case KNOB_HEX: return INDICATE_KNOB;
        case SLIDER_HEX: return INDICATE_SLIDER;
        case PHOTO_SENSOR_HEX: return INDICATE_PHOTO_SENSOR;
        case JOYSTICK_HEX: return INDICATE_JOYSTICK;
        case TOGGLE_SWITCH_HEX: return INDICATE_TOGGLES;
        case TOUCH_SENSOR_HEX: return INDICATE_TOUCH_SENSOR;
        default: return NO_INDICATION;
    };
}

indicator_led Wall::indicatorForOutput(output_hex hex)
{
    switch (hex)
    {
    case BLUE_MOTOR_HEX: return INDICATE_BLUE_MOTOR;
    case ORANGE_MOTOR_HEX: return INDICATE_ORANGE_MOTOR;
    case TRANSDUCER_HEX: return INDICATE_TRANSDUCER;
    case WHITE_LED_HEX: return INDICATE_WHITE_LED;
    case GREEN_LED_HEX: return INDICATE_GREEN_LED;
    case RED_LED_HEX: return INDICATE_RED_LED;
    default: return NO_INDICATION;
    }
}


int Wall::buttonDevice(large_button button)
{
    switch(button)
    {
        case BLUE_BUTTON:   return BLUE_BUTTON_I2C_DEVICE;
        case YELLOW_BUTTON: return YELLOW_BUTTON_I2C_DEVICE;
        case GREEN_BUTTON:  return GREEN_BUTTON_I2C_DEVICE;
        case RED_BUTTON:    return RED_BUTTON_I2C_DEVICE;
        case WHITE_BUTTON:  return WHITE_BUTTON_I2C_DEVICE;
        default: return 0;
    }
}
int Wall::buttonPin(large_button button)
{
    switch (button)
    {
        case BLUE_BUTTON:   return BLUE_BUTTON_PIN;
        case YELLOW_BUTTON: return YELLOW_BUTTON_PIN;
        case GREEN_BUTTON:  return GREEN_BUTTON_PIN;
        case RED_BUTTON:    return RED_BUTTON_PIN;
        case WHITE_BUTTON:  return WHITE_BUTTON_PIN;
        default: return 0;
    }
}
int Wall::buttonLEDpin(large_button button)
{
    switch (button)
    {
        case BLUE_BUTTON:   return BLUE_LED_PIN;
        case YELLOW_BUTTON: return YELLOW_LED_PIN;
        case GREEN_BUTTON:  return GREEN_LED_PIN;
        case RED_BUTTON:    return RED_LED_PIN;
        case WHITE_BUTTON:  return WHITE_LED_PIN;
        default: return 0;
    }
}
bool Wall::isButtonDepressed(large_button button)
{
    const int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    return (io_expander[device]->digitalRead(buttonPin(button)) == LOW);
}
void Wall::illuminateButton(large_button button)
{
    const int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    io_expander[device]->digitalWrite(buttonLEDpin(button), HIGH);
}
void Wall::extinguishButton(large_button button)
{
    const int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    io_expander[device]->digitalWrite(buttonLEDpin(button), LOW);
}

int Wall::elWirePin(EL_wire line)
{
    switch (line)
    {
        case RED_WIRE_ONE: return RED_EL_1;
        case RED_WIRE_TWO: return RED_EL_2;
        case GREEN_WIRE_ONE: return GREEN_EL_1;
        case GREEN_WIRE_TWO: return GREEN_EL_2;
        case YELLOW_WIRE: return YELLOW_EL;
        case WHITE_WIRE: return WHITE_EL;
        case BLUE_WIRE_ONE: return BLUE_EL_1;
        case BLUE_WIRE_TWO: return BLUE_EL_2;
        default: return 0;
    }
}
void Wall::illuminateELWire(EL_wire line)
{
    digitalWrite(elWirePin(line), HIGH);
}
void Wall::extinguishELWire(EL_wire line)
{
    digitalWrite(elWirePin(line), LOW);
}

void Wall::initializeLCD(void)
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->begin(16, 2);
    lcd->clear();
}

void Wall::lcdPrintAt(unsigned char column, unsigned char row, const char buf[])
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->setCursor(column, row);
    lcd->print(buf);
}

void Wall::lcdSetBacklightColor(unsigned char red, unsigned char green, unsigned char blue)
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->setRGB(red, green, blue);
}

void Wall::clearLCDscreen(void)
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->clear();
}
