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
//Patch Audio objects
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


//FastLED
#include "FastLED.h"
#define NUMPIXELS 28
#define LEDPIN 17


// Define the array of leds
CRGB leds[NUMPIXELS];

elapsedMillis msecs;  //keeps track of time

//Arrays to store variables from each input
float peak[4];      //Mapped values taken from the peak analyzer
float level[4];     //Post-fade level (applied to the leds)
float targLevel[4]; //Target for the fade



void setup() {
  Serial.begin(115200);

  //Setup audio boards
  AudioMemory(24);  // This should be enough memory for the audio processing
  audioShield1.setAddress(LOW);  // Set the address for board 1
  audioShield1.enable();         // Enable board 1
  audioShield1.volume(0.5);      // Set board 1 volume
  audioShield1.inputSelect(AUDIO_INPUT_LINEIN); // Select the right input for board 1
  audioShield2.setAddress(HIGH);  //Repeat steps for board 2
  audioShield2.enable();
  audioShield2.volume(0.5);
  audioShield2.inputSelect(AUDIO_INPUT_LINEIN);
  
  //Start LEDs
  FastLED.addLeds<NEOPIXEL, LEDPIN>(leds, NUMPIXELS);

}



void loop() {

   if (msecs > 40) {  // do every 40 milliseconds
    
    // If the peak analysis is available for all channels
    if (peak1.available() && peak2.available() && peak3.available() && peak4.available()) {
      msecs = 0;  // reset the timer
      
      // read analysers
      peak[0] = peak1.read() * NUMPIXELS;
      peak[1] = peak2.read() * NUMPIXELS;
      peak[2] = peak3.read() * NUMPIXELS;
      peak[3] = peak4.read() * NUMPIXELS;

      // Set target levels to the peaks
      for(int i=0; i<4; i++){
        targLevel[i] = peak[i];
      }
      printLevels();  // Prints the levels to serial monitor
    }
  }

  // apply fades to all the levels
  for(int i=0; i<4; i++){
    level[i] = fade(level[i], targLevel[i], 0.1);
  }

  // Show levels on LEDs
  for(int i=0; i<NUMPIXELS; i++){
    leds[i] = CRGB::Black;  // Set all leds to black
    
    if(i < level[0]-1) leds[i] += CRGB::Red;  // Make pixels lower than level0 red
    if(i < level[1]-1) leds[i] += CRGB::Blue; // Make pixels lower than level1 blue
    if(i >= NUMPIXELS-level[2]) leds[i] += CRGB::Red;   // pixels higher than level2 red (inverted)
    if(i >= NUMPIXELS-level[3]) leds[i] += CRGB::Green; // pixels higher than level2 green (inverted)
  }
  FastLED.show();    // Render LEDs
}


//--------------------------------------------------------------------------
// print levels
void printLevels(){
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
}


//--------------------------------------------------------------------------

//fade function
float fade(float current, float target, float amount){
  float result;
  if(current < target){              // if the current value is less than the target
    result = current + abs(amount);  // add the amount
    if(result > target){             // Limit the result
      result = target;
    }
  }
  else if(current > target){         // if the current value is greater than the target
    result = current - abs(amount);  // subract the amount
    if(result < target){             // limit the result
      result = target;
    }
  }
  else if(current == target){
    result = target;
  }
  return result;
}
