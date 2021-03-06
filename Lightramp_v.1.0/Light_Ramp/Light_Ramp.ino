/* Light Ramp play
 * By Hubbub: Braeden Foster, Reuben Poharama & Martin Hill
 */

#define FASTLED_ALLOW_INTERRUPTS 0  // THIS IS NEEDED!!!

//Audio code start

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2SQuad        i2s_quad1;      //xy=233,261
AudioAnalyzePeak         peak1;          //xy=407,219
AudioAnalyzePeak         peak2;          //xy=413,259
AudioAnalyzePeak         peak3;          //xy=422,299
AudioAnalyzePeak         peak4;          //xy=430,335
AudioConnection          patchCord1(i2s_quad1, 0, peak1, 0);
AudioConnection          patchCord2(i2s_quad1, 1, peak2, 0);
AudioConnection          patchCord3(i2s_quad1, 2, peak3, 0);
AudioConnection          patchCord4(i2s_quad1, 3, peak4, 0);
AudioControlSGTL5000     audioShield1;     //xy=234,178
AudioControlSGTL5000     audioShield2;     //xy=234,178
// GUItool: end automatically generated code


/*
// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=176,262
AudioAnalyzePeak         peak1;          //xy=407,219
AudioAnalyzePeak         peak2;          //xy=413,259
AudioAnalyzePeak         peak3;          //xy=422,299
AudioAnalyzePeak         peak4;          //xy=430,335
AudioConnection          patchCord1(i2s1, 0, peak1, 0);
AudioConnection          patchCord2(i2s1, 0, peak2, 0);
AudioConnection          patchCord3(i2s1, 1, peak3, 0);
AudioConnection          patchCord4(i2s1, 1, peak4, 0);
AudioControlSGTL5000     audioShield1;     //xy=234,178
// GUItool: end automatically generated code
*/



#include "FastLED.h"

#define NUMPIXELS 28

#define DATA_PIN 17

// 1 sec. frequency
unsigned long interval=1000;    // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.

// Define the array of leds
CRGB leds[NUMPIXELS];

void setup() {
  Serial.begin(115200);
  AudioMemory(24);
  audioShield1.setAddress(LOW);
  audioShield1.enable();
  audioShield1.volume(0.5);
  audioShield1.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield2.setAddress(HIGH);
  audioShield2.enable();
  audioShield2.volume(0.5);
  audioShield2.inputSelect(AUDIO_INPUT_LINEIN);
  
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUMPIXELS);

}

elapsedMillis msecs;

float peak[4];
float level[4];
float targLevel[4];

void loop() {

   if (msecs > 40) {
    if (peak1.available() && peak2.available() && peak3.available() && peak4.available()) {
      msecs = 0;
      peak[0] = peak1.read() * NUMPIXELS;
      peak[1] = peak2.read() * NUMPIXELS;
      peak[2] = peak3.read() * NUMPIXELS;
      peak[3] = peak4.read() * NUMPIXELS;

      int count;
      for (count=0; count < NUMPIXELS-peak[0]; count++) {
        Serial.print(" ");
      }
      while (count++ < NUMPIXELS) {
        Serial.print("<");
      }
      Serial.print("||");
      for (count=0; count < peak[1]; count++) {
        Serial.print(">");
      }
      while (count++ < NUMPIXELS) {
        Serial.print(" ");
      }


      for (count=0; count < NUMPIXELS-peak[2]; count++) {
        Serial.print(" ");
      }
      while (count++ < NUMPIXELS) {
        Serial.print("<");
      }
      Serial.print("||");
      for (count=0; count < peak[3]; count++) {
        Serial.print(">");
      }
      while (count++ < NUMPIXELS) {
        Serial.print(" ");
      }
      Serial.println();


      
      for(int i=0; i<4; i++){
        targLevel[i] = peak[i];
      }
     
    }
  }

  for(int i=0; i<4; i++){
    level[i] = fade(level[i], targLevel[i], 0.1);
  }
  
  for(int i=0; i<NUMPIXELS; i++){
    leds[i] = CRGB::Black;
    if(i <= level[0]) leds[i] += CRGB::Red;  // if pixel is lower than level1
    if(i <= level[1]) leds[i] += CRGB::Blue;
    if(i >= NUMPIXELS-level[2]) leds[i] += CRGB::Red;
    if(i >= NUMPIXELS-level[3]) leds[i] += CRGB::Green;
  }
  FastLED.show();
}


//--------------------------------------------------------------------------


//fade function
float fade(float current, float target, float amount){
  float result;
  if(current < target){           // if the current value is less than the target
    result = current + abs(amount);    // add the amount
    if(result > target){          // 
      result = target;
    }
  }
  else if(current > target){
    result = current - abs(amount);
    if(result < target){
      result = target;
    }
  }
  else if(current == target){
    result = target;
  }
  return result;
}
