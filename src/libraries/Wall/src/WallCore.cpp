// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//


Wall::Wall(SX1509* io_expanders[4])
{
    for(int x=0;x<NUMBER_OF_SX1509_DEVICES;x++)
      this->io_expander[x] = io_expanders[x];
}

int Wall::set_multiplexer_i2c_bus(uint8_t bus) {
    if (bus > 7)
        return 0;
    Wire.beginTransmission(ADAFRUIT_MULTIPLEXER_I2C_ADDRESS);
    Wire.write(1 << bus);
    return Wire.endTransmission();
}



bool Wall::resetIO(int device)
{
    int bus;
    if (device >= NUMBER_OF_SX1509_DEVICES)
        return false;
    set_multiplexer_i2c_bus(IODeviceBus[device]);
    return io_expander[device]->begin(IODeviceAddress[device]);
}

bool Wall::Initialize()
{
    bool result = true;
    for(int x=0;x< NUMBER_OF_SX1509_DEVICES;x++)
        result = result && resetIO(x);
    return result;
}



void Wall::ChangeLEDState(int led_selector, int led_state)
{
    const int io_device = 1;
    set_multiplexer_i2c_bus(IODeviceBus[io_device]);
    io_expander[io_device]->digitalWrite(led_selector, led_state);
}
