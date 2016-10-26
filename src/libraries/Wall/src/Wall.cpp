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

const int WallImplementation::ioDeviceBus[NUMBER_OF_SX1509_DEVICES] = {
    SPARKFUN_SX1509_FIRST_I2C_BUS,
    SPARKFUN_SX1509_SECOND_I2C_BUS,
    SPARKFUN_SX1509_THIRD_I2C_BUS,
    SPARKFUN_SX1509_FOURTH_I2C_BUS
};
const int WallImplementation::ioDeviceAddress[NUMBER_OF_SX1509_DEVICES] = {
    SPARKFUN_SX1509_FIRST_I2C_ADDRESS,
    SPARKFUN_SX1509_SECOND_I2C_ADDRESS,
    SPARKFUN_SX1509_THIRD_I2C_ADDRESS,
    SPARKFUN_SX1509_FOURTH_I2C_ADDRESS
};
const int WallImplementation::analogDeviceAddress[NUMBER_OF_ADS1015_DEVICES] = {
    ADAFRUIT_ANALOG_FIRST_I2C_ADDRESS,
    ADAFRUIT_ANALOG_SECOND_I2C_ADDRESS
};
const int WallImplementation::analogDeviceBus[NUMBER_OF_ADS1015_DEVICES] = {
    ADAFRUIT_ANALOG_FIRST_I2C_BUS,
    ADAFRUIT_ANALOG_SECOND_I2C_BUS
};


WallImplementation::WallImplementation(FactoryInterface *io) {
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        this->io_expander[device] = io->createSX1509Instance();
    for (int device = 0; device < NUMBER_OF_ADS1015_DEVICES; device++)
        this->analog_expander[device] = io->createADS1015Instance(analogDeviceAddress[device]);
    this->pwm = io->createPWMinstance(ADAFRUIT_PWM_I2C_ADDRESS);
    this->lcd = io->createLCDInstance();
}

void WallImplementation::setMultiplexerForIOexpander(int device) {
    setMultiplexerI2CBus(ioDeviceBus[device]);
}

void WallImplementation::setMultiplexerForAnalog(int device) {
    setMultiplexerI2CBus(analogDeviceBus[device]);
}

// Due to a bug in the Intel101 I2C protocol handling,it is possible that the
// 101 will not complete the transaction and the Adafruit multiplexer will not
// select the correct bus.  Luckily this is reported by the Wire protocol, and
// a second attempt can be made.  Back-to-back failures have not been observed,
// but must be assumed to be possible.  Loop until successful.
void WallImplementation::setMultiplexerI2CBus(int bus) {
    while (writeMultiplexerForBus(bus));
}

int WallImplementation::writeMultiplexerForBus(int bus)
{
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << bus);
    return Wire.endTransmission();
}

bool WallImplementation::resetDigitalIO(int device)
{
    setMultiplexerForIOexpander(device);
    return (io_expander[device]->begin(ioDeviceAddress[device]) != 0);
}
void WallImplementation::resetAnalogIO(int device)
{
    setMultiplexerForAnalog(device);
    analog_expander[device]->begin();
}

bool WallImplementation::initialize(void)
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
    resetCircuitInputs();
    return true;
}

bool WallImplementation::initializeIOexpanders(void)
{
    bool result = true;
    for(int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        result = result && resetDigitalIO(device);
    return result;
}

void WallImplementation::initializeAnalogExpanders(void)
{
    for (int device = 0; device < NUMBER_OF_ADS1015_DEVICES; device++)
        resetAnalogIO(device);
}

void WallImplementation::initializeLEDarrayOutputs(void)
{
    setMultiplexerForIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_GREEN_LEFT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_GREEN_RIGHT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_WHITE_LEFT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_WHITE_RIGHT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_1, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_2, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_3, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_4, OUTPUT);
}

void WallImplementation::initializeMotorOutputs(void)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR1_PWM, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR1_IN1, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR1_IN2, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR2_PWM, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR2_IN1, OUTPUT);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->pinMode(OUTPUT_MOTOR2_IN2, OUTPUT);
}

