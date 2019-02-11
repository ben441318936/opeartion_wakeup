#include "SD.h" //SD library
#define SD_ChipSelectPin 10 //Select the SS pin for the SD module

#include "SPI.h"

#include "TMRpcm.h" //The library to play the player files

TMRpcm player; //Here you put the name you want

void setup() {

	Serial.begin(9600); //Initialize serial com

	if (!SD.begin(SD_ChipSelectPin)) { //If the SS pin is in a LOW state it will send a Fail message Serial.println("SD fail");
		return;
	}

	player.speakerPin = 9; //The pin where you will put the speaker, usually the 9

}

void loop(){

	player.setVolume(7); //You can set the volume here up to 7
	Serial.println("Volume set");
	player.quality(1); //only accepts 1 or 0, 1 is for better quality
	Serial.println("Quality set");
	Serial.println("Start playing");
	player.play("3.wav"); //Here you place the name of your player

	delay(70000); //This delay should be at least of the same lenght of your player,

	//This library can play the music while the arduino is in other task so you can play it background

	//or wait for the player to finish

}
