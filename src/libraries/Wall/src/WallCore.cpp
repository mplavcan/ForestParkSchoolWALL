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

void Wall::turnOnLEDarray(int ledSelector)
{
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_LED_ARRAYS);
    io_expander[IO_EXPANDER_FOR_LED_ARRAYS]->digitalWrite(ledSelector, HIGH);
}

void Wall::turnOffLEDarray(int ledSelector)
{
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_LED_ARRAYS);
    io_expander[IO_EXPANDER_FOR_LED_ARRAYS]->digitalWrite(ledSelector, LOW);
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
void Wall::setMotorSpeed(wall_motor motor, int speed)
{
    setMultiplexerI2Cbus(IO_EXPANDER_FOR_MOTORS);
    io_expander[IO_EXPANDER_FOR_MOTORS]->analogWrite(motorPWMpin(motor), speed);
}
