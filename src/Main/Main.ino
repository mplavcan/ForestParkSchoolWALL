// Main program for Forest Park School Wall
//
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include "DeviceFactory.h"
#include "Wall.h"
#include "Timer.h"

#define ONE_TENTH_SECOND 100
#define ONE_HUNDREDTH_SECOND 10

Wall *wall;
Timer *ledTimer;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting setup()");
    Serial.println("Forest Park Circuit Wall starting up");

    wall = new Wall(new DeviceFactory());

    if (!wall->initialize())
        Serial.println("Initialization of I2C devices failed");
    wall->lcdSetBacklightColor(0, 255, 50);
    wall->lcdPrintAt(0, 0, "Forest Park");
    wall->lcdPrintAt(0, 1, "Elementary");
    wall->turnIndicatorOn(INDICATE_NEGATIVE_POLE);
    wall->turnIndicatorOn(INDICATE_POSITIVE_POLE);

    Serial.println("Exiting setup()");
}

input_hex energizedInput;
output_hex energizedOutput;
circuit_end inputConnection,
            outputConnection;
bool circuitComplete;


void driveMotor(wall_motor motor, uint16_t value, const int threshold)
{
    bool motorIsClockwise = (value > threshold);
    bool motorSpeed = (value % threshold);
    if (motorIsClockwise)
        wall->setMotorDirectionClockwise(motor);
    else
        wall->setMotorDirectionCounterClockwise(motor);
    wall->setMotorSpeed(motor, motorSpeed);
}

void driveTransducer(uint16_t value, const int THRESHOLD)
{
    if (value > THRESHOLD)
        wall->turnTransducerOn();
    else
        wall->turnTransducerOff();
}

void driveTwoPartLED(led_array array, uint16_t value, const int THRESHOLD)
{
    if (value > 0)
        wall->turnOnLEDarray(array, LEFT_SIDE);
    else
        wall->turnOffLEDarray(array, LEFT_SIDE);

    if (value > THRESHOLD)
        wall->turnOnLEDarray(array, RIGHT_SIDE);
    else
        wall->turnOffLEDarray(array, RIGHT_SIDE);
}

void driveFourPartLED(uint16_t value, const int THRESHOLD)
{
    if (value > 0)
        wall->turnOnLEDarray(RED_LED, LOWER_LEFT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, LOWER_LEFT_SIDE);

    if (value > (THRESHOLD / 2))
        wall->turnOnLEDarray(RED_LED, LEFT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, LEFT_SIDE);

    if (value > THRESHOLD)
        wall->turnOnLEDarray(RED_LED, RIGHT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, RIGHT_SIDE);

    if (value > (3 * THRESHOLD / 2))
        wall->turnOnLEDarray(RED_LED, LOWER_RIGHT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, LOWER_RIGHT_SIDE);
}

uint16_t getInputHexValue(void)
{
    switch (energizedInput)
    {
        case KNOB_HEX:
            return wall->getKnobPosition(); 
        case SLIDER_HEX:
            return wall->getSliderPosition();
        case PHOTO_SENSOR_HEX:
            return (wall->getPhotoSensorValue(LEFT_PHOTO) +
                    wall->getPhotoSensorValue(CENTER_PHOTO) +
                    wall->getPhotoSensorValue(RIGHT_PHOTO));
        case TOUCH_SENSOR_HEX:
            return (wall->getTouchSensorValue(LEFT_TOUCH) +
                    wall->getTouchSensorValue(BOTTOM_TOUCH) +
                    wall->getTouchSensorValue(RIGHT_TOUCH));
        case JOYSTICK_HEX:
            return PWM_FULL_DUTY_CYCLE *
               (wall->isJoystickDown() || 
                wall->isJoystickLeft() ||
                wall->isJoystickRight() ||
                wall->isJoystickUp());
        case TOGGLE_SWITCH_HEX:
            return (
                (wall->isToggleOn(LEFT_TOGGLE)   * (PWM_FULL_DUTY_CYCLE / 2)) +
                (wall->isToggleOn(CENTER_TOGGLE) * (PWM_FULL_DUTY_CYCLE / 4)) +
                (wall->isToggleOn(RIGHT_TOGGLE)  * (PWM_FULL_DUTY_CYCLE / 8)));
        default: return 0;
    }
}

