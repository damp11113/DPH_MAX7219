#ifndef MAX7219_SEVEN_SEGMENT_H
#define MAX7219_SEVEN_SEGMENT_H

#include "DPH_MAX7219_Base.h"

class MAX7219_SEVEN_SEGMENT : public MAX7219_Base {
private:
    bool decodeMode;
    
public:
    MAX7219_SEVEN_SEGMENT(int data, int clk, int cs, int devices = 1);
    
    void begin() override;
    void beginWithDecode(bool decode = true);
    void clear();
    void clearDevice(int device);
    
    // For BCD decode mode (0-9, -, E, H, L, P, blank)
    void setDigit(int device, int position, byte value, bool dp = false);
    void setChar(int device, int position, char c, bool dp = false);
    
    // For no-decode mode (raw segment control)
    void setSegments(int device, int position, byte segments);
    
    // High-level functions
    void displayNumber(int device, long number, bool leadingZeros = false);
    void displayFloat(int device, float number, int decimals = 2);
    void displayText(int device, const char* text);
};

#endif