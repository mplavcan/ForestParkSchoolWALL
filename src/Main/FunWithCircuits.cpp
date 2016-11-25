// Application object for Forest Park School Wall
//
#include "FunWithCircuits.h"

bool FunWithCircuits::setup(void)
{
    if (!wall->initialize())
        return false;
    wall->lcdSetBacklightColor(0, 255, 50);
    wall->lcdPrintAt(0, 0, "Forest Park");
    wall->lcdPrintAt(0, 1, "Elementary");
    wall->turnIndicatorOn(INDICATE_NEGATIVE_POLE);
    wall->turnIndicatorOn(INDICATE_POSITIVE_POLE);
    return true;
}

void FunWithCircuits::driveMotor(wall_motor motor, unsigned int value, const unsigned int midpoint)
{
    bool motorIsClockwise = (value > midpoint);
    unsigned int motorSpeed = (value % midpoint);
    if (motorIsClockwise)
        wall->setMotorDirectionClockwise(motor);
    else
        wall->setMotorDirectionCounterClockwise(motor);
    wall->setMotorSpeed(motor, motorSpeed);
}

void FunWithCircuits::driveTransducer(unsigned int value, const unsigned int midpoint)
{
    if (value > midpoint)
        wall->turnTransducerOn();
    else
        wall->turnTransducerOff();
}

void FunWithCircuits::driveTwoPartLED(led_array array, unsigned int value, const unsigned int midpoint)
{
    if (value > 0)
        wall->turnOnLEDarray(array, LEFT_SIDE);
    else
        wall->turnOffLEDarray(array, LEFT_SIDE);

    if (value > midpoint)
        wall->turnOnLEDarray(array, RIGHT_SIDE);
    else
        wall->turnOffLEDarray(array, RIGHT_SIDE);
}

void FunWithCircuits::driveFourPartLED(unsigned int value, const unsigned int midpoint)
{
    if (value > 0)
        wall->turnOnLEDarray(RED_LED, LOWER_LEFT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, LOWER_LEFT_SIDE);

    if (value > (midpoint / 2))
        wall->turnOnLEDarray(RED_LED, LEFT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, LEFT_SIDE);

    if (value > midpoint)
        wall->turnOnLEDarray(RED_LED, RIGHT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, RIGHT_SIDE);

    if (value > (3 * midpoint / 2))
        wall->turnOnLEDarray(RED_LED, LOWER_RIGHT_SIDE);
    else
        wall->turnOffLEDarray(RED_LED, LOWER_RIGHT_SIDE);
}

unsigned int FunWithCircuits::getInputHexValue(void)
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

unsigned int FunWithCircuits::sawtoothCycle(unsigned long currentTime)
{
    int cycle = currentTime % PWM_FULL_DUTY_CYCLE;
    if (cycle < PWM_FULL_DUTY_CYCLE / 2)
        return cycle;
    else
        return (PWM_FULL_DUTY_CYCLE - cycle);
}

input_hex FunWithCircuits::findConnectedInputHex(void)
{
    for (int hex = KNOB_HEX; hex <= TOUCH_SENSOR_HEX; hex++)
    {
        input_hex in = static_cast<input_hex>(hex);
        circuit_end end = wall->leftCircuitForInput(in);
        if (wall->isCircuitConnected(CIRCUIT_POSITIVE_POLE, end))
            return in;
    }
    return NO_INPUT;
}
output_hex FunWithCircuits::findConnectedOutputHex(void)
{
    for (int hex = WHITE_LED_HEX; hex <= TRANSDUCER_HEX; hex++)
    {
        output_hex out = static_cast<output_hex>(hex);
        circuit_end end = wall->rightCircuitForOutput(out);
        if (wall->isCircuitConnected(CIRCUIT_NEGATIVE_POLE, end))
            return out;
    }
    return NO_OUTPUT;
}

void FunWithCircuits::driveOutputHex(unsigned int value)
{
    const int midpoint = 2048;
    const int motorMidpoint = 500;
    switch (energizedOutput)
    {
    case BLUE_MOTOR_HEX:
        driveMotor(BLUE_MOTOR, value, motorMidpoint);
        break;
    case ORANGE_MOTOR_HEX:
        driveMotor(ORANGE_MOTOR, value, motorMidpoint);
        break;
    case TRANSDUCER_HEX:
        driveTransducer(value, midpoint);
        break;
    case WHITE_LED_HEX:
        driveTwoPartLED(WHITE_LED, value, midpoint);
        break;
    case GREEN_LED_HEX:
        driveTwoPartLED(GREEN_LED, value, midpoint);
        break;
    case RED_LED_HEX:
        driveFourPartLED(value, midpoint);
        break;
    default: break;
    }
}