void WallImplementation::initializePWMOutputs(void)
{
    pwm->setPWMFreq(WALL_PWM_FREQUENCY);
    turnIndicatorOff(INDICATE_WHITE_LED);
    turnIndicatorOff(INDICATE_RED_LED);
    turnIndicatorOff(INDICATE_GREEN_LED);
    turnIndicatorOff(INDICATE_BLUE_MOTOR);
    turnIndicatorOff(INDICATE_ORANGE_MOTOR);
    turnIndicatorOff(INDICATE_TRANSDUCER);
    turnIndicatorOff(INDICATE_TOGGLES);
    turnIndicatorOff(INDICATE_JOYSTICK);
    turnIndicatorOff(INDICATE_KNOB);
    turnIndicatorOff(INDICATE_SLIDER);
    turnIndicatorOff(INDICATE_PHOTO_SENSOR);
    turnIndicatorOff(INDICATE_TOUCH_SENSOR);
    turnIndicatorOff(INDICATE_POSITIVE_POLE);
    turnIndicatorOff(INDICATE_NEGATIVE_POLE);
    turnTransducerOff();
}

void WallImplementation::initializeToggleInputs(void)
{
    setMultiplexerForIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    io_expander[INPUT_TOGGLE_I2C_DEVICE]->pinMode(LEFT_TOGGLE, INPUT_PULLUP);
    io_expander[INPUT_TOGGLE_I2C_DEVICE]->pinMode(CENTER_TOGGLE, INPUT_PULLUP);
    io_expander[INPUT_TOGGLE_I2C_DEVICE]->pinMode(RIGHT_TOGGLE, INPUT_PULLUP);
}

void WallImplementation::initializeJoystickInputs(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_DOWN, INPUT_PULLUP);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_UP, INPUT_PULLUP);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_LEFT, INPUT_PULLUP);
    io_expander[INPUT_JOYSTICK_I2C_DEVICE]->pinMode(INPUT_JOYSTICK_RIGHT, INPUT_PULLUP);
}

void WallImplementation::initializeButtonInOuts(void)
{
    for(int b = BLUE_BUTTON; b <= WHITE_BUTTON; b++)
    {
        large_button button = static_cast<large_button>(b);
        int device = buttonDevice(button);
        setMultiplexerForIOexpander(device);
        io_expander[device]->pinMode(buttonPin(button), INPUT_PULLUP);
        io_expander[device]->pinMode(buttonLEDpin(button), OUTPUT);
    }
}

void WallImplementation::initalizeELwireOutputs(void)
{
    for (int el = RED_WIRE_ONE; el <= BLUE_WIRE_TWO; el++)
        pinMode(elWirePin(static_cast<EL_wire>(el)), OUTPUT);
}

bool WallImplementation::ledArrayIsActiveLow(led_array array)
{
    return (array == RED_LED);
}

int WallImplementation::greenLEDarrayPin(led_section section)
{
    switch (section)
    {
        case LEFT_SIDE: return OUTPUT_LED_ARRAY_GREEN_LEFT;
        case RIGHT_SIDE: return OUTPUT_LED_ARRAY_GREEN_RIGHT;
        default: return 0;
    }
}
int WallImplementation::whiteLEDarrayPin(led_section section)
{
    switch (section)
    {
        case LEFT_SIDE: return OUTPUT_LED_ARRAY_WHITE_LEFT;
        case RIGHT_SIDE: return OUTPUT_LED_ARRAY_WHITE_RIGHT;
        default: return 0;
    }
}
int WallImplementation::redLEDarrayPin(led_section section)
{
    switch (section)
    {
        case LEFT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_1;
        case RIGHT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_2;
        case LOWER_LEFT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_3;
        case LOWER_RIGHT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_4;
        default: return 0;
    }
}
int WallImplementation::ledArrayPin(led_array array, led_section section)
{
    if (array == GREEN_LED)
        return greenLEDarrayPin(section);
    if (array == WHITE_LED)
        return whiteLEDarrayPin(section);
    if (array == RED_LED)
        return redLEDarrayPin(section);
    return 0;
}

