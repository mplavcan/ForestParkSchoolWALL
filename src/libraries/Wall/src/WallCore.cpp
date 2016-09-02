// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//


Wall::Wall(SX1509 *io_one)
{
    this->io_expander = io_one;
}

int Wall::set_multiplexer_i2c_bus(uint8_t bus) {
    if (bus > 7)
        return 0;
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << bus);
    return Wire.endTransmission();
}

bool Wall::Initialize()
{
    return io_expander->begin(SPARKFUN_SX1509_FIRST_I2C_ADDRESS);
}
