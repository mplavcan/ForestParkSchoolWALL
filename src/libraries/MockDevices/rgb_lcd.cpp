// rgb_lcd.h
// Mocked Seeed GroveKit RGB LCD 16x2 display
#include "rgb_lcd.h"

static GroveLCDMock* p_GroveLCDMock = NULL;
GroveLCDMock* GroveLCDMockInstance(void)
{
    if (!p_GroveLCDMock) {
        p_GroveLCDMock = new GroveLCDMock();
    }
    return p_GroveLCDMock;
}

void releaseGroveLCDMock(void)
{
    if (p_GroveLCDMock) {
        delete p_GroveLCDMock;
        p_GroveLCDMock = NULL;
    }
}
