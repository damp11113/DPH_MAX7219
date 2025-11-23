#include "DPH_MAX7219_Segment.h"

MAX7219_SEVEN_SEGMENT display(11, 13, 10, 1);

void setup() {
    display.begin();
    display.setIntensity(8);
    
    // Display number
    display.displayNumber(0, 12345678);
    delay(2000);
    
    // Display float with 2 decimals
    display.displayFloat(0, 3.14159, 2); // Shows 3.14
    delay(2000);
    
    // Display text
    display.displayText(0, "HELLO");
}

void loop() {
}