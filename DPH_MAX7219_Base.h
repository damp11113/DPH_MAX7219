// MAX7219_Base.h
#ifndef MAX7219_BASE_H
#define MAX7219_BASE_H

#include <Arduino.h>

// MAX7219 Register addresses
#define MAX7219_REG_NOOP        0x00
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DIGIT1      0x02
#define MAX7219_REG_DIGIT2      0x03
#define MAX7219_REG_DIGIT3      0x04
#define MAX7219_REG_DIGIT4      0x05
#define MAX7219_REG_DIGIT5      0x06
#define MAX7219_REG_DIGIT6      0x07
#define MAX7219_REG_DIGIT7      0x08
#define MAX7219_REG_DECODEMODE  0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

// Base class for MAX7219
class MAX7219_Base {
protected:
    int dataPin;
    int clkPin;
    int csPin;
    int numDevices;
    
    void sendByte(byte data);
    void writeRegister(int device, byte reg, byte data);
    
public:
    MAX7219_Base(int data, int clk, int cs, int devices = 1);
    virtual ~MAX7219_Base();
    
    virtual void begin() = 0;
    void setIntensity(int intensity);
    void setIntensityDevice(int device, int intensity);
    void shutdown(bool b);
    void shutdownDevice(int device, bool b);
    void displayTest(bool b);
};

#endif