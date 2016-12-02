// Mocked Wall hardware abstraction layer object
// Forest Park School Wall Project 
//
#ifndef _WALL_MOCK_H_
#define _WALL_MOCK_H_

#include <gmock/gmock.h>
#include "Wall.h"

class WallMock : public WallInterface
{
public:
    WallMock* usingFactory(FactoryInterface *io) { return this; }
    MOCK_METHOD0(initialize, bool(void));
    MOCK_METHOD2(turnOnLEDarray, void(led_array array, led_section section));
    MOCK_METHOD2(turnOffLEDarray, void(led_array array, led_section section));
    MOCK_METHOD3(setLEDarrayBrightness, void(led_array array, led_section section, unsigned char brightness));
    MOCK_METHOD1(setMotorDirectionClockwise, void(wall_motor motor));
    MOCK_METHOD1(setMotorDirectionCounterClockwise, void(wall_motor motor));
    MOCK_METHOD2(setMotorSpeed, void(wall_motor motor, unsigned char speed));
    MOCK_METHOD1(stopMotor, void(wall_motor motor));
    MOCK_METHOD0(turnTransducerOn, void(void));
    MOCK_METHOD0(turnTransducerOff, void(void));
    MOCK_METHOD1(turnIndicatorOn, void(indicator_led lamp));
    MOCK_METHOD1(turnIndicatorOff, void(indicator_led lamp));
    MOCK_METHOD2(setIndicatorBrightness, void(indicator_led lamp, unsigned int value));
    MOCK_METHOD1(isToggleOn, bool(toggle_switch toggle));
    MOCK_METHOD0(isJoystickUp, bool(void));
    MOCK_METHOD0(isJoystickDown, bool(void));
    MOCK_METHOD0(isJoystickLeft, bool(void));
    MOCK_METHOD0(isJoystickRight, bool(void));
    MOCK_METHOD0(getKnobPosition, unsigned int(void));
    MOCK_METHOD0(getSliderPosition, unsigned int(void));
    MOCK_METHOD1(getPhotoSensorValue, unsigned int(photo_sensor sensor));
    MOCK_METHOD1(getTouchSensorValue, unsigned int(force_sensor sensor));
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
    MOCK_METHOD3(lcdPrintAt, void(unsigned char column, unsigned char row, const char buf[]));
    MOCK_METHOD3(lcdSetBacklightColor, void(unsigned char red, unsigned char green, unsigned char blue));
    MOCK_METHOD0(clearLCDscreen, void(void));

    MOCK_METHOD1(indicatorforInput, indicator_led(input_hex hex));
    MOCK_METHOD1(indicatorForOutput, indicator_led(output_hex hex));
    MOCK_METHOD1(leftCircuitForInput, circuit_end(input_hex hex));
    MOCK_METHOD1(rightCircuitForInput, circuit_end(input_hex hex));
    MOCK_METHOD1(leftCircuitForOutput, circuit_end(output_hex hex));
    MOCK_METHOD1(rightCircuitForOutput, circuit_end(output_hex hex));
};

#endif // _WALL_MOCK_H_