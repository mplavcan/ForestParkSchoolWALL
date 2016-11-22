// Basic exercise of all forest Park School Wall components
//
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include "DeviceFactory.h"
#include "Wall.h"

Wall *wall;
unsigned int counter = 0;

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Forest Park Circuit Wall Setup");

    wall = (new Wall)->usingFactory(new DeviceFactory);

    // BEGIN_WORKAROUND:
    // For some odd reason, the LCD display routine inside Wall is 
    // dropping the second print, and highlighting the upper line.
    // A nearly identical piece of code in Main.ino _works_perfectly_
    // Adding this separate initializtion appears to avoid the issue
    {
        rgb_lcd localLCD;
        Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
        Wire.write(1);
        localLCD.begin(16, 2);
    }
    // END WORKAROUND

    if(!wall->initialize())
        Serial.println("Initialization of I2C devices failed");

    wall->lcdSetBacklightColor(180, 100, 10);
    wall->lcdPrintAt(0, 0, "Wall Interface");
    wall->lcdPrintAt(0, 1, "Device Test");
    
    wall->setCircuitAsOutput(CIRCUIT_POSITIVE_POLE);
    wall->setCircuitAsOutput(CIRCUIT_NEGATIVE_POLE);
    wall->turnIndicatorOn(INDICATE_POSITIVE_POLE);
    wall->turnIndicatorOn(INDICATE_NEGATIVE_POLE);
}


void lightButtonIfPressed(large_button color)
{
    if (wall->isButtonDepressed(color))
    {
        Serial.println("Large Button" + String(color) + " depressed");
        wall->illuminateButton(color);
    }
    else
    {
        wall->extinguishButton(color);
    }
}


void printPhotoValues(photo_sensor sensor)
{
    int16_t value = wall->getPhotoSensorValue(sensor);
    String sensorName[3] = { "LEFT_PHOTO", "CENTER_PHOTO", "RIGHT_PHOTO" };
    Serial.print(sensorName[sensor] + ": "); Serial.println(value);
}

void printTouchValues(force_sensor sensor)
{
    int16_t value = wall->getTouchSensorValue(sensor);
    String sensorName[3] = { "LEFT_TOUCH", "BOTTOM_TOUCH", "RIGHT_TOUCH" };
    Serial.print(sensorName[sensor] + ": "); Serial.println(value);
}

void printToggleState(toggle_switch toggle)
{
    String state = wall->isToggleOn(toggle) ? "ON" : "OFF";
    String toggleName[3] = { "LEFT_TOGGLE", "CENTER_TOGGLE", "RIGHT_TOGGLE" };
    Serial.println(toggleName[toggle] + ": "+ state) ;
}


void printJoystickDirection()
{
    if (wall->isJoystickUp()) Serial.println("Joystick Up");
    if (wall->isJoystickDown()) Serial.println("Joystick Down");
    if (wall->isJoystickLeft()) Serial.println("Joystick Left");
    if (wall->isJoystickRight()) Serial.println("Joystick Rright");
}

void ControlMotor(wall_motor motor, int16_t controlValue)
{
    const int THRESHOLD = 512;
    const int DEADZONE = 50;
    const unsigned char maximumSpeed = 50;
    if (controlValue > THRESHOLD + DEADZONE) wall->setMotorDirectionClockwise(motor);
    else if (controlValue < THRESHOLD - DEADZONE) wall->setMotorDirectionCounterClockwise(motor);
    else wall->stopMotor(motor);
    unsigned char speed = (controlValue >> 5);
    wall->setMotorSpeed(motor, min(speed, maximumSpeed));
}

void turnOffAllLights()
{
    wall->turnOffLEDarray(GREEN_LED, LEFT_SIDE);
    wall->turnOffLEDarray(GREEN_LED, RIGHT_SIDE);
    wall->turnOffLEDarray(WHITE_LED, LEFT_SIDE);
    wall->turnOffLEDarray(WHITE_LED, RIGHT_SIDE);
    wall->turnOffLEDarray(RED_LED, LEFT_SIDE);
    wall->turnOffLEDarray(RED_LED, RIGHT_SIDE);
    wall->turnOffLEDarray(RED_LED, LOWER_LEFT_SIDE);
    wall->turnOffLEDarray(RED_LED, LOWER_RIGHT_SIDE);
    wall->extinguishELWire(RED_WIRE_ONE);
    wall->extinguishELWire(RED_WIRE_TWO);
    wall->extinguishELWire(GREEN_WIRE_ONE);
    wall->extinguishELWire(GREEN_WIRE_TWO);
    wall->extinguishELWire(YELLOW_WIRE);
    wall->extinguishELWire(WHITE_WIRE);
    wall->extinguishELWire(BLUE_WIRE_ONE);
    wall->extinguishELWire(BLUE_WIRE_TWO);
    for (int lamp = INDICATE_WHITE_LED; lamp <= INDICATE_TOUCH_SENSOR; lamp++)
        wall->turnIndicatorOff(static_cast<indicator_led>(lamp));
}

