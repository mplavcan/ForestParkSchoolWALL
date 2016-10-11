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
Wall::Wall(SX1509* io_expanders[4])
{
    for (int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        this->io_expander[device] = io_expanders[device];
}

int Wall::setMultiplexerI2Cbus(uint8_t bus) {
    if (bus > 7)
        return 0;
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << bus);
    return Wire.endTransmission();
}

bool Wall::resetIO(int device)
{
    if (device >= NUMBER_OF_SX1509_DEVICES)
        return false;
    setMultiplexerI2Cbus(IODeviceBus[device]);
    return io_expander[device]->begin(IODeviceAddress[device]);
}

bool Wall::Initialize()
{
    bool result = true;
    for(int device = 0; device < NUMBER_OF_SX1509_DEVICES; device++)
        result = result && resetIO(device);
    return result;
}



void Wall::ChangeLEDState(int ledSelector, int ledState)
{
    const int LEDDevice = 1;
    setMultiplexerI2Cbus(IODeviceBus[LEDDevice]);
    io_expander[LEDDevice]->digitalWrite(ledSelector, ledState);
}
