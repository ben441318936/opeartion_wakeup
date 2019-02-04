//We always have to include the library
#include "LedControl.h"

#include <DS3231.h>

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 10 is connected to the CLK 
 pin 11 is connected to LOAD / CS
 ***** Please set the number of devices you have *****
 But the maximum default of 8 MAX72XX wil also work.
 */
// LedControl(DIN,CLK,CS,num_device);
LedControl lc = LedControl(12,10,11,8);
int devices = lc.getDeviceCount();

// SDA: A4
// SCL: A5
DS3231 rtc(SDA, SCL);

const byte CHARS[][8] = {
{
  B01100000,
  B10010000,
  B10010000,
  B10010000,
  B10010000,
  B10010000,
  B10010000,
  B01100000
},
{
  B00100000,
  B01100000,
  B10100000,
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B11110000
},
{
  B01100000,
  B10010000,
  B00010000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
  B11110000
},
{
  B01100000,
  B10010000,
  B00010000,
  B01100000,
  B00010000,
  B00010000,
  B10010000,
  B01100000
},
{
  B00010000,
  B00110000,
  B01010000,
  B10010000,
  B11110000,
  B00010000,
  B00010000,
  B00010000
},
{
  B11110000,
  B10000000,
  B10000000,
  B11100000,
  B00010000,
  B00010000,
  B00010000,
  B11100000
},
{
  B01100000,
  B10010000,
  B10000000,
  B11100000,
  B10010000,
  B10010000,
  B10010000,
  B01100000
},
{
  B11110000,
  B00010000,
  B00010000,
  B00100000,
  B01000000,
  B01000000,
  B01000000,
  B01000000
},
{
  B01100000,
  B10010000,
  B10010000,
  B01100000,
  B10010000,
  B10010000,
  B10010000,
  B01100000
},
{
  B01100000,
  B10010000,
  B10010000,
  B01110000,
  B00010000,
  B00010000,
  B10010000,
  B01100000
},
{
  B00011000,
  B00100100,
  B00100100,
  B00100100,
  B00111100,
  B00100100,
  B00100100,
  B00100100
},
{
  B00111000,
  B00100100,
  B00100100,
  B00100100,
  B00111000,
  B00100000,
  B00100000,
  B00100000
},
{
  B00000000,
  B00001000,
  B00001000,
  B00000000,
  B00000000,
  B00001000,
  B00001000,
  B00000000
}
};
const int CHARS_LEN = sizeof(CHARS)/8;

byte DISPLAY_CHARS[4][8] = {
{
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},
{
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},
{
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},
{
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
}
};


/* we always wait a bit between updates of the display */
//unsigned long delaytime=500;

/* 
 This time we have more than one device. 
 But all of them have to be initialized 
 individually.
 */
void setup() {
  //we have already set the number of devices when we created the LedControl
  //we have to init all devices in a loop
  
  for(int address=0;address<devices;address++) {
    //The MAX72XX is in power-saving mode on startup
    lc.shutdown(address,false);
    //Set the brightness to a medium values
    lc.setIntensity(address,2);
    //and clear the display
    lc.clearDisplay(address);
  }
  
  Serial.begin(9600);
  rtc.begin();
  rtc.setDate(1,1,2019);
  rtc.setTime(0,0,0);
  Time t = rtc.getTime();
  construct_display_chars(t);
  for (int j=0; j<8; j++){
    for (int i=0; i<4; i++) {
      for (int k = 7; k >= 0; k--)
      {
        bool b = bitRead(DISPLAY_CHARS[i][j], k);
        Serial.print(b);
      }
    }
    Serial.println("");
  }
}

void loop() { 
  Time t = rtc.getTime();
  construct_display_chars(t);
  LED_matrix_display();
  delay(1000);
}

void construct_display_chars(Time t) {
  // Extract digits of the hour
  int hour = t.hour;
  int hr_first_dig = 0;
  int hr_second_dig = 0;
  if (t.hour > 12) {
    hour = t.hour - 12;
    hr_first_dig = (hour / 10) % 10;
    hr_second_dig = hour % 10;
  }
  else if (t.hour == 0){
    hr_first_dig = 1;
    hr_second_dig = 2;
  }
  else {
    hr_first_dig = (hour / 10) % 10;
    hr_second_dig = hour % 10;
  }
  // Set first matrix
  if (hr_first_dig == 0) {
    // No leading 0 for hour
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[0][i] = CHARS[hr_second_dig][i] >> 7;
    }
  }
  else {
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[0][i] = (CHARS[hr_first_dig][i] >> 2) | (CHARS[hr_second_dig][i] >> 7);
    }
  }
  // Set second and third matrix
  int min_first_dig = (t.min / 10) % 10;
  int min_second_dig = t.min % 10;
  for (int i=0; i<8; i++) {
    DISPLAY_CHARS[1][i] = (CHARS[hr_second_dig][i] << 1) | (CHARS[12][i]) | (CHARS[min_first_dig][i] >> 6);
    DISPLAY_CHARS[2][i] = (CHARS[min_first_dig][i] << 2) | (CHARS[min_second_dig][i] >> 3);
  }
  // Set fourth matrix
  if (t.hour >= 12) {
    // PM
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[3][i] = CHARS[11][i];
    }
  }
  else {
    // AM
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[3][i] = CHARS[10][i];
    }
  }
  return;
}

void clear_display_chars() {
  for (int device = 0; device < 4; device++){
    for (int row = 0; row < 8; row++) {
      DISPLAY_CHARS[device][row] = B00000000;
    }
  }
}

void LED_matrix_display() {
  for (int device = 0; device < 4; device++){
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        bool b = bitRead(DISPLAY_CHARS[device][row], col);
        lc.setLed(3-device,row,7-col,b);
      }
    }
  }
}