void WallImplementation::turnOnLEDarray(led_array array, led_section section)
{
    const int pinValue = ledArrayIsActiveLow(array) ? LOW : HIGH;
    setMultiplexerForIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->digitalWrite(
        WallImplementation::ledArrayPin(array, section), pinValue);
}
void WallImplementation::turnOffLEDarray(led_array array, led_section section)
{
    const int pinValue = ledArrayIsActiveLow(array) ? HIGH : LOW;
    setMultiplexerForIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->digitalWrite(
        WallImplementation::ledArrayPin(array, section), pinValue);
}

int WallImplementation::motorControlPin1(wall_motor motor)
{
    return (motor == BLUE_MOTOR) ? OUTPUT_MOTOR2_IN1 : OUTPUT_MOTOR1_IN1;
}
int WallImplementation::motorControlPin2(wall_motor motor)
{
    return (motor == BLUE_MOTOR) ? OUTPUT_MOTOR2_IN2 : OUTPUT_MOTOR1_IN2;
}
int WallImplementation::motorPWMpin(wall_motor motor)
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
void WallImplementation::setMotorDirectionClockwise(wall_motor motor)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin1(motor), HIGH);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin2(motor), LOW);
}
void WallImplementation::setMotorDirectionCounterClockwise(wall_motor motor)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin1(motor), LOW);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin2(motor), HIGH);
}
void WallImplementation::stopMotor(wall_motor motor)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin1(motor), LOW);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->digitalWrite(motorControlPin2(motor), LOW);
}
void WallImplementation::setMotorSpeed(wall_motor motor, uint8_t speed)
{
    setMultiplexerForIOexpander(OUTPUT_MOTOR_I2C_DEVICE);
    io_expander[OUTPUT_MOTOR_I2C_DEVICE]->analogWrite(motorPWMpin(motor), speed);
}

void WallImplementation::turnTransducerOn(void)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPWM(OUTPUT_TRANSDUCER, PWM_START_OF_DUTY_CYCLE, PWM_HALF_DUTY_CYCLE - 1);
}
void WallImplementation::turnTransducerOff(void)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPWM(OUTPUT_TRANSDUCER, PWM_START_OF_DUTY_CYCLE, PWM_FULL_DUTY_CYCLE);
}

void WallImplementation::turnIndicatorOn(indicator_led lamp)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(lamp, PWM_INDICATOR_ON_VALUE, FALSE);
}
void WallImplementation::turnIndicatorOff(indicator_led lamp)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(lamp, PWM_INDICATOR_OFF_VALUE, FALSE);
}
void WallImplementation::setIndicatorBrightness(indicator_led lamp, uint16_t value)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(lamp, value % PWM_FULL_DUTY_CYCLE, FALSE);
}

int WallImplementation::toggleSwitchPin(toggle_switch toggle)
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
bool WallImplementation::isToggleOn(toggle_switch toggle)
{
    setMultiplexerForIOexpander(INPUT_TOGGLE_I2C_DEVICE);
    return (io_expander[INPUT_TOGGLE_I2C_DEVICE]->digitalRead(toggleSwitchPin(toggle)) == LOW);
}

// Joystick directions are active low: 
//    Input pins have internal pullup, and switches connect them to ground
bool WallImplementation::isJoystickUp(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_UP) == LOW);
}
bool WallImplementation::isJoystickDown(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_DOWN) == LOW);
}
bool WallImplementation::isJoystickLeft(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_LEFT) == LOW);
}
bool WallImplementation::isJoystickRight(void)
{
    setMultiplexerForIOexpander(INPUT_JOYSTICK_I2C_DEVICE);
    return (io_expander[INPUT_JOYSTICK_I2C_DEVICE]->digitalRead(INPUT_JOYSTICK_RIGHT) == LOW);
}

// When knob is moved all the way left or right, the contact becomes
// disengaged from the track, resulting in a maximum value that is not
// acheivable, even when far right.  If this value is detected, the result
// must a be safe value that is the extreme value for the left side.
uint16_t WallImplementation::normalizedKnobValue(uint16_t rawKnobValue)
{
    return (rawKnobValue > INPUT_ROTARY_POT_RIGHT_LIMIT) ? INPUT_ROTARY_POT_LEFT_LIMIT : rawKnobValue;
}

