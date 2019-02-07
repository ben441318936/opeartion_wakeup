#include <DS3231.h>
#include "LedControl.h"

// Pins
#define select 6
#define change 7

// Operation modes
#define TIME_NOT_SET 0
#define NORMAL_CLOCK_DISPLAY 1
#define MENU_DISPLAY 2
#define ALARM 3

// Menu modes
#define ACTIVATE_SELECT 0
#define HOUR_SELECT 1
#define MINUTE_SELECT 2
#define AMPM_SELECT 3
#define CONFIRM_SCREEN 4

#define SNOOZE_TIME 1

// LedControl(DIN,CLK,CS,num_device);
LedControl lc = LedControl(12,10,11,8);
int num_devices = lc.getDeviceCount();
// SDA: A4
// SCL: A5
DS3231 rtc(SDA, SCL);

// yr, mo, da ... are globals used to initialize the clock and used to keep track of time change
// yrSet, moSet, daSet ... are globals used during the initialization sequence

int yr = 2018;
int mo = 1;
int da = 1;
int se = 0;
int hr = 12;
bool hrSet = false;
int mins = 0;
bool minsSet = false;
bool am = true;
bool ampmSet = false;
bool allSet = false;

// Buttons are used as a trigger to avoid lingering press effects

bool changePressed = false;
bool selectPressed = false;

// Operation modes
int displayMode = TIME_NOT_SET;

// Menu modes
int menuMode = ACTIVATE_SELECT;

bool alarmOn = false;
bool alarmOnOffSet = false;
int alarmHr = 12;
bool alarmHrSet = false;
int alarmMin = 0;
bool alarmMinSet = false;
bool alarmAM = true;
bool alarmAMPMSet = false;
bool menuConfirmed = false;

bool snoozeOn = false;
int snoozeHr = 0;
int snoozeMin = 0;
bool snoozeAM = true;

// Stores the building block characters
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
},
// ((
{
  B00100000,
  B01000100,
  B10001000,
  B10010000,
  B10010000,
  B10001000,
  B01000100,
  B00100000
},
// ))
{
  B00000100,
  B00100010,
  B00010001,
  B00001001,
  B00001001,
  B00010001,
  B00100010,
  B00000100
},
/* Some example arrangements
{
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
}*/};
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

