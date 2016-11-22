// Raw I/O test of all Wall devices
// Forest Park School Wall Project 
//
// This file is the original, non-tested code used to bring up
// the individual devices, when the hardware abstraction layer
// (Wall.cpp) was not yet developed.  It is useful in cases where 
// the board nees testing, but the software is suspect.  A computer
// with the serial monitor MUST be attached while running this sketch.
//
#include <Wire.h>
#include <rgb_lcd.h>
#include <SparkFunSX1509.h> 
#include <Adafruit_ADS1015.h>
#include <Adafruit_PWMServoDriver.h>
#include "Wall_IO_pins.h"

int active_EL = 0x100;
int pwm_frequency = 2000;

SX1509 io_1;
SX1509 io_2;
SX1509 io_3;
SX1509 io_4;

Adafruit_ADS1015 analog_1 = Adafruit_ADS1015(ADAFRUIT_ANALOG_FIRST_I2C_ADDRESS);
Adafruit_ADS1015 analog_2 = Adafruit_ADS1015(ADAFRUIT_ANALOG_SECOND_I2C_ADDRESS);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(ADAFRUIT_PWM_I2C_ADDRESS);

rgb_lcd lcd;


int multiplexer_select_i2c_bus(uint8_t bus) {
    if (bus > 7)
        return 0;
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << bus);
    return Wire.endTransmission();
}

