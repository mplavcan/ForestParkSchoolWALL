// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//


Wall::Wall(SX1509* io_expanders[4])
{
    for(int x=0;x<4;x++)
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
    int index = device - 1;
    if (device > 4)
        return false;
    set_multiplexer_i2c_bus(IODeviceBus[index]);
    return io_expander[index]->begin(IODeviceAddress[index]);
}

bool Wall::Initialize()
{
    bool result = true;
    for(int x=1;x<=4;x++)
        result = result && resetIO(x);
    return result;
}



void Wall::ChangeLEDState(int led_selector, int led_state)
{
    const int io_device = 1;
    set_multiplexer_i2c_bus(IODeviceBus[io_device]);
    io_expander[io_device]->digitalWrite(led_selector, led_state);
}
