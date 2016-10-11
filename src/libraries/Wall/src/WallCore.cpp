// Wall hardware abstraction layer object
// Forest Park School Wall Project 
//

Wall::Wall(SX1509* io_one, SX1509* io_two, SX1509* io_three, SX1509* io_four)
{
    this->io_expander1 = io_one;
    this->io_expander2 = io_two;
    this->io_expander3 = io_three;
    this->io_expander4 = io_four;
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
    set_multiplexer_i2c_bus(IODeviceBus[index]);
    
    if (device == 1)
       return io_expander1->begin(IODeviceAddress[index]);
    if (device == 2)
        return io_expander2->begin(IODeviceAddress[index]);
    if (device == 3)
        return io_expander3->begin(IODeviceAddress[index]);
    if (device == 4)
        return io_expander4->begin(IODeviceAddress[index]);
    return false;
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
    set_multiplexer_i2c_bus(SPARKFUN_SX1509_SECOND_I2C_BUS);
    io_expander2->digitalWrite(led_selector, led_state);
}
