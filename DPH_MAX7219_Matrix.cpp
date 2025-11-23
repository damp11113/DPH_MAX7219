#include "DPH_MAX7219_Matrix.h"

MAX7219_LED_MATRIX::MAX7219_LED_MATRIX(int data, int clk, int cs, int devices) 
    : MAX7219_Base(data, clk, cs, devices) {
    buffer = new byte[numDevices * 8];
}

MAX7219_LED_MATRIX::~MAX7219_LED_MATRIX() {
    delete[] buffer;
}

void MAX7219_LED_MATRIX::begin() {
    pinMode(dataPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    
    for(int i = 0; i < numDevices; i++) {
        writeRegister(i, MAX7219_REG_SCANLIMIT, 0x07);
        writeRegister(i, MAX7219_REG_DECODEMODE, 0x00);  // No decode for LED matrix
        writeRegister(i, MAX7219_REG_DISPLAYTEST, 0x00);
        writeRegister(i, MAX7219_REG_INTENSITY, 0x08);
        writeRegister(i, MAX7219_REG_SHUTDOWN, 0x01);
    }
    clear();
}

void MAX7219_LED_MATRIX::clear() {
    for(int i = 0; i < numDevices * 8; i++) {
        buffer[i] = 0;
    }
    for(int device = 0; device < numDevices; device++) {
        for(int row = 0; row < 8; row++) {
            writeRegister(device, MAX7219_REG_DIGIT0 + row, 0x00);
        }
    }
}

void MAX7219_LED_MATRIX::clearDevice(int device) {
    if(device >= numDevices) return;
    
    for(int row = 0; row < 8; row++) {
        buffer[device * 8 + row] = 0;
        writeRegister(device, MAX7219_REG_DIGIT0 + row, 0);
    }
}

void MAX7219_LED_MATRIX::setRow(int device, int row, byte value) {
    if(device >= numDevices || row >= 8) return;
    buffer[device * 8 + row] = value;
    writeRegister(device, MAX7219_REG_DIGIT0 + row, value);
}

void MAX7219_LED_MATRIX::setColumn(int device, int col, byte value) {
    if(device >= numDevices || col >= 8) return;
    
    for(int row = 0; row < 8; row++) {
        if(value & (1 << row)) {
            buffer[device * 8 + row] |= (1 << col);
        } else {
            buffer[device * 8 + row] &= ~(1 << col);
        }
    }
    refresh();
}

void MAX7219_LED_MATRIX::setLed(int device, int row, int col, bool state) {
    if(device >= numDevices || row >= 8 || col >= 8) return;
    
    if(state) {
        buffer[device * 8 + row] |= (1 << col);
    } else {
        buffer[device * 8 + row] &= ~(1 << col);
    }
    writeRegister(device, MAX7219_REG_DIGIT0 + row, buffer[device * 8 + row]);
}

void MAX7219_LED_MATRIX::refresh() {
    for(int device = 0; device < numDevices; device++) {
        for(int row = 0; row < 8; row++) {
            writeRegister(device, MAX7219_REG_DIGIT0 + row, buffer[device * 8 + row]);
        }
    }
}

void MAX7219_LED_MATRIX::setPixel(int x, int y, bool state) {
    int device = x / 8;
    int col = x % 8;
    setLed(device, y, col, state);
}

void MAX7219_LED_MATRIX::drawBitmap(int device, const byte bitmap[8]) {
    if(device >= numDevices) return;
    
    for(int row = 0; row < 8; row++) {
        setRow(device, row, bitmap[row]);
    }
}

byte MAX7219_LED_MATRIX::getRow(int device, int row) {
    if(device >= numDevices || row >= 8) return 0;
    return buffer[device * 8 + row];
}