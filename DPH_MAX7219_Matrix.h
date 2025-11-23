#ifndef MAX7219_LED_MATRIX_H
#define MAX7219_LED_MATRIX_H

#include "DPH_MAX7219_Base.h"

class MAX7219_LED_MATRIX : public MAX7219_Base {
private:
    byte *buffer;
    
public:
    MAX7219_LED_MATRIX(int data, int clk, int cs, int devices = 1);
    ~MAX7219_LED_MATRIX();
    
    void begin() override;
    void clear();
    void clearDevice(int device);
    
    void setRow(int device, int row, byte value);
    void setColumn(int device, int col, byte value);
    void setLed(int device, int row, int col, bool state);
    void setPixel(int x, int y, bool state);
    
    void refresh();
    void drawBitmap(int device, const byte bitmap[8]);
    byte getRow(int device, int row);
};

#endif
