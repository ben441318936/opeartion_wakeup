//#include <Time.h>
//#include <TimeLib.h>
#include <LiquidCrystal.h>

#include <DS3231.h>

#define tempIn A0
#define confirm 8
#define numUp 9

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// SDA: A4
// SCL: A5
DS3231 rtc(SDA, SCL);

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

bool numUpPressed = false;
bool confirmPressed = false;

bool time_set = false;

void setup() {
  pinMode(confirm, INPUT);
  pinMode(numUp, INPUT);
  //Serial.begin(9600);
  lcd.begin(16, 2);
  rtc.begin();

}

void loop() {

  if (time_set) {
    /* 
     * The following logic sequence is replaced 
     * by the actual code to reduce function call overhead
     * for time efficiency sincemtime accuracy is critical
    
    if (timeChanged()){
      lcd.clear();
      LCDClockDisplay();
    }
    */

    Time t = rtc.getTime();    
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

      /*
      if (mins != t.min) {
        setTime(t.hour, t.min, t.sec, t.date, t.mon, t.year);
      }
      */

      yr = t.year;
      mo = t.mon;
      da = t.date;
      hr = t.hour;
      mins = t.min;
      se = t.sec;
    }
  }
  else {
    if (!allSet) {
      if (!yrSet) {
        getYear();
      }
      else if (!monSet) {
        getMonth();
      }
      else if (!daSet) {
        getDay();
      }
      else if (!hrSet) {
        getHour();
      }
      else if (!minsSet) {
        getMinute();
      }
      else if (!secSet) {
        getSecond();
      }
    }
    allSet = timeSetReady(yrSet, monSet, daSet, hrSet, minsSet, secSet);
    if (allSet) {
      if (!time_set) {
        //setTime(hr, mins, se, da, mo, yr);
        rtc.setDate(da,mo,yr);
        rtc.setTime(hr,mins,se);
        time_set = true;
      }
    }
  }
  
  delay(10);

}

bool timeSetReady(bool yr, bool mo, bool da, bool hr, bool mins, bool se) {
  return yr && mo && da && hr && mins && se;
}

/*
bool timeChanged() {
  Time t = rtc.getTime();
  bool result = true;
  if (se == t.sec && mins == t.min && hr == t.hour && da == t.date && mo == t.mon && yr == t.year){
    result = false;
  }
  else {
    yr = t.year;
    mo = t.mon;
    da = t.date;
    hr = t.hour;
    mins = t.min;
    se = t.sec;
  }
  return result;
}
*/
/*
void LCDClockDisplay() {
  Time t = rtc.getTime();
  yr = t.year;
  mo = t.mon;
  da = t.date;
  hr = t.hour;
  mins = t.min;
  se = t.sec;

  lcd.clear()
  lcd.setCursor(3, 0);
  lcd.print(da);
  lcd.print(" ");
  lcd.print(mo);
  lcd.print(" ");
  lcd.print(yr);
  lcd.setCursor(4, 1);
  lcd.print(hr);
  LCDprintDigits(lcd, mins);
  LCDprintDigits(lcd, se);
}
*/
/*
void LCDprintDigits(LiquidCrystal l , int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  l.print(":");
  if (digits < 10)
    l.print('0');
  l.print(digits);
}
*/

void getYear() {
  lcd.clear();
  lcd.print("Year " + String(yr));
  if (digitalRead(numUp) == HIGH) {
    numUpPressed = true;
  }
  else {
    if (numUpPressed == true) {
      numUpPressed = false;
      yr++;
    }
  }
  if (digitalRead(confirm) == HIGH) {
    confirmPressed = true;
  }
  else {
    if (confirmPressed == true) {
      confirmPressed = false;
      yrSet = true;
    }
  }
}

void getMonth() {
  lcd.clear();
  lcd.print("Month " + String(mo));
  if (digitalRead(numUp) == HIGH) {
    numUpPressed = true;
  }
  else {
    if (numUpPressed == true) {
      numUpPressed = false;
      mo++;
      if (mo >= 13) {
        mo = 1;
      }
    }
  }
  if (digitalRead(confirm) == HIGH) {
    confirmPressed = true;
  }
  else {
    if (confirmPressed == true) {
      confirmPressed = false;
      monSet = true;
    }
  }
}

void getDay() {
  lcd.clear();
  lcd.print("Day " + String(da));
  if (digitalRead(numUp) == HIGH) {
    numUpPressed = true;
  }
  else {
    if (numUpPressed == true) {
      numUpPressed = false;
      da++;
      if (da >= 32) {
        da = 1;
      }
    }
  }
  if (digitalRead(confirm) == HIGH) {
    confirmPressed = true;
  }
  else {
    if (confirmPressed == true) {
      confirmPressed = false;
      daSet = true;
    }
  }
}

void getHour() {
  lcd.clear();
  lcd.print("Hour " + String(hr));
  if (digitalRead(numUp) == HIGH) {
    numUpPressed = true;
  }
  else {
    if (numUpPressed == true) {
      numUpPressed = false;
      hr++;
      if (hr >= 24) {
        hr = 0;
      }
    }
  }
  if (digitalRead(confirm) == HIGH) {
    confirmPressed = true;
  }
  else {
    if (confirmPressed == true) {
      confirmPressed = false;
      hrSet = true;
    }
  }
}

void getMinute() {
  lcd.clear();
  lcd.print("Minute " + String(mins));
  if (digitalRead(numUp) == HIGH) {
    numUpPressed = true;
  }
  else {
    if (numUpPressed == true) {
      numUpPressed = false;
      mins++;
      if (mins >= 60) {
        mins = 0;
      }
    }
  }
  if (digitalRead(confirm) == HIGH) {
    confirmPressed = true;
  }
  else {
    if (confirmPressed == true) {
      confirmPressed = false;
      minsSet = true;
    }
  }
}

void getSecond() {
  lcd.clear();
  lcd.print("Second " + String(se));
  if (digitalRead(numUp) == HIGH) {
    numUpPressed = true;
  }
  else {
    if (numUpPressed == true) {
      numUpPressed = false;
      se++;
      if (se >= 60) {
        se = 0;
      }
    }
  }
  if (digitalRead(confirm) == HIGH) {
    confirmPressed = true;
  }
  else {
    if (confirmPressed == true) {
      confirmPressed = false;
      secSet = true;
    }
  }
}


