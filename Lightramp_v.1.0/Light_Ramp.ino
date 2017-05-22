//Light Ramp play

//Audio code start

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=216,262
AudioAnalyzePeak         peak1;          //xy=462,344
AudioAnalyzePeak         peak2;          //xy=462,384
AudioOutputI2S           i2s1;           //xy=472,283
AudioConnection          patchCord1(i2s2, 0, i2s1, 0);
AudioConnection          patchCord2(i2s2, 0, i2s1, 1);
AudioConnection          patchCord3(i2s2, 0, peak1, 0);
AudioConnection          patchCord4(i2s2, 1, peak2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=325,426
// GUItool: end automatically generated code

//Audio code end



#include "FastLED.h"

#define NUMPIXELS 28

#define DATA_PIN 3

// 1 sec. frequency
unsigned long interval=1000;    // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.

// Define the array of leds
CRGB leds[NUMPIXELS];

void setup() {
  Serial.begin(115200);
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(36);
  
  usbMIDI.setHandleNoteOn(OnNoteOn);
//  usbMIDI.setHandleControlChange(OnControlChange);
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUMPIXELS);

}

elapsedMillis msecs;

void loop() {
  midi(); 

   if (msecs > 40) {
    if (peak1.available() && peak2.available()) {
      msecs = 0;
      float leftNumber = peak1.read();
      float rightNumber = peak2.read();
      int leftPeak = leftNumber * 30.0;
      int rightPeak = rightNumber * 30.0;
      int count;
      for (count=0; count < 30-leftPeak; count++) {
        Serial.print(" ");
      }
      while (count++ < 30) {
        Serial.print("<");
      }
      Serial.print("||");
      for (count=0; count < rightPeak; count++) {
        Serial.print(">");
      }
      while (count++ < 30) {
        Serial.print(" ");
      }
      Serial.print(leftNumber);
      Serial.print(", ");
      Serial.print(rightNumber);
      Serial.println();
    }
  }
}

void changeToRed() {
  // Turn the LEDs on
    for (int i = 0; i < NUMPIXELS; i++) {
      leds[i] = CRGB::Red;
    }
   FastLED.show();
}

void changeToGreen() {
  // Turn the LEDs on, then pause
    for (int i = 0; i < NUMPIXELS; i++) {
      leds[i] = CRGB::Green;
    }
   FastLED.show();
}

void changeToBlue() {
  // Turn the LEDs on, then pause
    for (int i = 0; i < NUMPIXELS; i++) {
      leds[i] = CRGB::Blue;
    }
   FastLED.show();
}



