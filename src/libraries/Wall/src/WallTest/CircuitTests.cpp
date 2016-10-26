// Circuit connection tests for Wall object
//
#include "Wallfixture.h"

namespace testing {

class CircuitConnectionFixture : public WallFixture,
    public ::testing::WithParamInterface<circuit_end> {
protected:
    void expectCircuitIsInput(circuit_end end);
    void expectCircuitIsOutput(circuit_end end);
};
void CircuitConnectionFixture::expectCircuitIsInput(circuit_end end)
{
    const int device = WallImplementation::circuitDevice(end);

    InSequence set_to_input;
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_PINMODE(device, WallImplementation::circuitPin(end), INPUT_PULLUP);
}
void CircuitConnectionFixture::expectCircuitIsOutput(circuit_end end)
{
    const int device = WallImplementation::circuitDevice(end);
    const int pin = WallImplementation::circuitPin(end);

    InSequence set_to_output;
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_PINMODE(device, pin, OUTPUT);
    EXPECT_SX1509_DIGITAL_WRITE(device, pin, LOW);
}

TEST_P(CircuitConnectionFixture, TestCircuitIdle)
{
    const circuit_end end = GetParam();
 
    InSequence read_connection_value;
    const int device = WallImplementation::circuitDevice(end);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_READ(device, WallImplementation::circuitPin(end), HIGH);
    ASSERT_EQ(wall->readCircuitState(end), HIGH);
}
TEST_P(CircuitConnectionFixture, TestCircuitEnergized)
{
    const circuit_end end = GetParam();

    InSequence read_connection_value;
    const int device = WallImplementation::circuitDevice(end);
    expectMultiplexerSelectsSX1509(device);
    EXPECT_SX1509_DIGITAL_READ(device, WallImplementation::circuitPin(end), LOW);
    ASSERT_EQ(wall->readCircuitState(end), LOW);
}
TEST_P(CircuitConnectionFixture, TestCircuitInput)
{
    const circuit_end end = GetParam();
    expectCircuitIsInput(end);
    wall->setCircuitAsInput(end);
}
TEST_P(CircuitConnectionFixture, TestCircuitOutput)
{
    const circuit_end end = GetParam();
    expectCircuitIsOutput(end);
    wall->setCircuitAsOutput(end);
}
TEST_F(CircuitConnectionFixture, TestCircuitsConnected)
{
    const circuit_end sink = CIRCUIT_BLUE_MOTOR_LEFT;
    const circuit_end source = CIRCUIT_NEGATIVE_POLE;

    InSequence check_connection;
    const int sink_device = WallImplementation::circuitDevice(sink);
    
    expectCircuitIsOutput(source);
    expectMultiplexerSelectsSX1509(sink_device);
    EXPECT_SX1509_DIGITAL_READ(sink_device, WallImplementation::circuitPin(sink), LOW);
    expectCircuitIsInput(source);

    ASSERT_TRUE(wall->isCircuitConnected(source, sink));
}
TEST_F(CircuitConnectionFixture, TestCircuitsNotConnected)
{
    const circuit_end sink = CIRCUIT_JOYSTICK_LEFT;
    const circuit_end source = CIRCUIT_POSITIVE_POLE;

    InSequence check_connection;
    const int sink_device = WallImplementation::circuitDevice(sink);

    expectCircuitIsOutput(source);
    expectMultiplexerSelectsSX1509(sink_device);
    EXPECT_SX1509_DIGITAL_READ(sink_device, WallImplementation::circuitPin(sink), HIGH);
    expectCircuitIsInput(source);

    ASSERT_FALSE(wall->isCircuitConnected(source, sink));
}

INSTANTIATE_TEST_CASE_P(ConnectionTests, CircuitConnectionFixture, Values(
    CIRCUIT_KNOB_LEFT,
    CIRCUIT_KNOB_RIGHT,
    CIRCUIT_SLIDER_LEFT,
    CIRCUIT_SLIDER_RIGHT,
    CIRCUIT_PHOTO_LEFT,
    CIRCUIT_PHOTO_RIGHT,
    CIRCUIT_JOYSTICK_LEFT,
    CIRCUIT_JOYSTICK_RIGHT,
    CIRCUIT_TOGGLE_LEFT,
    CIRCUIT_TOGGLE_RIGHT,
    CIRCUIT_TOUCH_LEFT,
    CIRCUIT_TOUCH_RIGHT,
    CIRCUIT_BLUE_MOTOR_LEFT,
    CIRCUIT_BLUE_MOTOR_RIGHT,
    CIRCUIT_ORANGE_MOTOR_LEFT,
    CIRCUIT_ORANGE_MOTOR_RIGHT,
    CIRCUIT_TRANSDUCER_LEFT,
    CIRCUIT_TRANSDUCER_RIGHT,
    CIRCUIT_WHITE_LED_LEFT,
    CIRCUIT_WHITE_LED_RIGHT,
    CIRCUIT_GREEN_LED_LEFT,
    CIRCUIT_GREEN_LED_RIGHT,
    CIRCUIT_RED_LED_LEFT,
    CIRCUIT_RED_LED_RIGHT,
    CIRCUIT_POSITIVE_POLE,
    CIRCUIT_NEGATIVE_POLE
    )
);

}; // namespace