// Due to a bug with Arduino101 and the TCS9548a,
//  Sometimes writing the bus value will result in an error
//  Loop here to guarantee that it's set before proceeding
//  Have never seen a scenarios where it has back-to-back errors
void select_i2c_path(uint8_t bus) {
    while (multiplexer_select_i2c_bus(bus));
}
void setup()
{
    while (!Serial);  // Will only start up once the Serial monitor is launched.
    delay(1000);
    Serial.begin(115200);
    Wire.begin();

    Serial.println("Starting I2C communication...");
    select_i2c_path(SPARKFUN_SX1509_THIRD_I2C_BUS);
    if (!io_3.begin(SPARKFUN_SX1509_THIRD_I2C_ADDRESS)) {
        Serial.println("0x70 FAILED.");
    }
    select_i2c_path(SPARKFUN_SX1509_FOURTH_I2C_BUS);
    if (!io_4.begin(SPARKFUN_SX1509_FOURTH_I2C_ADDRESS)) {
        Serial.println("0x71 FAILED.");
    }
    select_i2c_path(SPARKFUN_SX1509_FIRST_I2C_BUS);
    if (!io_1.begin(SPARKFUN_SX1509_FIRST_I2C_ADDRESS)) {
        Serial.println("0x3E FAILED.");
    }
    select_i2c_path(SPARKFUN_SX1509_SECOND_I2C_BUS);
    if (!io_2.begin(SPARKFUN_SX1509_SECOND_I2C_ADDRESS)) {
        Serial.println("0x3F FAILED.");
    }
    Serial.println("I2C I/O Initialized.");

    select_i2c_path(ADAFRUIT_ANALOG_FIRST_I2C_BUS);
    analog_1.begin();
    select_i2c_path(ADAFRUIT_ANALOG_SECOND_I2C_BUS);
    analog_2.begin();
    Serial.println("I2C ADS Initialized.");

    select_i2c_path(GROVE_LCD_I2C_BUS);
    lcd.begin(16, 2);
    lcd.clear();
    Serial.println("I2C LCD Initialized.");

    lcd.setRGB(0, 255, 50);
    lcd.setCursor(0, 0);
    lcd.print("Forest Park");
    lcd.setCursor(0, 1);
    lcd.print("Elementary");

    select_i2c_path(SPARKFUN_SX1509_THIRD_I2C_BUS);
    io_3.pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
    io_3.pinMode(BLUE_LED_PIN, OUTPUT);
    io_4.pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);
    io_4.pinMode(YELLOW_LED_PIN, OUTPUT);

    io_4.pinMode(INPUT_TOGGLE_1, INPUT_PULLUP);
    io_4.pinMode(INPUT_TOGGLE_2, INPUT_PULLUP);
    io_4.pinMode(INPUT_TOGGLE_3, INPUT_PULLUP);
    io_4.pinMode(INPUT_JOYSTICK_UP, INPUT_PULLUP);
    io_4.pinMode(INPUT_JOYSTICK_DOWN, INPUT_PULLUP);
    io_4.pinMode(INPUT_JOYSTICK_LEFT, INPUT_PULLUP);
    io_4.pinMode(INPUT_JOYSTICK_RIGHT, INPUT_PULLUP);

    io_3.pinMode(GRID_ROTARY_POT_LEFT, INPUT_PULLUP);
    io_3.pinMode(GRID_ROTARY_POT_RIGHT, INPUT_PULLUP);
    io_3.pinMode(GRID_LINEAR_POT_LEFT, INPUT_PULLUP);
    io_3.pinMode(GRID_LINEAR_POT_RIGHT, INPUT_PULLUP);
    io_3.pinMode(GRID_PHOTO_SENSOR_LEFT, INPUT_PULLUP);
    io_3.pinMode(GRID_PHOTO_SENSOR_RIGHT, INPUT_PULLUP);
    io_3.pinMode(GRID_JOYSTICK_LEFT, INPUT_PULLUP);
    io_3.pinMode(GRID_JOYSTICK_RIGHT, INPUT_PULLUP);

    io_4.pinMode(GRID_TOGGLE_SWITCH_LEFT, INPUT_PULLUP);
    io_4.pinMode(GRID_TOGGLE_SWITCH_RIGHT, INPUT_PULLUP);
    io_4.pinMode(GRID_FORCE_SENSOR_LEFT, INPUT_PULLUP);
    io_4.pinMode(GRID_FORCE_SENSOR_RIGHT, INPUT_PULLUP);

    io_4.pinMode(GRID_BATTERY_NEGATIVE, INPUT_PULLUP);
    io_4.pinMode(GRID_BATTERY_POSTIVE, OUTPUT);

    select_i2c_path(SPARKFUN_SX1509_FIRST_I2C_BUS);
    io_1.pinMode(GRID_TRANSDUCER_LEFT, INPUT_PULLUP);
    io_1.pinMode(GRID_TRANSDUCER_RIGHT, INPUT_PULLUP);
    io_1.pinMode(GRID_LED_ARRAY_GREEN_LEFT, INPUT_PULLUP);
    io_1.pinMode(GRID_LED_ARRAY_GREEN_RIGHT, INPUT_PULLUP);
    io_1.pinMode(GRID_LED_ARRAY_RED_LEFT, INPUT_PULLUP);
    io_1.pinMode(GRID_LED_ARRAY_RED_RIGHT, INPUT_PULLUP);
    io_1.pinMode(GRID_MOTOR_1_LEFT, INPUT_PULLUP);
    io_1.pinMode(GRID_MOTOR_1_RIGHT, INPUT_PULLUP);

    io_2.pinMode(GRID_MOTOR_2_LEFT, INPUT_PULLUP);
    io_2.pinMode(GRID_MOTOR_2_RIGHT, INPUT_PULLUP);
    io_2.pinMode(GRID_LED_ARRAY_WHITE_LEFT, INPUT_PULLUP);
    io_2.pinMode(GRID_LED_ARRAY_WHITE_RIGHT, INPUT_PULLUP);

    select_i2c_path(SPARKFUN_SX1509_SECOND_I2C_BUS);
    io_2.pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
    io_2.pinMode(GREEN_LED_PIN, OUTPUT);
    io_2.pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
    io_2.pinMode(RED_LED_PIN, OUTPUT);
    io_1.pinMode(WHITE_BUTTON_PIN, INPUT_PULLUP);
    io_1.pinMode(WHITE_LED_PIN, OUTPUT);


    select_i2c_path(SPARKFUN_SX1509_FIRST_I2C_BUS);
    io_1.pinMode(OUTPUT_MOTOR1_PWM, OUTPUT);
    io_1.pinMode(OUTPUT_MOTOR1_IN1, OUTPUT);
    io_1.pinMode(OUTPUT_MOTOR1_IN2, OUTPUT);
    io_1.pinMode(OUTPUT_MOTOR2_PWM, OUTPUT);
    io_1.pinMode(OUTPUT_MOTOR2_IN1, OUTPUT);
    io_1.pinMode(OUTPUT_MOTOR2_IN2, OUTPUT);

    io_2.pinMode(OUTPUT_LED_ARRAY_GREEN_LEFT, OUTPUT);
    io_2.pinMode(OUTPUT_LED_ARRAY_GREEN_RIGHT, OUTPUT);
    io_2.pinMode(OUTPUT_LED_ARRAY_WHITE_LEFT, OUTPUT);
    io_2.pinMode(OUTPUT_LED_ARRAY_WHITE_RIGHT, OUTPUT);
    io_2.pinMode(OUTPUT_LED_ARRAY_RED_QUAD_1, OUTPUT);
    io_2.pinMode(OUTPUT_LED_ARRAY_RED_QUAD_2, OUTPUT);
    io_2.pinMode(OUTPUT_LED_ARRAY_RED_QUAD_3, OUTPUT);
    io_2.pinMode(OUTPUT_LED_ARRAY_RED_QUAD_4, OUTPUT);

    select_i2c_path(SPARKFUN_SX1509_FOURTH_I2C_BUS);
    io_4.digitalWrite(GRID_BATTERY_POSTIVE, LOW);

    select_i2c_path(ADAFRUIT_PWM_I2C_BUS);
    pwm.begin();
    pwm.setPWMFreq(pwm_frequency);
    Serial.println("PWM Initialized.");

    for (int pin = INDICATOR_LED_ARRAY_WHITE; pin <= INDICATOR_BATTERY_POSITIVE; pin++)
    {
        pwm.setPin(pin, 2047, FALSE);
    }
    pwm.setPWM(OUTPUT_TRANSDUCER, 0, 0);

    // Escudo Dos EL to data pins
    pinMode(RED_EL_1, OUTPUT);
    pinMode(RED_EL_2, OUTPUT);
    pinMode(GREEN_EL_1, OUTPUT);
    pinMode(GREEN_EL_2, OUTPUT);
    pinMode(YELLOW_EL, OUTPUT);
    pinMode(WHITE_EL, OUTPUT);
    pinMode(BLUE_EL_1, OUTPUT);
    pinMode(BLUE_EL_2, OUTPUT);
}

