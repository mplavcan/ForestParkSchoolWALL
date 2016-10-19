//  Forest Park Circuit Wall
//  Definitions of interface pins

// All I2C device addresse used by the Wall
#define SPARKFUN_SX1509_FIRST_I2C_ADDRESS    0x3E
#define SPARKFUN_SX1509_SECOND_I2C_ADDRESS   0x3F
#define SPARKFUN_SX1509_THIRD_I2C_ADDRESS    0x70
#define SPARKFUN_SX1509_FOURTH_I2C_ADDRESS   0x71
#define GROVE_RGB_I2C_ADDRESS                0x62 // Driven by Grove library, here only for documentation
#define GROVE_LCD_I2C_ADDRESS                0x3E // Driven by Grove library, here only for documentation
#define ADAFRUIT_PWM_I2C_ADDRESS             0x40 // This board also occupies 0xE0, 0xE2,0xE4,0xE8,0x70, but they are not used
#define ADAFRUIT_ANALOG_FIRST_I2C_ADDRESS    0x48
#define ADAFRUIT_ANALOG_SECOND_I2C_ADDRESS   0x49
#define ADAFRUIT_MULTIPLEXER_I2C_ADDRESS     0x72

// Device order indexes 
#define NUMBER_OF_SX1509_DEVICES 4
#define FIRST_SX1509_INDEX    0x0
#define SECOND_SX1509_INDEX   0x1
#define THIRD_SX1509_INDEX    0x2
#define FOURTH_SX1509_INDEX   0x3

#define NUMBER_OF_ADS1015_DEVICES 2
#define FIRST_ADS1015_INDEX    0x0
#define SECOND_ADS1015_INDEX   0x1

// Multiplexer pathways (to avoid address conflicts)
#define SPARKFUN_SX1509_FIRST_I2C_BUS        1
#define SPARKFUN_SX1509_SECOND_I2C_BUS       1
#define SPARKFUN_SX1509_THIRD_I2C_BUS        0
#define SPARKFUN_SX1509_FOURTH_I2C_BUS       0
#define GROVE_I2C_BUS                        0
#define ADAFRUIT_PWM_I2C_BUS                 1
#define ADAFRUIT_ANALOG_FIRST_I2C_BUS        0 
#define ADAFRUIT_ANALOG_SECOND_I2C_BUS       0

// El Escudo Dos Electroluminescent wires
// (Arduino pins D2-D9)
#define RED_EL_1   2
#define RED_EL_2   3
#define GREEN_EL_1 4
#define GREEN_EL_2 5
#define YELLOW_EL  6
#define WHITE_EL   7
#define BLUE_EL_1  8
#define BLUE_EL_2  9

// Large LED buttons
#define BLUE_BUTTON_I2C_DEVICE         THIRD_SX1509_INDEX 
#define BLUE_LED_I2C_DEVICE            THIRD_SX1509_INDEX
#define BLUE_BUTTON_PIN                 8
#define BLUE_LED_PIN                    9
#define YELLOW_BUTTON_I2C_DEVICE       FOURTH_SX1509_INDEX
#define YELLOW_LED_I2C_DEVICE          FOURTH_SX1509_INDEX
#define YELLOW_BUTTON_PIN              10
#define YELLOW_LED_PIN                 11
#define GREEN_BUTTON_I2C_DEVICE        SECOND_SX1509_INDEX
#define GREEN_LED_I2C_DEVICE           SECOND_SX1509_INDEX
#define GREEN_BUTTON_PIN                7
#define GREEN_LED_PIN                   6
#define RED_BUTTON_I2C_DEVICE          SECOND_SX1509_INDEX
#define RED_LED_I2C_DEVICE             SECOND_SX1509_INDEX
#define RED_BUTTON_PIN                  5
#define RED_LED_PIN                     4
#define WHITE_BUTTON_I2C_DEVICE        FIRST_SX1509_INDEX
#define WHITE_LED_I2C_DEVICE           FIRST_SX1509_INDEX
#define WHITE_BUTTON_PIN               15
#define WHITE_LED_PIN                  14

// LED Indicators for input and output hexes
#define ADAFRUIT_PWM_NUM_OUTPUTS       16
#define INDICATOR_LED_ARRAY_WHITE       0
#define INDICATOR_MOTOR_2               1
#define INDICATOR_LED_ARRAY_RED         2
#define INDICATOR_MOTOR_1               3
#define INDICATOR_TRANSDUCER            4
#define INDICATOR_LED_ARRAY_GREEN       5
#define INDICATOR_ROTARY_POT            8
#define INDICATOR_LINEAR_POT            9
#define INDICATOR_PHOTO_SENSOR         10
#define INDICATOR_JOYSTICK             11
#define INDICATOR_TOGGLE_SWITCH        12
#define INDICATOR_FORCE_SENSOR         13
#define INDICATOR_BATTERY_NEGATIVE      6
#define INDICATOR_BATTERY_POSITIVE     14
#define OUTPUT_TRANSDUCER              15

// Digital Inputs
#define INPUT_TOGGLE_I2C_DEVICE        FOURTH_SX1509_INDEX
#define INPUT_TOGGLE_1                  0 
#define INPUT_TOGGLE_2                  1
#define INPUT_TOGGLE_3                  2

