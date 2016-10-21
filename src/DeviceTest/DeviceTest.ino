// Basic exercise of all forest Park School Wall components
//
#include <Arduino.h>
#include <Wire.h>
#include <SparkFunSX1509.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_ADS1015.h>
#include "rgb_lcd.h"
#include "DeviceFactory.h"
#include "Wall.h"

Wall *wall;
rgb_lcd lcd;

unsigned int counter = 0;

void setup() {
    Serial.begin(115200);

    DeviceFactory *io = new DeviceFactory();
    wall = new Wall(io);

    if(!wall->initialize())
        Serial.println("Initialization of I2C devices failed");
    wall->setMultiplexerI2CBus(GROVE_LCD_I2C_BUS);
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setRGB(0, 255, 50);
    lcd.setCursor(0, 0);
    lcd.print("Forest Park");
    lcd.setCursor(0, 1);
    lcd.print("Elementary");
}


void lightButtonIfPressed(large_button color)
{
    if (wall->isButtonDepressed(color))
    {
        Serial.println(String(color) + "depressed");
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
    Serial.print(String(sensor) + ": "); Serial.println(value);
}

void printPressureValues(force_sensor sensor)
{
    int16_t value = wall->getTouchSensorValue(sensor);
    Serial.print(String(sensor) + ": "); Serial.println(value);
}

void printToggleState(toggle_switch toggle)
{
    String state = wall->isToggleOn(toggle) ? "ON" : "OFF";
    Serial.print(String(toggle) + ": "+ state) ;
}


void printJoystickDirection()
{
    if (wall->isJoystickUp()) Serial.println("Joystick Down");
    if (wall->isJoystickDown()) Serial.println("Joystick Up");
    if (wall->isJoystickLeft()) Serial.println("Joystick Left");
    if (wall->isJoystickRight()) Serial.println("Joystick Rright");
}

void ControlMotor(wall_motor motor, int16_t controlValue)
{
    const int THRESHOLD = 512;
    if (controlValue > THRESHOLD) wall->setMotorDirectionClockwise(motor);
    if (controlValue < THRESHOLD) wall->setMotorDirectionCounterClockwise(motor);
    if (controlValue == THRESHOLD) wall->stopMotor(motor);
    wall->setMotorSpeed(motor, controlValue >> 5);
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

    printPressureValues(LEFT_PRESSURE);
    printPressureValues(BOTTOM_PRESSURE);
    printPressureValues(RIGHT_PRESSURE);

    printToggleState(LEFT_TOGGLE);
    printToggleState(CENTER_TOGGLE);
    printToggleState(RIGHT_TOGGLE);

    printJoystickDirection();

    int16_t slider = wall->getSliderPosition();
    Serial.print("SLIDER: "); Serial.println(slider);

    int16_t knob = wall->getKnobPosition();
    Serial.print("KNOB: "); Serial.println(knob);

    ControlMotor(BLUE_MOTOR, knob);
    ControlMotor(ORANGE_MOTOR, slider);

    for(int grid = CIRCUIT_KNOB_LEFT; grid <= CIRCUIT_NEGATIVE_POLE; grid++)
    {
        if(wall->readCircuitState(static_cast<circuit_end>(grid)) == LOW)
        {
            Serial.println(String(grid) + " triggered LOW");
            wall->turnTransducerOn();
        }
    }

    if(counter == 0)
    {
        wall->turnOffLEDarray(GREEN_LED, LEFT_SIDE);
        wall->turnOffLEDarray(GREEN_LED, RIGHT_SIDE);
        wall->turnOffLEDarray(WHITE_LED, LEFT_SIDE);
        wall->turnOffLEDarray(WHITE_LED, RIGHT_SIDE);
        wall->turnOffLEDarray(RED_LED, LEFT_SIDE);
        wall->turnOffLEDarray(RED_LED, RIGHT_SIDE);
        wall->turnOffLEDarray(RED_LED, LOWER_LEFT_SIDE);
        wall->turnOffLEDarray(RED_LED, LOWER_RIGHT_SIDE);
        wall->illuminateELWire(RED_WIRE_ONE);
        wall->illuminateELWire(RED_WIRE_TWO);
        wall->illuminateELWire(GREEN_WIRE_ONE);
        wall->illuminateELWire(GREEN_WIRE_TWO);
        wall->illuminateELWire(YELLOW_WIRE);
        wall->illuminateELWire(WHITE_WIRE);
        wall->illuminateELWire(BLUE_WIRE_ONE);
        wall->illuminateELWire(BLUE_WIRE_TWO);
        wall->turnTransducerOff();
    }

    switch (counter)
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

    counter++;
    delay(100);
}
