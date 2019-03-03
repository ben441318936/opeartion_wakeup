#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3); // RX, TX

char data = '0';
bool LEDState = HIGH;

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,LEDState);
  Serial.begin(9600);
  mySerial.begin(9600);
  establishContact();
  LEDState = LOW;
  digitalWrite(13,LEDState);
}

void loop() {
  if (Serial.available()) {
    if (Serial.read() == '0') {
      LEDState = LOW;
      mySerial.write('0');
    }
    else if (Serial.read() == '1') {
      LEDState = HIGH;
      mySerial.write('1');
    }
  }
  digitalWrite(13,LEDState);
  delay(10);
}

void establishContact(){
  while(mySerial.available()<=0){
    Serial.println("Waiting for client connect request");
    delay(500);
  }
  Serial.println("Connection request received");
  mySerial.write('R');
}