unsigned int sawtoothCycle()
{
    int cycle = millis() % PWM_FULL_DUTY_CYCLE;
    if (cycle < PWM_FULL_DUTY_CYCLE / 2)
        return cycle;
    else
        return (PWM_FULL_DUTY_CYCLE - cycle);
}

input_hex findConnectedInputHex()
{
    for (int hex = KNOB_HEX; hex <= TOUCH_SENSOR_HEX; hex++)
    {
        input_hex in = static_cast<input_hex>(hex);
        circuit_end end = Wall::leftCircuitForInput(in);
        if (wall->isCircuitConnected(CIRCUIT_POSITIVE_POLE, end))
            return in;
    }
    return NO_INPUT;
}
output_hex findConnectedOutputHex()
{
    for (int hex = WHITE_LED_HEX; hex <= TRANSDUCER_HEX; hex++)
    {
        output_hex out = static_cast<output_hex>(hex);
        circuit_end end = Wall::rightCircuitForOutput(out);
        if (wall->isCircuitConnected(CIRCUIT_POSITIVE_POLE, end))
            return out;
    }
    return NO_OUTPUT;
}

void driveOutputHex(uint16_t value)
{
    const int THRESHOLD = 2048;
    switch (energizedOutput)
    {
    case CIRCUIT_BLUE_MOTOR_RIGHT:
        driveMotor(BLUE_MOTOR, value, THRESHOLD);
        break;
    case CIRCUIT_ORANGE_MOTOR_RIGHT:
        driveMotor(ORANGE_MOTOR, value, THRESHOLD);
        break;
    case CIRCUIT_TRANSDUCER_RIGHT:
        driveTransducer(value, THRESHOLD);
        break;
    case CIRCUIT_WHITE_LED_RIGHT:
        driveTwoPartLED(WHITE_LED, value, THRESHOLD);
        break;
    case CIRCUIT_GREEN_LED_RIGHT:
        driveTwoPartLED(GREEN_LED, value, THRESHOLD);
        break;
    case CIRCUIT_RED_LED_RIGHT:
        driveFourPartLED(value, THRESHOLD);
        break;
    default: break;
    }
}

void collectCircuitConnections()
{
    energizedInput = findConnectedInputHex();
    energizedOutput = findConnectedOutputHex();
    inputConnection = Wall::rightCircuitForInput(energizedInput);
    outputConnection = Wall::rightCircuitForOutput(energizedOutput);

    circuitComplete = wall->isCircuitConnected(inputConnection, outputConnection) &&
        (energizedInput != NO_INPUT) &&
        (energizedOutput != NO_OUTPUT);
}



void lightIndicatorsForConnectedCircuits()
{
    if (circuitComplete)
    {
        wall->turnIndicatorOn(Wall::indicatorforInput(energizedInput));
        wall->turnIndicatorOn(Wall::indicatorForOutput(energizedOutput));
    }
    else
    {
        if (energizedInput != CIRCUIT_POSITIVE_POLE)
            wall->setIndicatorBrightness(Wall::indicatorforInput(energizedInput), sawtoothCycle());
        if (energizedOutput != CIRCUIT_NEGATIVE_POLE)
            wall->setIndicatorBrightness(Wall::indicatorForOutput(energizedOutput), sawtoothCycle());
    }
}


void turnOffAllOutputHexes()
{
    wall->turnTransducerOff();
    wall->turnOffLEDarray(GREEN_LED, LEFT_SIDE);
    wall->turnOffLEDarray(GREEN_LED, RIGHT_SIDE);
    wall->turnOffLEDarray(WHITE_LED, LEFT_SIDE);
    wall->turnOffLEDarray(WHITE_LED, RIGHT_SIDE);
    wall->turnOffLEDarray(RED_LED, LEFT_SIDE);
    wall->turnOffLEDarray(RED_LED, RIGHT_SIDE);
    wall->turnOffLEDarray(RED_LED, LOWER_LEFT_SIDE);
    wall->turnOffLEDarray(RED_LED, LOWER_RIGHT_SIDE);
    wall->stopMotor(BLUE_MOTOR);
    wall->stopMotor(ORANGE_MOTOR);
}

void timerDelay(unsigned long time)
{
    ledTimer = new Timer(time);
    while (!ledTimer->expired());
}

void loop()
{
    collectCircuitConnections();
    lightIndicatorsForConnectedCircuits();
    if (circuitComplete)
        driveOutputHex(getInputHexValue());
    else 
        turnOffAllOutputHexes();
    timerDelay(ONE_HUNDREDTH_SECOND);
}
