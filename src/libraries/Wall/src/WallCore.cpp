// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//
#define LOW 0
#define HIGH 1

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

const int Wall::IODeviceBus[NUMBER_OF_SX1509_DEVICES] = {
    SPARKFUN_SX1509_FIRST_I2C_BUS,
    SPARKFUN_SX1509_SECOND_I2C_BUS,
    SPARKFUN_SX1509_THIRD_I2C_BUS,
    SPARKFUN_SX1509_FOURTH_I2C_BUS
};
const int Wall::IODeviceAddress[NUMBER_OF_SX1509_DEVICES] = {
    SPARKFUN_SX1509_FIRST_I2C_ADDRESS,
    SPARKFUN_SX1509_SECOND_I2C_ADDRESS,
    SPARKFUN_SX1509_THIRD_I2C_ADDRESS,
    SPARKFUN_SX1509_FOURTH_I2C_ADDRESS
};

Wall::Wall(FactoryInterface *io) {
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        this->io_expander[device] = io->createSX1509Instance();
    this->pwm = io->createPWMinstance(ADAFRUIT_PWM_I2C_ADDRESS);
}

int Wall::setMultiplexerForIOexpander(int device) {
    return setMultiplexerI2CBus(IODeviceBus[device]);
}

int Wall::setMultiplexerI2CBus(int bus) {
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << bus);
    return Wire.endTransmission();
}


bool Wall::resetIO(int device)
{
    if (device >= NUMBER_OF_SX1509_DEVICES)
        return false;
    setMultiplexerForIOexpander(device);
    return (io_expander[device]->begin(IODeviceAddress[device]) != 0);
}

bool Wall::initialize(void)
{
    if(!initializeIOexpanders())
        return false;
    initializeLEDarrayOutputs();
    initializeMotorOutputs();
    return true;
}

bool Wall::initializeIOexpanders(void)
{
    bool result = true;
    for(int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        result = result && resetIO(device);
    return result;
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
    pwm->setPWMFreq(PWM_FREQUENCY);
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
        case LOWER_LEFT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_1;
        case LOWER_RIGHT_SIDE: return OUTPUT_LED_ARRAY_RED_QUAD_2;
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