#define INPUT_JOYSTICK_I2C_DEVICE      FOURTH_SX1509_INDEX
#define INPUT_JOYSTICK_UP               5
#define INPUT_JOYSTICK_RIGHT            4
#define INPUT_JOYSTICK_LEFT             6
#define INPUT_JOYSTICK_DOWN             7

// Analog Inputs
#define INPUT_ROTARY_POT_I2C_DEVICE    FIRST_ADS1015_INDEX
#define INPUT_ROTARY_POT                0

#define INPUT_PHOTO_SENSOR_I2C_DEVICE  FIRST_ADS1015_INDEX
#define INPUT_PHOTO_SENSOR_1            1
#define INPUT_PHOTO_SENSOR_2            2
#define INPUT_PHOTO_SENSOR_3            3

#define INPUT_LINEAR_POT_I2C_DEVICE    SECOND_ADS1015_INDEX
#define INPUT_LINEAR_POT                0

#define INPUT_FORCE_SENSOR_I2C_DEVICE  SECOND_ADS1015_INDEX
#define INPUT_FORCE_SENSOR_1            1 
#define INPUT_FORCE_SENSOR_2            2
#define INPUT_FORCE_SENSOR_3            3

// Motor controller outputs
#define OUTPUT_MOTOR_I2C_DEVICE        FIRST_SX1509_INDEX
#define OUTPUT_MOTOR1_PWM               8  
#define OUTPUT_MOTOR1_IN1              10 
#define OUTPUT_MOTOR1_IN2               9
#define OUTPUT_MOTOR2_PWM              13
#define OUTPUT_MOTOR2_IN1              11
#define OUTPUT_MOTOR2_IN2              12

// LED array output array outputs
#define OUTPUT_LED_ARRAY_I2C_DEVICE    SECOND_SX1509_INDEX
#define OUTPUT_LED_ARRAY_GREEN_LEFT     0
#define OUTPUT_LED_ARRAY_GREEN_RIGHT    1
#define OUTPUT_LED_ARRAY_WHITE_LEFT     2
#define OUTPUT_LED_ARRAY_WHITE_RIGHT    3
#define OUTPUT_LED_ARRAY_RED_QUAD_1     8
#define OUTPUT_LED_ARRAY_RED_QUAD_2     9
#define OUTPUT_LED_ARRAY_RED_QUAD_3    10
#define OUTPUT_LED_ARRAY_RED_QUAD_4    11

// Hex grid input/output nets
#define GRID_ROTARY_POT_I2C_DEVICE     THIRD_SX1509_INDEX
#define GRID_LINEAR_POT_I2C_DEVICE     THIRD_SX1509_INDEX
#define GRID_PHOTO_SENSOR_I2C_DEVICE   THIRD_SX1509_INDEX
#define GRID_JOYSTICK_I2C_DEVICE       THIRD_SX1509_INDEX
#define GRID_TOGGLE_SWITCH_I2C_DEVICE  FOURTH_SX1509_INDEX
#define GRID_FORCE_SENSOR_I2C_DEVICE   FOURTH_SX1509_INDEX
#define GRID_ROTARY_POT_LEFT           15
#define GRID_ROTARY_POT_RIGHT           0
#define GRID_LINEAR_POT_LEFT           14
#define GRID_LINEAR_POT_RIGHT           1
#define GRID_PHOTO_SENSOR_LEFT         13
#define GRID_PHOTO_SENSOR_RIGHT         2
#define GRID_JOYSTICK_LEFT             12
#define GRID_JOYSTICK_RIGHT             3
#define GRID_TOGGLE_SWITCH_LEFT        15
#define GRID_TOGGLE_SWITCH_RIGHT       13
#define GRID_FORCE_SENSOR_LEFT         14
#define GRID_FORCE_SENSOR_RIGHT        12

#define GRID_BATTERY_I2C_DEVICE        FOURTH_SX1509_INDEX
#define GRID_BATTERY_NEGATIVE           8   
#define GRID_BATTERY_POSTIVE            9

#define GRID_TRANSDUCER_I2C_DEVICE       FIRST_SX1509_INDEX
#define GRID_LED_ARRAY_GREEN_I2C_DEVICE  FIRST_SX1509_INDEX
#define GRID_LED_ARRAY_RED_I2C_DEVICE    FIRST_SX1509_INDEX
#define GRID_MOTOR_1_I2C_DEVICE          FIRST_SX1509_INDEX
#define GRID_MOTOR_2_I2C_DEVICE          SECOND_SX1509_INDEX
#define GRID_LED_ARRAY_WHITE_I2C_DEVICE  SECOND_SX1509_INDEX
#define GRID_TRANSDUCER_LEFT              2
#define GRID_TRANSDUCER_RIGHT             1
#define GRID_LED_ARRAY_GREEN_LEFT         3
#define GRID_LED_ARRAY_GREEN_RIGHT        0
#define GRID_LED_ARRAY_RED_LEFT           4
#define GRID_LED_ARRAY_RED_RIGHT          5
#define GRID_MOTOR_1_LEFT                 7
#define GRID_MOTOR_1_RIGHT                6
#define GRID_MOTOR_2_LEFT                15
#define GRID_MOTOR_2_RIGHT               14
#define GRID_LED_ARRAY_WHITE_LEFT        13
#define GRID_LED_ARRAY_WHITE_RIGHT       12
