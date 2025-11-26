#include "DPH_MAX7219_Matrix.h"

#define DIN 0
#define CLK 2
#define CS  1
#define Devices 3

MAX7219_LED_MATRIX display(DIN, CLK, CS, Devices);

#define CMD_CLEAR           0x01
#define CMD_CLEAR_DEVICE    0x02
#define CMD_SET_ROW         0x03
#define CMD_SET_COLUMN      0x04
#define CMD_SET_LED         0x05
#define CMD_SET_PIXEL       0x06
#define CMD_DRAW_BITMAP     0x07
#define CMD_SET_INTENSITY   0x08
#define CMD_SET_INTENSITY_DEV 0x09

void setup() {
  Serial.begin(115200);
  display.begin();
  display.setIntensity(10);
  display.clear();
}

void loop() {
  if (Serial.available() > 0) {
    byte cmdId = Serial.read();
    
    switch (cmdId) {
      case CMD_CLEAR:
        display.clear();
        break;
        
      case CMD_CLEAR_DEVICE: {
        if (Serial.available() >= 1) {
          byte device = Serial.read();
          display.clearDevice(device);
        }
        break;
      }
      
      case CMD_SET_ROW: {
        if (Serial.available() >= 3) {
          byte device = Serial.read();
          byte row = Serial.read();
          byte value = Serial.read();
          display.setRow(device, row, value);
        }
        break;
      }
      
      case CMD_SET_COLUMN: {
        if (Serial.available() >= 3) {
          byte device = Serial.read();
          byte col = Serial.read();
          byte value = Serial.read();
          display.setColumn(device, col, value);
        }
        break;
      }
      
      case CMD_SET_LED: {
        if (Serial.available() >= 4) {
          byte device = Serial.read();
          byte row = Serial.read();
          byte col = Serial.read();
          byte state = Serial.read();
          display.setLed(device, row, col, state != 0);
        }
        break;
      }
      
      case CMD_SET_PIXEL: {
        if (Serial.available() >= 3) {
          byte x = Serial.read();
          byte y = Serial.read();
          byte state = Serial.read();
          display.setPixel(x, y, state != 0);
        }
        break;
      }
      
      case CMD_DRAW_BITMAP: {
        if (Serial.available() >= 9) {
          byte device = Serial.read();
          byte bitmap[8];
          for (int i = 0; i < 8; i++) {
            bitmap[i] = Serial.read();
          }
          display.drawBitmap(device, bitmap);
        }
        break;
      }
      
      case CMD_SET_INTENSITY: {
        if (Serial.available() >= 1) {
          byte intensity = Serial.read();
          display.setIntensity(intensity);
        }
        break;
      }
      
      case CMD_SET_INTENSITY_DEV: {
        if (Serial.available() >= 2) {
          byte device = Serial.read();
          byte intensity = Serial.read();
          display.setIntensityDevice(device, intensity);
        }
        break;
      }
    }
  }
}