uint16_t WallImplementation::getKnobPosition(void)
{
    setMultiplexerForAnalog(INPUT_ROTARY_POT_I2C_DEVICE);
    return normalizedKnobValue(analog_expander[INPUT_ROTARY_POT_I2C_DEVICE]->
        readADC_SingleEnded(INPUT_ROTARY_POT));
}

// When slider is moved all the way left or right, the contact becomes
// disengaged from the track, resulting in a maximum value that is not
// acheivable, even when far right.  If this value is detected, the result
// must a be safe value that is the extreme value for the side last observed.
uint16_t WallImplementation::normalizedSliderValue(uint16_t rawSliderValue)
{
    const uint16_t extremePosition = (this->lastSliderPosition < INPUT_LINEAR_POT_MIDDLE_POSITION) ?
        INPUT_LINEAR_POT_LEFT_LIMIT :
        INPUT_LINEAR_POT_RIGHT_LIMIT;
    return (rawSliderValue > INPUT_LINEAR_POT_RIGHT_LIMIT) ? 
        extremePosition : rawSliderValue;
}

uint16_t WallImplementation::getSliderPosition(void)
{
    setMultiplexerForAnalog(INPUT_LINEAR_POT_I2C_DEVICE);
    this->lastSliderPosition = normalizedSliderValue(analog_expander[INPUT_LINEAR_POT_I2C_DEVICE]->
        readADC_SingleEnded(INPUT_LINEAR_POT));
    return this->lastSliderPosition;
}

int WallImplementation::photoSensorPin(photo_sensor sensor)
{
    switch (sensor)
    {
        case LEFT_PHOTO:   return INPUT_PHOTO_SENSOR_1;
        case CENTER_PHOTO: return INPUT_PHOTO_SENSOR_2;
        case RIGHT_PHOTO:  return INPUT_PHOTO_SENSOR_3;
        default: return 0;
    }
}

uint16_t WallImplementation::getPhotoSensorValue(photo_sensor sensor)
{
    setMultiplexerForAnalog(INPUT_PHOTO_SENSOR_I2C_DEVICE);
    return analog_expander[INPUT_PHOTO_SENSOR_I2C_DEVICE]->readADC_SingleEnded(photoSensorPin(sensor));
}

int WallImplementation::forceSensorPin(force_sensor sensor)
{
    switch (sensor)
    {
        case LEFT_TOUCH:   return INPUT_FORCE_SENSOR_2;
        case BOTTOM_TOUCH: return INPUT_FORCE_SENSOR_3;
        case RIGHT_TOUCH:  return INPUT_FORCE_SENSOR_1;
        default: return 0;
    }
}

uint16_t WallImplementation::getTouchSensorValue(force_sensor sensor)
{
    setMultiplexerForAnalog(INPUT_FORCE_SENSOR_I2C_DEVICE);
    return analog_expander[INPUT_FORCE_SENSOR_I2C_DEVICE]->readADC_SingleEnded(forceSensorPin(sensor));
}

int WallImplementation::circuitDevice(circuit_end end)
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
int WallImplementation::circuitPin(circuit_end end)
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

int WallImplementation::readCircuitState(circuit_end end)
{
    const int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    return io_expander[device]->digitalRead(circuitPin(end));
}

void WallImplementation::setCircuitAsInput(circuit_end end)
{
    const int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    io_expander[device]->pinMode(circuitPin(end), INPUT_PULLUP);
}
void WallImplementation::setCircuitAsOutput(circuit_end end)
{
    const int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    io_expander[device]->pinMode(circuitPin(end), OUTPUT);
    io_expander[device]->digitalWrite(circuitPin(end), LOW);
}
void WallImplementation::resetCircuitInputs(void)
{
    for (int circuit = CIRCUIT_KNOB_LEFT; circuit <= CIRCUIT_NEGATIVE_POLE; circuit++)
        setCircuitAsInput(static_cast<circuit_end>(circuit));
}

