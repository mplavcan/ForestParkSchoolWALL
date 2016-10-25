Wall_IO
=======

Wall Input/Output Object

Wall Interface methods:
-------------------------
```C++
    // Create Wall interface
    Wall(FactoryInterface *io);
    // Write all devices to starting state
    bool initialize(void);

    // LED Arrays (GREEN_LED, WHITE_LED, RED_LED)
    //   by section (LEFT_SIDE, RIGHT_SIDE, LOWER_LEFT_SIDE, LOWER_RIGHT_SIDE)
    void turnOnLEDarray(led_array array, led_section section);
    void turnOffLEDarray(led_array array, led_section section);

    // Motors (BLUE_MOTOR, ORANGE_MOTOR)
    void setMotorDirectionClockwise(wall_motor motor);
    void setMotorDirectionCounterClockwise(wall_motor motor);
    void setMotorSpeed(wall_motor motor, uint8_t speed);
    void stopMotor(wall_motor motor);

    // Buzzer
    void turnTransducerOn(void);
    void turnTransducerOff(void);

    // Toggle Switches (LEFT_TOGGLE, CENTER_TOGGLE, RIGHT_TOGGLE)
    bool isToggleOn(toggle_switch toggle);

    // Joystick
    bool isJoystickUp(void);
    bool isJoystickDown(void);
    bool isJoystickLeft(void);
    bool isJoystickRight(void);

    // Silver Knob
    uint16_t getKnobPosition(void);

    // Arrow Slider
    uint16_t getSliderPosition(void);

    // Photo Sensors (LEFT_PHOTO, CENTER_PHOTO, RIGHT_PHOTO)
    uint16_t getPhotoSensorValue(photo_sensor sensor);

    // Touch Sensors (LEFT_TOUCH, BOTTOM_TOUCH, RIGHT_TOUCH)
    uint16_t getTouchSensorValue(force_sensor sensor);
  
    // Input/Output Indicator LEDs (per input/output)
    //
    void turnIndicatorOn(indicator_led lamp);
    void turnIndicatorOff(indicator_led lamp);
    void setIndicatorBrightness(indicator_led lamp, uint16_t value);

    // Hex circuit points (per input/output)
    int readCircuitState(circuit_end end);
    void resetCircuitInputs(void);
    bool isCircuitConnected(circuit_end A, circuit_end B);
    static indicator_led indicatorForCircuit(circuit_end point);

    // Large Color Buttons
    //  (BLUE_BUTTON, YELLOW_BUTTON, GREEN_BUTTON, RED_BUTTON, WHITE_BUTTON)
    bool isButtonDepressed(large_button button);
    void illuminateButton(large_button button);
    void extinguishButton(large_button button);

    // Electroluminescent wires
    void illuminateELWire(EL_wire line);
    void extinguishELWire(EL_wire line);

    // LCD Display
    void lcdPrintAt(uint8_t column, uint8_t row, const char buf[]);
    void lcdSetBacklightColor(uint8_t red, uint8_t green, uint8_t blue);
    void clearLCDscreen(void);

```

