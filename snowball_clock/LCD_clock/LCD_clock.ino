#include <LiquidCrystal.h>

#include <DS3231.h>

#define tempIn A0
#define select 8
#define change 9

// Operation modes
#define TIME_NOT_SET 0
#define NORMAL_CLOCK_DISPLAY 1
#define MENU_DISPLAY 2
#define ALARM 3

// Menu modes
#define ACTIVATE_SELECT 0
#define HOUR_SELECT 1
#define MINUTE_SELECT 2
#define CONFIRM_SCREEN 3

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// SDA: A4
// SCL: A5
DS3231 rtc(SDA, SCL);

// yr, mo, da ... are globals used to initialize the clock and used to keep track of time change
// yrSet, moSet, daSet ... are globals used during the initialization sequence

int yr = 2017;
bool yrSet = false;
int mo = 1;
bool monSet = false;
int da = 1;
bool daSet = false;
int hr = 12;
bool hrSet = false;
int mins = 0;
bool minsSet = false;
int se = 0;
bool secSet = false;
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
int alarmHr = 0;
bool alarmHrSet = false;
int alarmMin = 0;
bool alarmMinSet = false;
bool menuConfirmed = false;

void setup() {
  pinMode(select, INPUT);
  pinMode(change, INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //Serial.begin(9600);
  lcd.begin(16, 2);
  rtc.begin();
  //Serial.println("Set up complete");
}

void loop() {

  if (displayMode == TIME_NOT_SET) {
    if (!allSet) {
      if (!yrSet) {
        getYear();
        //Serial.println("Getting year");
      }
      else if (!monSet) {
        getMonth();
        //Serial.println("Getting month");
      }
      else if (!daSet) {
        getDay();
        //Serial.println("Getting day");
      }
      else if (!hrSet) {
        getHour();
        //Serial.println("Getting hour");
      }
      else if (!minsSet) {
        getMinute();
        //Serial.println("Getting minutes");
      }
      else if (!secSet) {
        getSecond();
        //Serial.println("Getting second");
      }
    }
    allSet = timeSetReady(yrSet, monSet, daSet, hrSet, minsSet, secSet);
    if (allSet) {
      rtc.setDate(da,mo,yr);
      rtc.setTime(hr,mins,se);
      displayMode = NORMAL_CLOCK_DISPLAY;
      //Serial.println("Initial ime set");
    }
  }
  else if (displayMode == NORMAL_CLOCK_DISPLAY) {
    Time t = rtc.getTime();
    LCDClockDisplay(t);
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
  }
  else if (displayMode == MENU_DISPLAY) {
    LCDMenuDisplay();
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
      getAlarmMinute();
      if (alarmMinSet) {
        alarmMinSet = false;
        menuMode = ACTIVATE_SELECT;
        displayMode = NORMAL_CLOCK_DISPLAY;
      }
    }
  }
  else if (displayMode == ALARM) {
    Time t = rtc.getTime();
    displayAlarm(t);
    // TODO: add in logic for snooze and turning off
    if (digitalRead(select) == HIGH) {
      selectPressed = true;
    }
    else {
      if (selectPressed == true) {
        selectPressed = false;
        displayMode = NORMAL_CLOCK_DISPLAY;
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
  
  delay(10);

}

// Returns true if all the fields have been initialized
bool timeSetReady(bool yr, bool mo, bool da, bool hr, bool mins, bool se) {
  return yr && mo && da && hr && mins && se;
}

void LCDClockDisplay(Time t) {

  if (se != t.sec || mins != t.min || hr != t.hour || da != t.date || mo != t.mon || yr != t.year){
    // Printing sequence
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(t.date);
    lcd.print(" ");
    lcd.print(t.mon);
    lcd.print(" ");
    lcd.print(t.year);
    lcd.setCursor(4, 1);
    lcd.print(t.hour);
    // Print minutes with formatting
    lcd.print(":");
    if (t.min < 10)
      lcd.print('0');
    lcd.print(t.min);
    // Print seconds with formatting
    lcd.print(":");
    if (t.sec < 10)
      lcd.print('0');
    lcd.print(t.sec);

    yr = t.year;
    mo = t.mon;
    da = t.date;
    hr = t.hour;
    mins = t.min;
    se = t.sec;
  }
}

// Get functions used during the initialization stage and alarm setting

void getYear() {
  lcd.clear();
  lcd.print("Year " + String(yr));
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      yr++;
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      yrSet = true;
    }
  }
}

void getMonth() {
  lcd.clear();
  lcd.print("Month " + String(mo));
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      mo++;
      if (mo >= 13) {
        mo = 1;
      }
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      monSet = true;
    }
  }
}

void getDay() {
  lcd.clear();
  lcd.print("Day " + String(da));
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      da++;
      if (da >= 32) {
        da = 1;
      }
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      daSet = true;
    }
  }
}

void getHour() {
  lcd.clear();
  lcd.print("Hour " + String(hr));
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      hr++;
      if (hr >= 24) {
        hr = 0;
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
  lcd.clear();
  lcd.print("Minute " + String(mins));
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

void getSecond() {
  lcd.clear();
  lcd.print("Second " + String(se));
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      se++;
      if (se >= 60) {
        se = 0;
      }
    }
  }
  if (digitalRead(select) == HIGH) {
    selectPressed = true;
  }
  else {
    if (selectPressed == true) {
      selectPressed = false;
      secSet = true;
    }
  }
}

void getOnOff() {
  lcd.clear();
  if (alarmOn) {
    lcd.print("Alarm: On");
  }
  else {
    lcd.print("Alarm: Off");
  }
  
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
  lcd.clear();
  lcd.print("Hour " + String(alarmHr));
  if (digitalRead(change) == HIGH) {
    changePressed = true;
  }
  else {
    if (changePressed == true) {
      changePressed = false;
      alarmHr++;
      if (alarmHr >= 24) {
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

void getAlarmMinute() {
  lcd.clear();
  lcd.print("Min " + String(alarmMin));
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
    }
  }
}

bool checkAlarm(Time t) {
  return (alarmHr == t.hour && alarmMin == t.min && t.sec == 0);
}

void LCDMenuDisplay() {
  
}

void displayAlarm(Time t) {
  digitalWrite(LED_BUILTIN, HIGH);
  
  lcd.clear();
  lcd.print("Alarm!!!");
  lcd.setCursor(4, 1);
  lcd.print(t.hour);
  // Print minutes with formatting
  lcd.print(":");
  if (t.min < 10)
    lcd.print('0');
  lcd.print(t.min);
  // Print seconds with formatting
  lcd.print(":");
  if (t.sec < 10)
    lcd.print('0');
  lcd.print(t.sec);

  yr = t.year;
  mo = t.mon;
  da = t.date;
  hr = t.hour;
  mins = t.min;
  se = t.sec;
}
