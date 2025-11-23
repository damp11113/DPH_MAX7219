#include "DPH_MAX7219_Base.h"

MAX7219_Base::MAX7219_Base(int data, int clk, int cs, int devices) {
    dataPin = data;
    clkPin = clk;
    csPin = cs;
    numDevices = devices;
}

MAX7219_Base::~MAX7219_Base() {
}

void MAX7219_Base::sendByte(byte data) {
    for(int i = 7; i >= 0; i--) {
        digitalWrite(clkPin, LOW);
        digitalWrite(dataPin, (data >> i) & 0x01);
        digitalWrite(clkPin, HIGH);
    }
}

void MAX7219_Base::writeRegister(int device, byte reg, byte data) {
    digitalWrite(csPin, LOW);
    
    for(int i = numDevices - 1; i >= 0; i--) {
        if(i == device) {
            sendByte(reg);
            sendByte(data);
        } else {
            sendByte(MAX7219_REG_NOOP);
            sendByte(0);
        }
    }
    
    digitalWrite(csPin, HIGH);
}

void MAX7219_Base::setIntensity(int intensity) {
    intensity = constrain(intensity, 0, 15);
    for(int i = 0; i < numDevices; i++) {
        writeRegister(i, MAX7219_REG_INTENSITY, intensity);
    }
}

void MAX7219_Base::setIntensityDevice(int device, int intensity) {
    if(device >= numDevices) return;
    intensity = constrain(intensity, 0, 15);
    writeRegister(device, MAX7219_REG_INTENSITY, intensity);
}

void MAX7219_Base::shutdown(bool b) {
    for(int i = 0; i < numDevices; i++) {
        writeRegister(i, MAX7219_REG_SHUTDOWN, b ? 0x00 : 0x01);
    }
}

void MAX7219_Base::shutdownDevice(int device, bool b) {
    if(device >= numDevices) return;
    writeRegister(device, MAX7219_REG_SHUTDOWN, b ? 0x00 : 0x01);
}

void MAX7219_Base::displayTest(bool b) {
    for(int i = 0; i < numDevices; i++) {
        writeRegister(i, MAX7219_REG_DISPLAYTEST, b ? 0x01 : 0x00);
    }
}