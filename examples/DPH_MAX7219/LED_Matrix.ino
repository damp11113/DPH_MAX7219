#include "DPH_MAX7219_Matrix.h"

MAX7219_LED_MATRIX matrix(11, 13, 10, 1);

void setup() {
    matrix.begin();
    matrix.setIntensity(8);
    
    byte smile[8] = {
        B00111100,
        B01000010,
        B10100101,
        B10000001,
        B10100101,
        B10011001,
        B01000010,
        B00111100
    };
    matrix.drawBitmap(0, smile);
}

void loop() {
}