void FunWithCircuits::collectCircuitConnections(void)
{
    energizedInput = findConnectedInputHex();
    energizedOutput = findConnectedOutputHex();
    bool inputAndOutputConnected = wall->isCircuitConnected(
        wall->rightCircuitForInput(energizedInput),
        wall->leftCircuitForOutput(energizedOutput));

    circuitComplete = inputAndOutputConnected &&
        (energizedInput != NO_INPUT) &&
        (energizedOutput != NO_OUTPUT);
}

void FunWithCircuits::lightIndicatorsForConnectedCircuits(unsigned long currentTime)
{
    if (circuitComplete)
    {
        indicator_led inputLamp = wall->indicatorforInput(energizedInput);
        indicator_led outputLamp = wall->indicatorForOutput(energizedOutput);
        wall->turnIndicatorOn(inputLamp);
        wall->turnIndicatorOn(outputLamp);
    }
    else
    {
        for (int hex = KNOB_HEX; hex <= TOUCH_SENSOR_HEX; hex++)
        {
            input_hex in = static_cast<input_hex>(hex);
            indicator_led inputLamp = wall->indicatorforInput(in);
            if (in == energizedInput)
                wall->setIndicatorBrightness(inputLamp, sawtoothCycle(currentTime));
            else
                wall->setIndicatorBrightness(inputLamp, 0);
        }
        for (int hex = WHITE_LED_HEX; hex <= TRANSDUCER_HEX; hex++)
        {
            output_hex out = static_cast<output_hex>(hex);
            indicator_led outputLamp = wall->indicatorForOutput(out);
            if (out == energizedOutput)
                wall->setIndicatorBrightness(outputLamp, sawtoothCycle(currentTime));
            else
                wall->setIndicatorBrightness(outputLamp, 0);
        }
    }
}


EL_wire FunWithCircuits::chooseELwireForButton(large_button color)
{
    switch (color)
    {
    case RED_BUTTON: return RED_WIRE_ONE;
    case YELLOW_BUTTON: return YELLOW_WIRE;
    case WHITE_BUTTON: return WHITE_WIRE;
    case BLUE_BUTTON: return BLUE_WIRE_ONE;
    case GREEN_BUTTON: return GREEN_WIRE_ONE;
    default: return RED_WIRE_TWO;
    }
}

void FunWithCircuits::lightButtonIfPressed(large_button color, unsigned long time)
{
    if (wall->isButtonDepressed(color))
    {
        wall->illuminateButton(color);
        wall->illuminateELWire(chooseELwireForButton(color));
        lightsOnTime = time;
    }
    else
        wall->extinguishButton(color);
}


void FunWithCircuits::turnOffAllELwires()
{
    wall->extinguishELWire(RED_WIRE_ONE);
    wall->extinguishELWire(RED_WIRE_TWO);
    wall->extinguishELWire(GREEN_WIRE_ONE);
    wall->extinguishELWire(GREEN_WIRE_TWO);
    wall->extinguishELWire(YELLOW_WIRE);
    wall->extinguishELWire(WHITE_WIRE);
    wall->extinguishELWire(BLUE_WIRE_ONE);
    wall->extinguishELWire(BLUE_WIRE_TWO);
}

void FunWithCircuits::turnOffAllOutputHexes(void)
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

void FunWithCircuits::lightELwiresFromButtons(unsigned long currentTime)
{
    lightButtonIfPressed(BLUE_BUTTON, currentTime);
    lightButtonIfPressed(RED_BUTTON, currentTime);
    lightButtonIfPressed(GREEN_BUTTON, currentTime);
    lightButtonIfPressed(YELLOW_BUTTON, currentTime);
    lightButtonIfPressed(WHITE_BUTTON, currentTime);

    if (currentTime > lightsOnTime + (2 * ONE_SECOND))
        turnOffAllELwires();
}

void FunWithCircuits::loop(unsigned long elapsedTime)
{
    lightELwiresFromButtons(elapsedTime);

    collectCircuitConnections();
    lightIndicatorsForConnectedCircuits(elapsedTime);

    if (circuitComplete)
        driveOutputHex(getInputHexValue());
    else
        turnOffAllOutputHexes();
}
