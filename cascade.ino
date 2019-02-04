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
},
{
  B00011000,
  B00100101,
  B00000100,
  B00000100,
  B00001000,
  B00010000,
  B00100001,
  B00111100
},
{
  B11000001,
  B00101010,
  B00101010,
  B11000010,
  B00100010,
  B00101010,
  B00101010,
  B11000001
},
{
  B10000010,
  B01000110,
  B01001010,
  B01010010,
  B01011110,
  B01000010,
  B01000010,
  B10000010
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
unsigned long delaytime=500;

/* 
 This time we have more than one device. 
 But all of them have to be initialized 
 individually.
 */
void setup() {
  //we have already set the number of devices when we created the LedControl
  int devices=lc.getDeviceCount();
  //we have to init all devices in a loop
  for(int address=0;address<devices;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,2);
    /* and clear the display */
    lc.clearDisplay(address);
  }
  rtc.begin();
  rtc.setDate(1,1,2018);
  rtc.setTime(12,0,0);
}

void loop() { 
  //read the number cascaded devices
  int devices=lc.getDeviceCount();
  
  //we have to init all devices in a loop
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      for(int address=0;address<devices;address++) {
        delay(delaytime);
        lc.setLed(address,row,col,true);
        delay(delaytime);
        lc.setLed(address,row,col,false);
      }
    }
  }
}

void construct_display_chars(Time t) {
  if (t.hour < 12) {
    
  }
}