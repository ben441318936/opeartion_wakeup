#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3); // RX, TX

char data = '0';
bool LEDState = HIGH;

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,LEDState);
  mySerial.begin(9600);
  establishContact();
  LEDState = LOW;
  digitalWrite(13,LEDState);
}

void loop() {
  if (mySerial.available()) {
    data = mySerial.read();
    if (data == 's') {
      LEDState = LOW;
    }
    else if (data == 'a') {
      LEDState = HIGH;
    }
    digitalWrite(13,LEDState);
  }
  delay(10);
}

void establishContact(){
  while(mySerial.available()<=0){
    mySerial.println("R");
    delay(500);
  }
}
