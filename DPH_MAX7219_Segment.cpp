#include "DPH_MAX7219_Segment.h"

MAX7219_SEVEN_SEGMENT::MAX7219_SEVEN_SEGMENT(int data, int clk, int cs, int devices)
    : MAX7219_Base(data, clk, cs, devices) {
    decodeMode = false;
}

void MAX7219_SEVEN_SEGMENT::begin() {
    beginWithDecode(true);
}

void MAX7219_SEVEN_SEGMENT::beginWithDecode(bool decode) {
    pinMode(dataPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    
    decodeMode = decode;
    
    for(int i = 0; i < numDevices; i++) {
        writeRegister(i, MAX7219_REG_SCANLIMIT, 0x07);
        writeRegister(i, MAX7219_REG_DECODEMODE, decode ? 0xFF : 0x00);
        writeRegister(i, MAX7219_REG_DISPLAYTEST, 0x00);
        writeRegister(i, MAX7219_REG_INTENSITY, 0x08);
        writeRegister(i, MAX7219_REG_SHUTDOWN, 0x01);
    }
    clear();
}

void MAX7219_SEVEN_SEGMENT::clear() {
    for(int i = 0; i < numDevices; i++) {
        clearDevice(i);
    }
}

void MAX7219_SEVEN_SEGMENT::clearDevice(int device) {
    if(device >= numDevices) return;
    
    for(int pos = 0; pos < 8; pos++) {
        writeRegister(device, MAX7219_REG_DIGIT0 + pos, 0x0F); // Blank in decode mode
    }
}

void MAX7219_SEVEN_SEGMENT::setDigit(int device, int position, byte value, bool dp) {
    if(device >= numDevices || position >= 8) return;
    if(!decodeMode) return;
    
    byte data = value & 0x0F;
    if(dp) data |= 0x80;
    writeRegister(device, MAX7219_REG_DIGIT0 + position, data);
}

void MAX7219_SEVEN_SEGMENT::setChar(int device, int position, char c, bool dp) {
    if(device >= numDevices || position >= 8) return;
    if(!decodeMode) return;
    
    byte value;
    if(c >= '0' && c <= '9') {
        value = c - '0';
    } else if(c == '-') {
        value = 0x0A;
    } else if(c == 'E' || c == 'e') {
        value = 0x0B;
    } else if(c == 'H' || c == 'h') {
        value = 0x0C;
    } else if(c == 'L' || c == 'l') {
        value = 0x0D;
    } else if(c == 'P' || c == 'p') {
        value = 0x0E;
    } else {
        value = 0x0F; // Blank
    }
    
    if(dp) value |= 0x80;
    writeRegister(device, MAX7219_REG_DIGIT0 + position, value);
}

void MAX7219_SEVEN_SEGMENT::setSegments(int device, int position, byte segments) {
    if(device >= numDevices || position >= 8) return;
    writeRegister(device, MAX7219_REG_DIGIT0 + position, segments);
}

void MAX7219_SEVEN_SEGMENT::displayNumber(int device, long number, bool leadingZeros) {
    if(device >= numDevices) return;
    
    bool negative = number < 0;
    if(negative) number = -number;
    
    for(int pos = 0; pos < 8; pos++) {
        if(number > 0 || pos == 0 || leadingZeros) {
            setDigit(device, pos, number % 10);
            number /= 10;
        } else if(negative && number == 0) {
            setChar(device, pos, '-');
            negative = false;
        } else {
            setChar(device, pos, ' ');
        }
    }
}

void MAX7219_SEVEN_SEGMENT::displayFloat(int device, float number, int decimals) {
    if(device >= numDevices || decimals < 0 || decimals > 7) return;
    
    long intPart = (long)(number * pow(10, decimals));
    bool negative = intPart < 0;
    if(negative) intPart = -intPart;
    
    for(int pos = 0; pos < 8; pos++) {
        if(pos == decimals && decimals > 0) {
            setDigit(device, pos, intPart % 10, true); // With decimal point
        } else if(intPart > 0 || pos <= decimals) {
            setDigit(device, pos, intPart % 10);
        } else if(negative && intPart == 0) {
            setChar(device, pos, '-');
            negative = false;
        } else {
            setChar(device, pos, ' ');
        }
        intPart /= 10;
    }
}

void MAX7219_SEVEN_SEGMENT::displayText(int device, const char* text) {
    if(device >= numDevices) return;
    
    int len = strlen(text);
    for(int pos = 0; pos < 8; pos++) {
        if(pos < len) {
            setChar(device, pos, text[len - 1 - pos]); // Reverse order
        } else {
            setChar(device, pos, ' ');
        }
    }
}