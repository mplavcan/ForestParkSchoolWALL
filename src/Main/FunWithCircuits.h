// Application object for Forest Park School Wall
//
#include "Wall.h"

#ifndef _FUN_WITH_CIRCUITS_H_
#define _FUN_WITH_CIRCUITS_H_

#define ONE_HUNDREDTH_SECOND 10
#define ONE_TENTH_SECOND 100
#define ONE_SECOND 1000

class FunWithCircuits {
public:
    FunWithCircuits(WallInterface *w) : wall(w) {}
    bool setup(void);
    void loop(unsigned long elapsedTime);

    void driveMotor(wall_motor motor, unsigned int value, const unsigned int midpoint);
    void driveTransducer(unsigned int value, const unsigned int midpoint);
    void driveTwoPartLED(led_array array, unsigned int value, const unsigned int midpoint);
    void driveFourPartLED(unsigned int value, const unsigned int midpoint);
    unsigned int getInputHexValue(void);
    unsigned sawtoothCycle(unsigned long time);
    input_hex findConnectedInputHex(void);
    output_hex findConnectedOutputHex(void);
    void driveOutputHex(unsigned int value);
    void collectCircuitConnections(void);
    void lightButtonIfPressed(large_button color, unsigned long currentTime);
    void lightIndicatorsForConnectedCircuits(unsigned long currentTime);
    EL_wire chooseELwireForButton(large_button color);
    void turnOffAllOutputHexes(void);
    void lightELwiresFromButtons(unsigned long currentTime);
    void turnOffAllELwires(void);


private:
    WallInterface *wall;

    input_hex energizedInput = NO_INPUT;
    output_hex energizedOutput = NO_OUTPUT;
    bool circuitComplete = false;

    unsigned long lightsOnTime = 0;
};
#endif // _FUN_WITH_CIRCUITS_H_
