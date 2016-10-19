// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//
#ifndef LOW
#define LOW 0
#endif
#ifndef HIGH
#define HIGH 1
#endif

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

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


Wall::Wall(FactoryInterface *io) {
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        this->io_expander[device] = io->createSX1509Instance();
    for (int device = 0; device < NUMBER_OF_ADS1015_DEVICES; device++)
        this->analog_expander[device] = io->createADS1015Instance(analogDeviceAddress[device]);
    this->pwm = io->createPWMinstance(ADAFRUIT_PWM_I2C_ADDRESS);
}

int Wall::setMultiplexerForIOexpander(int device) {
    return setMultiplexerI2CBus(ioDeviceBus[device]);
}

int Wall::setMultiplexerForAnalog(int device) {
    return setMultiplexerI2CBus(analogDeviceBus[device]);
}

int Wall::setMultiplexerI2CBus(int bus) {
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
    if(!initializeIOexpanders())
        return false;
    initializeAnalogExpanders();
    initializeLEDarrayOutputs();
    initializeMotorOutputs();
    initializePWMOutputs();
    initializeToggleInputs();
    initializeJoystickInputs();
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
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_GREEN_LEFT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_GREEN_RIGHT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_WHITE_LEFT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_WHITE_RIGHT, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_1, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_2, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_3, OUTPUT);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->pinMode(OUTPUT_LED_ARRAY_RED_QUAD_4, OUTPUT);
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
    turnIndicatorOff(INDICATE_PRESSURE_SENSOR);
    turnIndicatorOff(INDICATE_POSITIVE_POLE);
    turnIndicatorOff(INDICATE_NEGATIVE_POLE);
    turnTransducerOff();
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

bool Wall::ledArrayIsActiveLow(led_array array)
{
    return (array == RED_LED);
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
        case LOWER_LEFT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_3;
        case LOWER_RIGHT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_4;
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

void Wall::turnOnLEDarray(led_array array, led_section section)
{
    int pinValue = ledArrayIsActiveLow(array) ? LOW : HIGH;
    setMultiplexerForIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->digitalWrite(
        Wall::ledArrayPin(array, section), pinValue);
}
void Wall::turnOffLEDarray(led_array array, led_section section)
{
    int pinValue = ledArrayIsActiveLow(array) ? HIGH : LOW;
    setMultiplexerForIOexpander(OUTPUT_LED_ARRAY_I2C_DEVICE);
    io_expander[OUTPUT_LED_ARRAY_I2C_DEVICE]->digitalWrite(
        Wall::ledArrayPin(array, section), pinValue);
}

int Wall::motorControlPin1(wall_motor motor)
{
    return (motor = BLUE_MOTOR) ? OUTPUT_MOTOR1_IN1 : OUTPUT_MOTOR2_IN1;
}
int Wall::motorControlPin2(wall_motor motor)
{
    return (motor = BLUE_MOTOR) ? OUTPUT_MOTOR1_IN2 : OUTPUT_MOTOR2_IN2;
}
int Wall::motorPWMpin(wall_motor motor)
{
    return (motor = BLUE_MOTOR) ? OUTPUT_MOTOR1_PWM : OUTPUT_MOTOR2_PWM;
}

// Motor Truth Table (from TB6612 documentation):
//
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
void Wall::setMotorSpeed(wall_motor motor, uint8_t speed)
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

void Wall::turnIndicatorOn(indicator_led lamp)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(lamp, PWM_INDICATOR_ON_VALUE, FALSE);
}
void Wall::turnIndicatorOff(indicator_led lamp)
{
    setMultiplexerI2CBus(ADAFRUIT_PWM_I2C_BUS);
    pwm->setPin(lamp, PWM_INDICATOR_OFF_VALUE, FALSE);
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

uint16_t Wall::getKnobPosition(void)
{
    setMultiplexerForAnalog(INPUT_ROTARY_POT_I2C_DEVICE);
    return analog_expander[INPUT_ROTARY_POT_I2C_DEVICE]->readADC_SingleEnded(INPUT_ROTARY_POT);
}

uint16_t Wall::getSliderPosition(void)
{
    setMultiplexerForAnalog(INPUT_LINEAR_POT_I2C_DEVICE);
    return analog_expander[INPUT_LINEAR_POT_I2C_DEVICE]->readADC_SingleEnded(INPUT_LINEAR_POT);
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

uint16_t Wall::getPhotoSensorValue(photo_sensor sensor)
{
    setMultiplexerForAnalog(INPUT_PHOTO_SENSOR_I2C_DEVICE);
    return analog_expander[INPUT_PHOTO_SENSOR_I2C_DEVICE]->readADC_SingleEnded(photoSensorPin(sensor));
}

int Wall::forceSensorPin(force_sensor sensor)
{
    switch (sensor)
    {
        case LEFT_PRESSURE:   return INPUT_FORCE_SENSOR_1;
        case BOTTOM_PRESSURE: return INPUT_FORCE_SENSOR_2;
        case RIGHT_PRESSURE:  return INPUT_FORCE_SENSOR_3;
        default: return 0;
    }
}

uint16_t Wall::getTouchSensorValue(force_sensor sensor)
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
        case CIRCUIT_BLUE_MOTOR_LEFT: return GRID_MOTOR_1_I2C_DEVICE;
        case CIRCUIT_BLUE_MOTOR_RIGHT: return GRID_MOTOR_1_I2C_DEVICE;
        case CIRCUIT_ORANGE_MOTOR_LEFT: return GRID_MOTOR_2_I2C_DEVICE;
        case CIRCUIT_ORANGE_MOTOR_RIGHT: return GRID_MOTOR_2_I2C_DEVICE;
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
        case CIRCUIT_BLUE_MOTOR_LEFT: return GRID_MOTOR_1_LEFT;
        case CIRCUIT_BLUE_MOTOR_RIGHT: return GRID_MOTOR_1_RIGHT;
        case CIRCUIT_ORANGE_MOTOR_LEFT: return GRID_MOTOR_2_LEFT;
        case CIRCUIT_ORANGE_MOTOR_RIGHT: return GRID_MOTOR_2_RIGHT;
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
    int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    return io_expander[device]->digitalRead(circuitPin(end));
}

void Wall::setCircuitAsInput(circuit_end end)
{
    int device = circuitDevice(end);
    setMultiplexerForIOexpander(device);
    io_expander[device]->pinMode(circuitPin(end), INPUT_PULLUP);
}

void Wall::setCircuitAsOutput(circuit_end end)
{
    int device = circuitDevice(end);
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
    int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    return (io_expander[device]->digitalRead(buttonPin(button)) == LOW);
}
void Wall::illuminateButton(large_button button)
{
    int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    io_expander[device]->digitalWrite(buttonLEDpin(button), HIGH);
}
void Wall::extinguishButton(large_button button)
{
    int device = buttonDevice(button);
    setMultiplexerForIOexpander(device);
    io_expander[device]->digitalWrite(buttonLEDpin(button), LOW);
}


