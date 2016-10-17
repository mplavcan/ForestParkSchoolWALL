// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//

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

int Wall::setMultiplexerI2Cbus(int device) {
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << IODeviceBus[device]);
    return Wire.endTransmission();
}

bool Wall::resetIO(int device)
{
    if (device >= NUMBER_OF_SX1509_DEVICES)
        return false;
    setMultiplexerI2Cbus(device);
    return io_expander[device]->begin(IODeviceAddress[device]);
}

bool Wall::initialize()
{
    bool result = true;
    for(int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        result = result && resetIO(device);
    return result;
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
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_LED_ARRAYS);
    io_expander[IO_EXPANDER_FOR_LED_ARRAYS]->digitalWrite(
        Wall::ledArrayPin(array, section), pinValue);
}

void Wall::turnOffLEDarray(led_array array, led_section section)
{
    int pinValue = ledArrayIsActiveLow(array) ? HIGH : LOW;
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_LED_ARRAYS);
    io_expander[IO_EXPANDER_FOR_LED_ARRAYS]->digitalWrite(
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
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_MOTORS);
    io_expander[IO_EXPANDER_FOR_MOTORS]->digitalWrite(motorControlPin1(motor), HIGH);
    io_expander[IO_EXPANDER_FOR_MOTORS]->digitalWrite(motorControlPin2(motor), LOW);
}
void Wall::setMotorDirectionCounterClockwise(wall_motor motor)
{
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_MOTORS);
    io_expander[IO_EXPANDER_FOR_MOTORS]->digitalWrite(motorControlPin1(motor), LOW);
    io_expander[IO_EXPANDER_FOR_MOTORS]->digitalWrite(motorControlPin2(motor), HIGH);
}
void Wall::stopMotor(wall_motor motor)
{
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_MOTORS);
    io_expander[IO_EXPANDER_FOR_MOTORS]->digitalWrite(motorControlPin1(motor), LOW);
    io_expander[IO_EXPANDER_FOR_MOTORS]->digitalWrite(motorControlPin2(motor), LOW);
}
void Wall::setMotorSpeed(wall_motor motor, uint8_t speed)
{
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_MOTORS);
    io_expander[IO_EXPANDER_FOR_MOTORS]->analogWrite(motorPWMpin(motor), speed);
}

// Sound methods
void Wall::turnOnTransducer(void)
{
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_MOTORS);
    pwm->setPWMFreq(2000.0);
    pwm->setPWM(OUTPUT_TRANSDUCER, 2048, 4095);
}

