// rgb_lcd.h
// Mocked SeeedStudio Grove Kit RGB LCD 16x2 display

#ifndef __RGB_LCD_H__
#define __RGB_LCD_H__

#include <gmock/gmock.h>
#include <Print.h>

// color define 
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class rgb_lcd
{
public:
    rgb_lcd() {};

    virtual void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) = 0;
    virtual void clear() = 0;
    virtual void home() = 0;
    virtual void noDisplay() = 0;
    virtual void display() = 0;
    virtual void noBlink() = 0;
    virtual void blink() = 0;
    virtual void noCursor() = 0;
    virtual void cursor() = 0;
    virtual void scrollDisplayLeft() = 0;
    virtual void scrollDisplayRight() = 0;
    virtual void leftToRight() = 0;
    virtual void rightToLeft() = 0;
    virtual void autoscroll() = 0;
    virtual void noAutoscroll() = 0;
    virtual void createChar(uint8_t, uint8_t[]) = 0;
    virtual void setCursor(uint8_t, uint8_t) = 0;
    virtual void command(uint8_t) = 0;
    virtual void setRGB(unsigned char r, unsigned char g, unsigned char b) = 0;               // set rgb
    virtual void setColor(unsigned char color) = 0;
    virtual void setColorAll() = 0;
    virtual void setColorWhite() = 0;
    virtual void blinkLED(void) = 0;
    virtual void noBlinkLED(void) = 0;
    virtual size_t write(const char *str) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;

    //virtual size_t print(const __FlashStringHelper *) = 0;
    //virtual size_t print(const String &) = 0;
    virtual size_t print(const char[]) = 0;
    virtual size_t print(char) = 0;
    virtual size_t print(unsigned char, int = DEC) = 0;
    virtual size_t print(int, int = DEC) = 0;
    virtual size_t print(unsigned int, int = DEC) = 0;
    virtual size_t print(long, int = DEC) = 0;
    virtual size_t print(long long, int = DEC) = 0;
    virtual size_t print(unsigned long, int = DEC) = 0;
    virtual size_t print(unsigned long long, int = DEC) = 0;
    virtual size_t print(double, int = BIN) = 0;
    //virtual size_t print(const Printable&) = 0;

    //virtual size_t println(const __FlashStringHelper *) = 0;
    //virtual size_t println(const String &s) = 0;
    virtual size_t println(const char[]) = 0;
    virtual size_t println(char) = 0;
    virtual size_t println(unsigned char, int = DEC) = 0;
    virtual size_t println(int, int = DEC) = 0;
    virtual size_t println(unsigned int, int = DEC) = 0;
    virtual size_t println(long, int = DEC) = 0;
    virtual size_t println(long long, int = DEC) = 0;
    virtual size_t println(unsigned long, int = DEC) = 0;
    virtual size_t println(unsigned long long, int = DEC) = 0;
    virtual size_t println(double, int = BIN) = 0;
    //virtual size_t println(const Printable&) = 0;
    virtual size_t println(void) = 0;
};


class GroveLCDMock : public rgb_lcd
{
public:
    GroveLCDMock(void) {};
    MOCK_METHOD3(begin, void(uint8_t, uint8_t, uint8_t));
    MOCK_METHOD0(clear, void());
    MOCK_METHOD0(home, void());
    MOCK_METHOD0(noDisplay, void());
    MOCK_METHOD0(display, void());
    MOCK_METHOD0(noBlink, void());
    MOCK_METHOD0(blink, void());
    MOCK_METHOD0(noCursor, void());
    MOCK_METHOD0(cursor, void());
    MOCK_METHOD0(scrollDisplayLeft, void());
    MOCK_METHOD0(scrollDisplayRight, void());
    MOCK_METHOD0(leftToRight, void());
    MOCK_METHOD0(rightToLeft, void());
    MOCK_METHOD0(autoscroll, void());
    MOCK_METHOD0(noAutoscroll, void());
    MOCK_METHOD2(createChar, void(uint8_t, uint8_t[]));
    MOCK_METHOD2(setCursor, void(uint8_t, uint8_t));
    MOCK_METHOD1(command, void(uint8_t));
    MOCK_METHOD3(setRGB, void(unsigned char, unsigned char, unsigned char));
    MOCK_METHOD2(setPWM, void(unsigned char, unsigned char));
    MOCK_METHOD1(setColor, void(unsigned char));
    MOCK_METHOD0(setColorAll, void());
    MOCK_METHOD0(setColorWhite, void());
    MOCK_METHOD0(blinkLED, void());
    MOCK_METHOD0(noBlinkLED, void());
    MOCK_METHOD1(write, size_t(const char *));
    MOCK_METHOD2(write, size_t(const uint8_t *, size_t));

    //MOCK_METHOD1(print, size_t(const __FlashStringHelper *));
    //MOCK_METHOD1(print, size_t(const String &));
    MOCK_METHOD1(print, size_t(const char[]));
    MOCK_METHOD1(print, size_t(char));
    MOCK_METHOD2(print, size_t(unsigned char, int));
    MOCK_METHOD2(print, size_t(int, int));
    MOCK_METHOD2(print, size_t(unsigned int, int));
    MOCK_METHOD2(print, size_t(long, int));
    MOCK_METHOD2(print, size_t(long long, int));
    MOCK_METHOD2(print, size_t(unsigned long, int));
    MOCK_METHOD2(print, size_t(unsigned long long, int));
    MOCK_METHOD2(print, size_t(double, int));
    //MOCK_METHOD1(print, size_t(const Printable&));

    //MOCK_METHOD1(println, size_t(const __FlashStringHelper *));
    //MOCK_METHOD1(println, size_t(const String &s));
    MOCK_METHOD1(println, size_t(const char[]));
    MOCK_METHOD1(println, size_t(char));
    MOCK_METHOD2(println, size_t(unsigned char, int));
    MOCK_METHOD2(println, size_t(int, int));
    MOCK_METHOD2(println, size_t(unsigned int, int));
    MOCK_METHOD2(println, size_t(long, int));
    MOCK_METHOD2(println, size_t(long long, int));
    MOCK_METHOD2(println, size_t(unsigned long, int));
    MOCK_METHOD2(println, size_t(unsigned long long, int));
    MOCK_METHOD2(println, size_t(double, int));
    //MOCK_METHOD1(println, size_t(const Printable&));
    MOCK_METHOD0(println, size_t());
};

GroveLCDMock * GroveLCDMockInstance(void);
void releaseGroveLCDMock(void);

#endif
