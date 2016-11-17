// Mocked Wall hardware abstraction layer object
// Forest Park School Wall Project 
//
#ifndef _WALL_MOCK_H_
#define _WALL_MOCK_H_

#include <gmock/gmock.h>
#include "Wall.h"

class WallMock : public Wall
{
public:
    WallMock* usingFactory(FactoryInterface *io) { return this; }
    MOCK_METHOD0(initialize, bool(void));
    MOCK_METHOD2(turnOnLEDarray, void(led_array array, led_section section));
    MOCK_METHOD2(turnOffLEDarray, void(led_array array, led_section section));
    MOCK_METHOD3(setLEDarrayBrightness, void(led_array array, led_section section, unsigned char brightness));
    MOCK_METHOD1(setMotorDirectionClockwise, void(wall_motor motor));
    MOCK_METHOD1(setMotorDirectionCounterClockwise, void(wall_motor motor));
    MOCK_METHOD2(setMotorSpeed, void(wall_motor motor, uint8_t speed));
    MOCK_METHOD1(stopMotor, void(wall_motor motor));
    MOCK_METHOD0(turnTransducerOn, void(void));
    MOCK_METHOD0(turnTransducerOff, void(void));
    MOCK_METHOD1(turnIndicatorOn, void(indicator_led lamp));
    MOCK_METHOD1(turnIndicatorOff, void(indicator_led lamp));
    MOCK_METHOD2(setIndicatorBrightness, void(indicator_led lamp, uint16_t value));
    MOCK_METHOD1(isToggleOn, bool(toggle_switch toggle));
    MOCK_METHOD0(isJoystickUp, bool(void));
    MOCK_METHOD0(isJoystickDown, bool(void));
    MOCK_METHOD0(isJoystickLeft, bool(void));
    MOCK_METHOD0(isJoystickRight, bool(void));
    MOCK_METHOD0(getKnobPosition, uint16_t(void));
    MOCK_METHOD0(getSliderPosition, uint16_t(void));
    MOCK_METHOD1(getPhotoSensorValue, uint16_t(photo_sensor sensor));
    MOCK_METHOD1(getTouchSensorValue, uint16_t(force_sensor sensor));
    MOCK_METHOD1(readCircuitState, int(circuit_end end));
    MOCK_METHOD0(resetCircuitInputs, void(void));
    MOCK_METHOD1(setCircuitAsInput, void(circuit_end end));
    MOCK_METHOD1(setCircuitAsOutput, void(circuit_end end));
    MOCK_METHOD2(isCircuitConnected, bool(circuit_end A, circuit_end B));
    MOCK_METHOD1(isButtonDepressed, bool(large_button button));
    MOCK_METHOD1(illuminateButton, void(large_button button));
    MOCK_METHOD1(extinguishButton, void(large_button button));
    MOCK_METHOD1(illuminateELWire, void(EL_wire line));
    MOCK_METHOD1(extinguishELWire, void(EL_wire line));
    MOCK_METHOD3(lcdPrintAt, void(uint8_t column, uint8_t row, const char buf[]));
    MOCK_METHOD3(lcdSetBacklightColor, void(uint8_t red, uint8_t green, uint8_t blue));
    MOCK_METHOD0(clearLCDscreen, void(void));

    MOCK_METHOD1(indicatorforInput, indicator_led(input_hex hex));
    MOCK_METHOD1(indicatorForOutput, indicator_led(output_hex hex));
    MOCK_METHOD1(leftCircuitForInput, circuit_end(input_hex hex));
    MOCK_METHOD1(rightCircuitForInput, circuit_end(input_hex hex));
    MOCK_METHOD1(leftCircuitForOutput, circuit_end(output_hex hex));
    MOCK_METHOD1(rightCircuitForOutput, circuit_end(output_hex hex));
};

#endif // _WALL_MOCK_H_