void loop()
{
    select_i2c_path(SPARKFUN_SX1509_THIRD_I2C_BUS);
    int BlueButtonPress = io_3.digitalRead(BLUE_BUTTON_PIN);
    if (BlueButtonPress == LOW)
    {
        Serial.println("Blue button depressed");
        io_3.digitalWrite(BLUE_LED_PIN, HIGH);
    }
    else
        io_3.digitalWrite(BLUE_LED_PIN, LOW);

    int YellowButtonPress = io_4.digitalRead(YELLOW_BUTTON_PIN);
    if (YellowButtonPress == LOW)
    {
        Serial.println("Yellow button depressed");
        io_4.digitalWrite(YELLOW_LED_PIN, HIGH);
    }
    else
        io_4.digitalWrite(YELLOW_LED_PIN, LOW);

    select_i2c_path(SPARKFUN_SX1509_SECOND_I2C_BUS);
    int GreenButtonPress = io_2.digitalRead(GREEN_BUTTON_PIN);
    if (GreenButtonPress == LOW)
    {
        Serial.println("Green button depressed");
        io_2.digitalWrite(GREEN_LED_PIN, HIGH);
    }
    else
        io_2.digitalWrite(GREEN_LED_PIN, LOW);

    int RedButtonPress = io_2.digitalRead(RED_BUTTON_PIN);
    if (RedButtonPress == LOW)
    {
        Serial.println("Red button depressed");
        io_2.digitalWrite(RED_LED_PIN, HIGH);
    }
    else
        io_2.digitalWrite(RED_LED_PIN, LOW);

    int WhiteButtonPress = io_1.digitalRead(WHITE_BUTTON_PIN);
    if (WhiteButtonPress == LOW)
    {
        Serial.println("White button depressed");
        io_1.digitalWrite(WHITE_LED_PIN, HIGH);
    }
    else
        io_1.digitalWrite(WHITE_LED_PIN, LOW);


    select_i2c_path(ADAFRUIT_ANALOG_FIRST_I2C_BUS);
    int16_t slider = analog_1.readADC_SingleEnded(0);
    Serial.print("SLIDER: "); Serial.println(slider);

    int16_t photo_1 = analog_1.readADC_SingleEnded(1);
    int16_t photo_2 = analog_1.readADC_SingleEnded(2);
    int16_t photo_3 = analog_1.readADC_SingleEnded(3);
    Serial.print("PHOTO1: "); Serial.println(photo_1);
    Serial.print("PHOTO2: "); Serial.println(photo_2);
    Serial.print("PHOTO3: "); Serial.println(photo_3);

    int16_t knob = analog_2.readADC_SingleEnded(0);
    Serial.print("KNOB: "); Serial.println(knob);

    int16_t touch_1 = analog_2.readADC_SingleEnded(1);
    int16_t touch_2 = analog_2.readADC_SingleEnded(2);
    int16_t touch_3 = analog_2.readADC_SingleEnded(3);
    Serial.print("TOUCH1: "); Serial.println(touch_1);
    Serial.print("TOUCH2: "); Serial.println(touch_2);
    Serial.print("TOUCH3: "); Serial.println(touch_3);
    Serial.println(" ");

    int toggle_1 = io_4.readPin(INPUT_TOGGLE_1);
    int toggle_2 = io_4.readPin(INPUT_TOGGLE_2);
    int toggle_3 = io_4.readPin(INPUT_TOGGLE_3);
    Serial.print("TOGGLE1: "); Serial.println(toggle_1);
    Serial.print("TOGGLE2: "); Serial.println(toggle_2);
    Serial.print("TOGGLE3: "); Serial.println(toggle_3);

    int joy_up = io_4.readPin(INPUT_JOYSTICK_UP);
    int joy_down = io_4.readPin(INPUT_JOYSTICK_DOWN);
    int joy_left = io_4.readPin(INPUT_JOYSTICK_LEFT);
    int joy_right = io_4.readPin(INPUT_JOYSTICK_RIGHT);
    if (joy_up == LOW)    Serial.println("Joystick UP");
    if (joy_down == LOW)  Serial.println("Joystick DOWN");
    if (joy_left == LOW)  Serial.println("Joystick LEFT");
    if (joy_right == LOW) Serial.println("Joystick RIGHT");

    select_i2c_path(SPARKFUN_SX1509_THIRD_I2C_BUS);
    if (io_3.digitalRead(GRID_ROTARY_POT_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_ROTARY_POT_LEFT triggered LOW"); }
    if (io_3.digitalRead(GRID_ROTARY_POT_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_ROTARY_POT_RIGHT triggered LOW"); }
    if (io_3.digitalRead(GRID_LINEAR_POT_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LINEAR_POT_RIGHT triggered LOW"); }
    if (io_3.digitalRead(GRID_LINEAR_POT_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LINEAR_POT_LEFT triggered LOW"); }
    if (io_3.digitalRead(GRID_PHOTO_SENSOR_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_PHOTO_SENSOR_LEFT triggered LOW"); }
    if (io_3.digitalRead(GRID_PHOTO_SENSOR_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_PHOTO_SENSOR_RIGHT triggered LOW"); }
    if (io_3.digitalRead(GRID_JOYSTICK_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_JOYSTICK_LEFT triggered LOW"); }
    if (io_3.digitalRead(GRID_JOYSTICK_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_JOYSTICK_RIGHT triggered LOW"); }
    if (io_4.digitalRead(GRID_TOGGLE_SWITCH_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_TOGGLE_SWITCH_LEFT triggered LOW"); }
    if (io_4.digitalRead(GRID_TOGGLE_SWITCH_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_TOGGLE_SWITCH_RIGHT triggered LOW"); }
    if (io_4.digitalRead(GRID_FORCE_SENSOR_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_FORCE_SENSOR_LEFT triggered LOW"); }
    if (io_4.digitalRead(GRID_FORCE_SENSOR_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_FORCE_SENSOR_RIGHT triggered LOW"); }

    select_i2c_path(SPARKFUN_SX1509_FIRST_I2C_BUS);
    if (io_1.digitalRead(GRID_TRANSDUCER_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_TRANSDUCER_LEFT triggered LOW"); }
    if (io_1.digitalRead(GRID_TRANSDUCER_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_TRANSDUCER_RIGHT triggered LOW"); }
    if (io_1.digitalRead(GRID_LED_ARRAY_GREEN_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LED_ARRAY_2_LEFT triggered LOW"); }
    if (io_1.digitalRead(GRID_LED_ARRAY_GREEN_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LED_ARRAY_2_RIGHT triggered LOW"); }
    if (io_1.digitalRead(GRID_LED_ARRAY_RED_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LED_ARRAY_3_LEFT triggered LOW"); }
    if (io_1.digitalRead(GRID_LED_ARRAY_RED_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LED_ARRAY_3_RIGHT triggered LOW"); }
    if (io_1.digitalRead(GRID_MOTOR_1_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_MOTOR_1_LEFT triggered LOW"); }
    if (io_1.digitalRead(GRID_MOTOR_1_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_MOTOR_1_RIGHT triggered LOW"); }
    if (io_2.digitalRead(GRID_MOTOR_2_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_MOTOR_2_LEFT triggered LOW"); }
    if (io_2.digitalRead(GRID_MOTOR_2_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_MOTOR_2_RIGHT triggered LOW"); }
    if (io_2.digitalRead(GRID_LED_ARRAY_WHITE_LEFT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LED_ARRAY_1_LEFT triggered LOW"); }
    if (io_2.digitalRead(GRID_LED_ARRAY_WHITE_RIGHT) == LOW) { pwm_frequency = 400; Serial.println("GRID_LED_ARRAY_1_RIGHT triggered LOW"); }

    select_i2c_path(SPARKFUN_SX1509_FOURTH_I2C_BUS);
    if (io_4.digitalRead(GRID_BATTERY_NEGATIVE) == LOW) { pwm_frequency = 400; Serial.println("GRID_BATTERY_NEGATIVE triggered LOW"); }

    select_i2c_path(SPARKFUN_SX1509_FIRST_I2C_BUS);
    io_1.analogWrite(OUTPUT_MOTOR1_PWM, 0);// knob >> 5);
    io_1.digitalWrite(OUTPUT_MOTOR1_IN1, knob < 512);
    io_1.digitalWrite(OUTPUT_MOTOR1_IN2, knob > 512);
    io_1.analogWrite(OUTPUT_MOTOR2_PWM, slider >> 5);
    io_1.digitalWrite(OUTPUT_MOTOR2_IN1, slider < 512);
    io_1.digitalWrite(OUTPUT_MOTOR2_IN2, slider > 512);

    int LED_flicker1 = (active_EL & 0x01);
    int LED_flicker2 = (active_EL & 0x02) >> 1;
    int LED_flicker3 = (active_EL & 0x04) >> 2;
    io_2.digitalWrite(OUTPUT_LED_ARRAY_WHITE_LEFT, LED_flicker1);
    io_2.digitalWrite(OUTPUT_LED_ARRAY_WHITE_RIGHT, LED_flicker1);
    io_2.digitalWrite(OUTPUT_LED_ARRAY_GREEN_LEFT, LED_flicker2);
    io_2.digitalWrite(OUTPUT_LED_ARRAY_GREEN_RIGHT, LED_flicker2);
    io_2.digitalWrite(OUTPUT_LED_ARRAY_RED_QUAD_1, LED_flicker3);
    io_2.digitalWrite(OUTPUT_LED_ARRAY_RED_QUAD_2, LED_flicker3);
    io_2.digitalWrite(OUTPUT_LED_ARRAY_RED_QUAD_3, LED_flicker3);
    io_2.digitalWrite(OUTPUT_LED_ARRAY_RED_QUAD_4, LED_flicker3);

    int tone_on = (active_EL & 0x08) >> 3 || (pwm_frequency != 2000);
    select_i2c_path(ADAFRUIT_PWM_I2C_BUS);
    pwm.setPWMFreq(pwm_frequency);
    pwm.setPWM(OUTPUT_TRANSDUCER, tone_on * 2048, tone_on * 4095);
    pwm_frequency = 2000;

    active_EL = active_EL >> 1;
    if (active_EL == 0)
        active_EL = 0x80;

    digitalWrite(RED_EL_1, active_EL & 0x01);
    digitalWrite(RED_EL_2, (active_EL >> 1) & 0x01);
    digitalWrite(GREEN_EL_1, (active_EL >> 2) & 0x01);
    digitalWrite(GREEN_EL_2, (active_EL >> 3) & 0x01);
    digitalWrite(YELLOW_EL, (active_EL >> 4) & 0x01);
    digitalWrite(WHITE_EL, (active_EL >> 5) & 0x01);
    digitalWrite(BLUE_EL_1, (active_EL >> 6) & 0x01);
    digitalWrite(BLUE_EL_2, (active_EL >> 7) & 0x01);

    delay(100);
}