byte PREV_DISPLAY_CHARS[4][8] = {
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

void setup() {
  pinMode(select, INPUT);
  pinMode(change, INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  
  //we have already set the number of devices when we created the LedControl
  //we have to init all devices in a loop
  
  for(int address=0;address<num_devices;address++) {
    //The MAX72XX is in power-saving mode on startup
    lc.shutdown(address,false);
    //Set the brightness to a medium values
    lc.setIntensity(address,2);
    //and clear the display
    lc.clearDisplay(address);
  }

  rtc.begin();
  Serial.println("Set up complete");
}

void loop() {

  if (displayMode == TIME_NOT_SET) {
    if (!allSet) {
      if (!hrSet) {
        getHour();
        //Serial.println("Getting hour");
      }
      else if (!minsSet) {
        getMinute();
        //Serial.println("Getting minutes");
      }
      else if (!ampmSet) {
        getAMPM();
      }
    }
    allSet = timeSetReady(hrSet, minsSet, ampmSet);
    if (allSet) {
      if (am && hr == 12) {
        hr = 0;
      }
      else if (!am && hr != 12) {
        hr = hr + 12;
      }
      rtc.setDate(da,mo,yr);
      rtc.setTime(hr,mins,se);
      displayMode = NORMAL_CLOCK_DISPLAY;
      //Serial.println("Initial ime set");
    }
  }
  else if (displayMode == NORMAL_CLOCK_DISPLAY) {
    Time t = rtc.getTime();
    construct_time_chars(t);
    //LED_display();
    //Serial.println("Clock display");
    if (digitalRead(select) == HIGH) {
      selectPressed = true;
    }
    else {
      if (selectPressed == true) {
        selectPressed = false;
        displayMode = MENU_DISPLAY;
      }
    }
    if (alarmOn && checkAlarm(t)) {
      displayMode = ALARM;
    }
    if (snoozeOn && checkSnooze(t)) {
      displayMode = ALARM;
    }
  }
  else if (displayMode == MENU_DISPLAY) {
    if (menuMode == ACTIVATE_SELECT) {
      getOnOff();
      if (alarmOnOffSet) {
        if (alarmOn) {
          menuMode = HOUR_SELECT;
        }
        else {
          menuMode = ACTIVATE_SELECT;
          displayMode = NORMAL_CLOCK_DISPLAY;
        }
        alarmOnOffSet = false;
      }
    }
    else if (menuMode == HOUR_SELECT) {
      getAlarmHour();
      if (alarmHrSet) {
        menuMode = MINUTE_SELECT;
        alarmHrSet = false;
      }
    }
    else if (menuMode == MINUTE_SELECT) {
      Serial.println("Getting alarm min");
      getAlarmMinute();
      if (alarmMinSet) {
        alarmMinSet = false;
        menuMode = AMPM_SELECT;
        Serial.println("Getting alarm min done");
      }
    }
    else if (menuMode == AMPM_SELECT) {
      getAlarmAMPM();
      Serial.println("Getting alarm am");
      if (alarmAMPMSet) {
        alarmAMPMSet = false;
        menuMode = ACTIVATE_SELECT;
        snoozeHr = alarmHr;
        snoozeMin = alarmMin;
        snoozeAM = alarmAM;
        displayMode = NORMAL_CLOCK_DISPLAY;
      }
    }
  }
  else if (displayMode == ALARM) {
    Time t = rtc.getTime();
    displayAlarm(t);
    if (digitalRead(change) == HIGH) {
      changePressed = true;
    } 
    else if (changePressed == true) {
      changePressed = false;
      snoozeOn = true;
      snoozeMin = t.min + SNOOZE_TIME;
      if (snoozeMin >= 60) {
        snoozeMin = snoozeMin - 60;
        if (t.hour == 0) {
          snoozeHr = 1;
          snoozeAM = true;
        }
        else if (t.hour < 11) {
          snoozeHr = t.hour + 1;
          snoozeAM = true;
        }
        else if (t.hour == 11) {
          snoozeHr = t.hour + 1;
          snoozeAM = false;
        }
        else if (t.hour == 12) {
          snoozeHr = 1;
          snoozeAM = false;
        }
        else if (t.hour < 23) {
          snoozeHr = t.hour - 12 + 1;
          snoozeAM = false;
        }
        else if (t.hour == 23) {
          snoozeHr = 12;
          snoozeAM = true;
        }
      }
      else {
        if (t.hour == 0) {
          snoozeHr = 12;
          snoozeAM = true;
        }
        if (t.hour < 12) {
          snoozeHr = t.hour;
          snoozeAM = true;
        }
        else if (t.hour == 12) {
          snoozeHr = t.hour;
          snoozeAM = false;
        }
        else {
          snoozeHr = t.hour - 12;
          snoozeAM = false;
        }
      }
      displayMode = NORMAL_CLOCK_DISPLAY;
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (digitalRead(select) == HIGH) {
      selectPressed = true;
    }
    else if (selectPressed == true) {
      selectPressed = false;
      displayMode = NORMAL_CLOCK_DISPLAY;
      snoozeOn = false;
      snoozeHr = alarmHr;
      snoozeMin = alarmMin;
      snoozeAM = alarmAM;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  Serial.println("Starting to display...");
  LED_display();
  Serial.println("Finished display.");

}

// Returns true if all the fields have been initialized
bool timeSetReady(bool hr, bool mins, bool ampm) {
  return hr && mins && ampm;
}


// Get functions used during the initialization stage and alarm setting

void getHour() {
  Serial.println("Starting to construct get hour...");
  construct_get_hr_chars(hr);
  Serial.println("Finished construct get hour.");
  //LED_display();
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
  Serial.println("Starting to construct get mins...");
  construct_get_min_chars(mins);
  Serial.println("Finished construct get mins.");
  //LED_display();
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
  Serial.println("Starting to construct get AMPM...");
  construct_get_am_pm_chars(am);
  Serial.println("Finished construct get AMPM.");
  //LED_display();
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

void getOnOff() {
  Serial.println("Starting to construct get on off...");
  construct_get_menu_on_off_chars(alarmOn);
  Serial.println("Finished construct get on off.");
  //LED_display();
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      alarmOn = !alarmOn;
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      alarmOnOffSet = true;
    }
  }
}

void getAlarmHour() {
  Serial.println("Starting to construct get alarm hour...");
  construct_get_hr_chars(alarmHr);
  Serial.println("Finished construct get alarm hour.");
  //LED_display();
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      alarmHr++;
      if (alarmHr > 12) {
        alarmHr = 1;
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

void getAlarmMinute() {
  Serial.println("Starting to construct get alarm mins...");
  construct_get_min_chars(alarmMin);
  Serial.println("Finished construct get alarm mins.");
  //LED_display();
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      alarmMin++;
      if (alarmMin >= 60) {
        alarmMin = 0;
      }
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      alarmMinSet = true;
      Serial.println("Setting alarm min true");
    }
  }
}

void getAlarmAMPM() {
  Serial.println("Starting to construct get alarm AMPM...");
  construct_get_am_pm_chars(alarmAM);
  Serial.println("Finished construct get alarm AMPM.");
  //LED_display();
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      alarmAM = !alarmAM;
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      alarmAMPMSet = true;
    }
  }
}

bool checkAlarm(Time t) {
  if (alarmAM && alarmHr == 12) {
    return (0 == t.hour && alarmMin == t.min && t.sec == 0);
  }
  else if (!alarmAM && alarmHr != 12) {
    return (alarmHr+12 == t.hour && alarmMin == t.min && t.sec == 0);
  }
  else {
    return (alarmHr == t.hour && alarmMin == t.min && t.sec == 0);
  }
}

bool checkSnooze(Time t) {
  if (snoozeAM && snoozeHr == 12) {
    return (0 == t.hour && snoozeMin == t.min && t.sec == 0);
  }
  else if (!snoozeAM && snoozeHr != 12) {
    return (snoozeHr+12 == t.hour && snoozeMin == t.min && t.sec == 0);
  }
  else {
    return (snoozeHr == t.hour && snoozeMin == t.min && t.sec == 0);
  }
}

void displayAlarm(Time t) {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Starting to construct alarm chars...");
  construct_alarm_chars(t);
  Serial.println("Finished construct alarm chats.");
  //LED_display();
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
  for (int device = 3; device >= 0; device--){
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        bool b = bitRead(DISPLAY_CHARS[device][row], col);
        bool b2 = bitRead(PREV_DISPLAY_CHARS[device][row], col);
        if (b != b2) {
          lc.setLed(3-device,row,7-col,b);
        }
      }
      PREV_DISPLAY_CHARS[device][row] = DISPLAY_CHARS[device][row];
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
        DISPLAY_CHARS[2][i] = (CHARS[first_dig][i] << 2) | (CHARS[second_dig][i] >> 3);
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
      if (first_dig != 0) {
        DISPLAY_CHARS[2][i] = (CHARS[17][i] << 4) | (CHARS[12][i] >> 2) | (CHARS[first_dig][i] >> 4);
        DISPLAY_CHARS[3][i] = CHARS[second_dig][i] >> 1;
      }
      else {
        DISPLAY_CHARS[2][i] = (CHARS[17][i] << 4) | (CHARS[12][i] >> 2) | (CHARS[second_dig][i] >> 4);
        DISPLAY_CHARS[3][i] = B00000000;
      }
  }
}

void construct_get_am_pm_chars(bool x) {
  for (int i=0; i<8; i++) {
    if (x) {
      // AM case
      DISPLAY_CHARS[0][i] = (CHARS[10][i]) | (CHARS[15][i] >> 5);
      DISPLAY_CHARS[1][i] = (CHARS[15][i] << 3);
    }
    else {
      // PM case
      DISPLAY_CHARS[0][i] = (CHARS[11][i]) | (CHARS[15][i] >> 5);
      DISPLAY_CHARS[1][i] = (CHARS[15][i] << 3);
    }  
    DISPLAY_CHARS[2][i] = B00000000;
    DISPLAY_CHARS[3][i] = B00000000;
  }
}

void construct_get_menu_on_off_chars(bool on) {
  if (on) {
    for (int i=0; i<8; i++) {
      DISPLAY_CHARS[0][i] = (CHARS[20][i]);
      DISPLAY_CHARS[1][i] = (CHARS[18][i] >> 2);
      DISPLAY_CHARS[2][i] = (CHARS[17][i]);
      DISPLAY_CHARS[3][i] = B00000000;
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

void construct_alarm_chars(Time t) {
  for (int i=0; i<8; i++) {
    DISPLAY_CHARS[0][i] = (CHARS[21][i]);
    DISPLAY_CHARS[1][i] = (CHARS[20][i]);
    DISPLAY_CHARS[2][i] = (CHARS[22][i]);
    DISPLAY_CHARS[3][i] = B00000000;
  }
}