void turnOnSelectiveLight(int choice)
{
    switch (choice)
    {
        case 1: wall->turnOnLEDarray(GREEN_LED, LEFT_SIDE); break;
        case 2: wall->turnOnLEDarray(GREEN_LED, RIGHT_SIDE); break;
        case 3: wall->turnOnLEDarray(WHITE_LED, LEFT_SIDE); break;
        case 4: wall->turnOnLEDarray(WHITE_LED, RIGHT_SIDE); break;
        case 5: wall->turnOnLEDarray(RED_LED, LEFT_SIDE); break;
        case 6: wall->turnOnLEDarray(RED_LED, RIGHT_SIDE); break;
        case 7: wall->turnOnLEDarray(RED_LED, LOWER_LEFT_SIDE); break;
        case 8: wall->turnOnLEDarray(RED_LED, LOWER_RIGHT_SIDE); break;
        case 9: wall->illuminateELWire(RED_WIRE_ONE); break;
        case 10: wall->illuminateELWire(RED_WIRE_TWO); break;
        case 11: wall->illuminateELWire(GREEN_WIRE_ONE); break;
        case 12: wall->illuminateELWire(GREEN_WIRE_TWO); break;
        case 13: wall->illuminateELWire(YELLOW_WIRE); break;
        case 14: wall->illuminateELWire(WHITE_WIRE); break;
        case 15: wall->illuminateELWire(BLUE_WIRE_ONE); break;
        case 16: wall->illuminateELWire(BLUE_WIRE_TWO); break;
        case 17: wall->turnTransducerOn(); break;
        default: break;
    }
}

void indicateInputCircuitState(input_hex hex)
{
    indicator_led lamp = Wall::indicatorforInput(hex);
    circuit_end left = Wall::leftCircuitForInput(hex);
    circuit_end right = Wall::rightCircuitForInput(hex);
    if (wall->readCircuitState(left) == LOW)
    {
        Serial.println("Left side input "+ String(hex) + " circuit triggered");
        wall->turnIndicatorOn(lamp);
        wall->turnTransducerOn();
    }
    if (wall->readCircuitState(right) == LOW)
    {
        Serial.println("Right side input " + String(hex) + " circuit triggered");
        wall->turnIndicatorOn(lamp);
        wall->turnTransducerOn();
    }
}

void indicateOutputCircuitState(output_hex hex)
{
    indicator_led lamp = Wall::indicatorForOutput(hex);
    circuit_end left = Wall::leftCircuitForOutput(hex);
    circuit_end right = Wall::rightCircuitForOutput(hex);
    if (wall->readCircuitState(left) == LOW)
    {
        Serial.println("Left side output " + String(hex) + " circuit triggered");
        wall->turnIndicatorOn(lamp);
        wall->turnTransducerOn();
    }
    if (wall->readCircuitState(right) == LOW)
    {
        Serial.println("Right side output " + String(hex) + " circuit triggered");
        wall->turnIndicatorOn(lamp);
        wall->turnTransducerOn();
    }
}

void loop() {
    lightButtonIfPressed(BLUE_BUTTON);
    lightButtonIfPressed(RED_BUTTON);
    lightButtonIfPressed(GREEN_BUTTON);
    lightButtonIfPressed(YELLOW_BUTTON);
    lightButtonIfPressed(WHITE_BUTTON);

    printPhotoValues(LEFT_PHOTO);
    printPhotoValues(CENTER_PHOTO);
    printPhotoValues(RIGHT_PHOTO);

    printTouchValues(LEFT_TOUCH);
    printTouchValues(BOTTOM_TOUCH);
    printTouchValues(RIGHT_TOUCH);

    printToggleState(LEFT_TOGGLE);
    printToggleState(CENTER_TOGGLE);
    printToggleState(RIGHT_TOGGLE);

    printJoystickDirection();

    int16_t slider = wall->getSliderPosition();
    Serial.print("Slider: "); Serial.println(slider);
    ControlMotor(ORANGE_MOTOR, slider);

    int16_t knob = wall->getKnobPosition();
    Serial.print("Knob: "); Serial.println(knob);
    ControlMotor(BLUE_MOTOR, knob);

    for (int hex = KNOB_HEX; hex <= TOUCH_SENSOR_HEX; hex++)
        indicateInputCircuitState(static_cast<input_hex>(hex));
    for (int hex = WHITE_LED_HEX; hex <= TRANSDUCER_HEX; hex++)
        indicateOutputCircuitState(static_cast<output_hex>(hex));

    if (counter == 0)
    {
        turnOffAllLights();
        wall->turnTransducerOff();
    }
    else
        turnOnSelectiveLight(counter);

    counter++;
    if (counter == 18)
        counter = 0;
    delay(100);
}
