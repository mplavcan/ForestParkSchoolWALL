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
#include "HexCircuits.h"
#include "Timer.h"

#define ONE_TENTH_SECOND 100

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
    wall->turnIndicatorOn(Wall::indicatorForCircuit(CIRCUIT_POSITIVE_POLE));
    wall->turnIndicatorOn(Wall::indicatorForCircuit(CIRCUIT_NEGATIVE_POLE));

    Serial.println("Exiting setup()");
}

circuit_end energizedInput,
            energizedOutput,
            connectionInput,
            connectionOutput;
indicator_led inputIndicator, outputIndicator;
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
        case CIRCUIT_KNOB_LEFT:      
            return wall->getKnobPosition(); 
        case CIRCUIT_SLIDER_LEFT:
            return wall->getSliderPosition();
        case CIRCUIT_PHOTO_LEFT:
            return (wall->getPhotoSensorValue(LEFT_PHOTO) +
                    wall->getPhotoSensorValue(CENTER_PHOTO) +
                    wall->getPhotoSensorValue(RIGHT_PHOTO));
        case CIRCUIT_TOUCH_LEFT:
            return (wall->getTouchSensorValue(LEFT_TOUCH) +
                    wall->getTouchSensorValue(BOTTOM_TOUCH) +
                    wall->getTouchSensorValue(RIGHT_TOUCH));
        case CIRCUIT_JOYSTICK_LEFT:
            return PWM_FULL_DUTY_CYCLE *
               (wall->isJoystickDown() || 
                wall->isJoystickLeft() ||
                wall->isJoystickRight() ||
                wall->isJoystickUp());
        case CIRCUIT_TOGGLE_LEFT:
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

circuit_end inputHexCircuitLeft()
{
    for (int hex = 0; hex < NUMBER_INPUT_HEXES; hex++)
    {
        circuit_end end = input_hexes_left[hex];
        if (wall->isCircuitConnected(CIRCUIT_POSITIVE_POLE, end))
            return end;
    }
    return CIRCUIT_POSITIVE_POLE;
}
circuit_end inputHexCircuitRight()
{
    for (int hex = 0; hex < NUMBER_INPUT_HEXES; hex++)
    {
        circuit_end end = input_hexes_right[hex];
        if (wall->isCircuitConnected(CIRCUIT_POSITIVE_POLE, end))
            return end;
    }
    return CIRCUIT_POSITIVE_POLE;
}
circuit_end outputHexCircuitLeft()
{
    for (int hex = 0; hex < NUMBER_OUTPUT_HEXES; hex++)
    {
        circuit_end end = output_hexes_left[hex];
        if (wall->isCircuitConnected(CIRCUIT_NEGATIVE_POLE, end))
            return end;
    }
    return CIRCUIT_NEGATIVE_POLE;
}
circuit_end outputHexCircuitRight()
{
    for (int hex = 0; hex < NUMBER_OUTPUT_HEXES; hex++)
    {
        circuit_end end = output_hexes_right[hex];
        if (wall->isCircuitConnected(CIRCUIT_NEGATIVE_POLE, end))
            return end;
    }
    return CIRCUIT_NEGATIVE_POLE;
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
    energizedInput = inputHexCircuitLeft();
    energizedOutput = outputHexCircuitRight();
    connectionInput = inputHexCircuitRight();
    connectionOutput = outputHexCircuitLeft();
    inputIndicator = Wall::indicatorForCircuit(energizedInput);
    outputIndicator = Wall::indicatorForCircuit(energizedOutput);

    circuitComplete = wall->isCircuitConnected(connectionInput, connectionOutput) &&
        (energizedInput != CIRCUIT_POSITIVE_POLE) &&
        (energizedOutput != CIRCUIT_NEGATIVE_POLE);
}



void lightIndicatorsForConnectedCircuits()
{
    if (circuitComplete)
    {
        wall->turnIndicatorOn(inputIndicator);
        wall->turnIndicatorOn(outputIndicator);
    }
    else
    {
        if (energizedInput != CIRCUIT_POSITIVE_POLE)
            wall->setIndicatorBrightness(inputIndicator, sawtoothCycle());
        if (energizedOutput != CIRCUIT_NEGATIVE_POLE)
            wall->setIndicatorBrightness(outputIndicator, sawtoothCycle());
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

void loop()
{
    collectCircuitConnections();
    lightIndicatorsForConnectedCircuits();
    if (wall->isCircuitConnected(connectionInput, connectionOutput))
        driveOutputHex(getInputHexValue());
    else 
        turnOffAllOutputHexes();
    ledTimer = new Timer(100);
    while (!ledTimer->expired());
}
