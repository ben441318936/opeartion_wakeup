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
// 0
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
// 1
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
// 2
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
// 3
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
// 4
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
// 5
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
// 6
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
// 7 
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
// 8
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
// 9
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
// A
{
  B01100000,
  B10010000,
  B10010000,
  B10010000,
  B11110000,
  B10010000,
  B10010000,
  B10010000
},
// P
{
  B11100000,
  B10010000,
  B10010000,
  B10010000,
  B11100000,
  B10000000,
  B10000000,
  B10000000
},
// :
{
  B00000000,
  B10000000,
  B10000000,
  B00000000,
  B00000000,
  B10000000,
  B10000000,
  B00000000
},
// H
{
  B10001000,
  B10001000,
  B10001000,
  B11111000,
  B10001000,
  B10001000,
  B10001000,
  B10001000
},
// R
{
  B11110000,
  B10001000,
  B10001000,
  B11110000,
  B11000000,
  B10100000,
  B10010000,
  B10001000
},
// M
{
  B10001000,
  B11011000,
  B10101000,
  B10101000,
  B10101000,
  B10001000,
  B10001000,
  B10001000
},
// I
{
  B11111000,
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B00100000,
  B11111000
},
// N
{
  B10001000,
  B10001000,
  B11001000,
  B11101000,
  B10111000,
  B10011000,
  B10001000,
  B10001000
},
// O
{
  B01110000,
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B10001000,
  B01110000
},
// F
{
  B11111000,
  B10000000,
  B10000000,
  B11110000,
  B10000000,
  B10000000,
  B10000000,
  B10000000
},
// Alarm icon
{
  B01100110,
  B10000001,
  B00111100,
  B01010010,
  B01010010,
  B01010010,
  B01001010,
  B00111100
},{
  B00011000,
  B00100101,
  B00000100,
  B00000100,
  B00001000,
  B00010000,
  B00100001,
  B00111100
},{
  B11000001,
  B00101010,
  B00101010,
  B11000010,
  B00100010,
  B00101010,
  B00101010,
  B11000001
},{
  B10000010,
  B01000110,
  B01001010,
  B01010010,
  B01011110,
  B01000010,
  B01000010,
  B10000010
},{
  B10001011,
  B10001010,
  B10001010,
  B11111011,
  B10001011,
  B10001010,
  B10001010,
  B10001010
},{
  B11000000,
  B00101001,
  B00101010,
  B11000000,
  B00000000,
  B10001000,
  B01001000,
  B00100011
},{
  B10001100,
  B10010010,
  B10000010,
  B10000010,
  B10000100,
  B10001000,
  B10010000,
  B11011110
}};
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
  construct_time_chars(t);
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
  construct_time_chars(t);
  LED_display();
  delay(1000);
}

void construct_time_chars(Time t) {
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
    DISPLAY_CHARS[1][i] = (CHARS[hr_second_dig][i] << 1) | (CHARS[12][i] >> 4) | (CHARS[min_first_dig][i] >> 6);
    DISPLAY_CHARS[2][i] = (CHARS[min_first_dig][i] << 2) | (CHARS[min_second_dig][i] >> 3);
  }
  // Set fourth matrix
  if (t.hour >= 12) {
    // PM
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[3][i] = CHARS[11][i] >> 2;
    }
  }
  else {
    // AM
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[3][i] = CHARS[10][i] >> 2;
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

void LED_display() {
  for (int device = 0; device < 4; device++){
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        bool b = bitRead(DISPLAY_CHARS[device][row], col);
        lc.setLed(3-device,row,7-col,b);
      }
    }
  }
}

void construct_get_hr_chars(int x) {
  int first_dig = (x / 10) % 10;
  int second_dig = x % 10;
  for (int i=0; i<8; i++) {
      DISPLAY_CHARS[0][i] = (CHARS[13][i]) | (CHARS[14][i] >> 6);
      if (first_dig != 0) {
        DISPLAY_CHARS[1][i] = (CHARS[14][i] << 2) | (CHARS[12][i] >> 4) | (CHARS[first_dig][i] >> 6);
        DISPLAY_CHARS[2][i] = (CHARS[first_dig][i] << 2) | (CHARS[second_dig][i] << 3);
      }
      else {
        DISPLAY_CHARS[1][i] = (CHARS[14][i] << 2) | (CHARS[12][i] >> 4) | (CHARS[second_dig][i] >> 6);
        DISPLAY_CHARS[2][i] = (CHARS[second_dig][i] << 2);
      }
      DISPLAY_CHARS[3][i] = B00000000;
  }
}

void construct_get_min_chars(int x) {
  int first_dig = (x / 10) % 10;
  int second_dig = x % 10;
  for (int i=0; i<8; i++) {
      DISPLAY_CHARS[0][i] = (CHARS[15][i]) | (CHARS[16][i] >> 6);
      DISPLAY_CHARS[1][i] = (CHARS[16][i] << 2) | (CHARS[17][i] >> 4);
      DISPLAY_CHARS[2][i] = (CHARS[17][i] << 4) | (CHARS[12][i] >> 2) | (CHARS[first_dig][i] >> 4);
      DISPLAY_CHARS[3][i] = CHARS[second_dig][i] >> 1;
  }
}

void construct_get_am_pm_chars(bool x) {
  for (int i=0; i<8; i++) {
    if (x) {
      // AM case
      DISPLAY_CHARS[0][i] = (CHARS[10][i]) | (CHARS[15][i] >> 5);
      DISPLAY_CHARS[1][i] = (CHARS[15][i] << 3)
    }
    else {
      // PM case
      DISPLAY_CHARS[0][i] = (CHARS[11][i]) | (CHARS[15][i] >> 5);
      DISPLAY_CHARS[1][i] = (CHARS[15][i] << 3)
    }  
    DISPLAY_CHARS[2][i] = B00000000;
    DISPLAY_CHARS[3][i] = B00000000;
  }
}

void construct_alarm_menu_chars(bool on) {
  if (on) {
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[0][i] = (CHARS[20][i]);
      DISPLAY_CHARS[1][i] = (CHARS[18][i] >> 2);
      DISPLAY_CHARS[2][i] = (CHARS[17][i]);
    }
  }
  else {
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[0][i] = (CHARS[20][i]);
      DISPLAY_CHARS[1][i] = (CHARS[18][i] >> 2);
      DISPLAY_CHARS[2][i] = (CHARS[19][i]) | (CHARS[19][i] >> 6);
      DISPLAY_CHARS[3][i] = (CHARS[19][i] << 2);
    }
  }
}

void getHour() {
  construct_get_hr_chars(hr);
  LED_display();
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      hr++;
      if (hr > 12) {
        hr = 1;
      }
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      hrSet = true;
    }
  }
}

void getMinute() {
  construct_get_hr_chars(mins);
  LED_display();
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      mins++;
      if (mins >= 60) {
        mins = 0;
      }
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      minsSet = true;
    }
  }
}

void getAMPM() {
  construct_get_am_pm_chars(am);
  LED_display();
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      am = !am;
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      ampmSet = true;
    }
  }
}

void getAlarmHour() {
  construct_get_alarm_hr_chars(alarmHr)
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      alarmHr++;
      if (alarmHr > 12) {
        alarmHr = 0;
      }
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      alarmHrSet = true;
    }
  }
}