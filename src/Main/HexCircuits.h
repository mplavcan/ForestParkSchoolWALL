// Constants for use in Main wall interations
#include "Wall.h"

#define NUMBER_INPUT_HEXES 6
#define NUMBER_OUTPUT_HEXES 6

const circuit_end input_hexes_left[NUMBER_INPUT_HEXES] = {
    CIRCUIT_KNOB_LEFT,
    CIRCUIT_SLIDER_LEFT,
    CIRCUIT_PHOTO_LEFT,
    CIRCUIT_JOYSTICK_LEFT,
    CIRCUIT_TOGGLE_LEFT,
    CIRCUIT_TOUCH_LEFT
};
const circuit_end input_hexes_right[NUMBER_INPUT_HEXES] = {
    CIRCUIT_KNOB_RIGHT,
    CIRCUIT_SLIDER_RIGHT,
    CIRCUIT_PHOTO_RIGHT,
    CIRCUIT_JOYSTICK_RIGHT,
    CIRCUIT_TOGGLE_RIGHT,
    CIRCUIT_TOUCH_RIGHT
};

const circuit_end output_hexes_left[NUMBER_OUTPUT_HEXES] = {
    CIRCUIT_BLUE_MOTOR_LEFT,
    CIRCUIT_ORANGE_MOTOR_LEFT,
    CIRCUIT_TRANSDUCER_LEFT,
    CIRCUIT_WHITE_LED_LEFT,
    CIRCUIT_GREEN_LED_LEFT,
    CIRCUIT_RED_LED_LEFT
};

const circuit_end output_hexes_right[NUMBER_OUTPUT_HEXES] = {
    CIRCUIT_BLUE_MOTOR_RIGHT,
    CIRCUIT_ORANGE_MOTOR_RIGHT,
    CIRCUIT_TRANSDUCER_RIGHT,
    CIRCUIT_WHITE_LED_RIGHT,
    CIRCUIT_GREEN_LED_RIGHT,
    CIRCUIT_RED_LED_RIGHT
};