bool WallImplementation::isCircuitConnected(circuit_end A, circuit_end B)
{
    setCircuitAsOutput(A);
    bool areConnected = (readCircuitState(B) == LOW);
    setCircuitAsInput(A);
    return areConnected;
}

indicator_led WallImplementation::indicatorForCircuit(circuit_end point)
{
    switch(point)
    {
        case CIRCUIT_POSITIVE_POLE: return INDICATE_POSITIVE_POLE;
        case CIRCUIT_KNOB_LEFT:
        case CIRCUIT_KNOB_RIGHT: return INDICATE_KNOB;
        case CIRCUIT_SLIDER_LEFT:
        case CIRCUIT_SLIDER_RIGHT: return INDICATE_SLIDER;
        case CIRCUIT_PHOTO_LEFT:
        case CIRCUIT_PHOTO_RIGHT: return INDICATE_PHOTO_SENSOR;
        case CIRCUIT_JOYSTICK_LEFT:
        case CIRCUIT_JOYSTICK_RIGHT: return INDICATE_JOYSTICK;
        case CIRCUIT_TOGGLE_LEFT:
        case CIRCUIT_TOGGLE_RIGHT: return INDICATE_TOGGLES;
        case CIRCUIT_TOUCH_LEFT:
        case CIRCUIT_TOUCH_RIGHT: return INDICATE_TOUCH_SENSOR;
        case CIRCUIT_BLUE_MOTOR_LEFT:
        case CIRCUIT_BLUE_MOTOR_RIGHT: return INDICATE_BLUE_MOTOR;
        case CIRCUIT_ORANGE_MOTOR_LEFT: 
        case CIRCUIT_ORANGE_MOTOR_RIGHT: return INDICATE_ORANGE_MOTOR;
        case CIRCUIT_TRANSDUCER_LEFT:
        case CIRCUIT_TRANSDUCER_RIGHT: return INDICATE_TRANSDUCER;
        case CIRCUIT_WHITE_LED_LEFT:
        case CIRCUIT_WHITE_LED_RIGHT: return INDICATE_WHITE_LED;
        case CIRCUIT_GREEN_LED_LEFT:
        case CIRCUIT_GREEN_LED_RIGHT: return INDICATE_GREEN_LED;
        case CIRCUIT_RED_LED_LEFT:
        case CIRCUIT_RED_LED_RIGHT: return INDICATE_RED_LED;
        case CIRCUIT_NEGATIVE_POLE:
        default:  return INDICATE_NEGATIVE_POLE;
    }
}

int WallImplementation::buttonDevice(large_button button)
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
int WallImplementation::buttonPin(large_button button)
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
int WallImplementation::buttonLEDpin(large_button button)
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
bool WallImplementation::isButtonDepressed(large_button button)
{
    const int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    return (io_expander[device]->digitalRead(buttonPin(button)) == LOW);
}
void WallImplementation::illuminateButton(large_button button)
{
    const int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    io_expander[device]->digitalWrite(buttonLEDpin(button), HIGH);
}
void WallImplementation::extinguishButton(large_button button)
{
    const int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    io_expander[device]->digitalWrite(buttonLEDpin(button), LOW);
}

int WallImplementation::elWirePin(EL_wire line)
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
void WallImplementation::illuminateELWire(EL_wire line)
{
    digitalWrite(elWirePin(line), HIGH);
}
void WallImplementation::extinguishELWire(EL_wire line)
{
    digitalWrite(elWirePin(line), LOW);
}

void WallImplementation::initializeLCD(void)
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->begin(16, 2);
    lcd->clear();
}

void WallImplementation::lcdPrintAt(uint8_t column, uint8_t row, const char buf[])
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->setCursor(column, row);
    lcd->print(buf);
}

void WallImplementation::lcdSetBacklightColor(uint8_t red, uint8_t green, uint8_t blue)
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->setRGB(red, green, blue);
}

void WallImplementation::clearLCDscreen(void)
{
    setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd->clear();
}
