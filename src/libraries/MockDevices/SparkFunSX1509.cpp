// SparkFunSX1509.h
// Mocked SparkFun SX1509 I/O Expander Library

#include "SparkFunSX1509.h"


SX1509Mock* SX1509MockInstance()
{
    return new SX1509Mock();
}

void releaseSX1509Mock(SX1509Mock *mock)
{
    delete mock;
}
