/* Living Arts Collaborative Community Project 2016-17
 * Srishti Gupta - Project Manager
 * Amanda Taylor - Original Concept
 * Kevin Wang - Programming
 * Elijah Blackamore - Hardware
 * Steve Xu - Swag
 * Roger Teng - Swag */

#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <time.h>

/* HARDWARE INSTRUCTIONS:
 *  
 *  Adafruit Neopixel has four wires:
 *  1. DIM
 *  2. 5V (5 Volt)
 *  3. GND (Ground) x2
 *  
 *  1. DIM goes into one of the Arduino's numbered pins, which must be defined
 *  in the code.
 *  2. 5V goes into the 5V pin on the Arduino.
 *  3. GND goes into the GND pins.
 *  
 *  When using the breadboard (which is necessary for utilizing multiple
 *  strips,) be aware of the breadboard's wiring (pictured below.) Red and blue
 *  columns are all part of the same strips, whereas the rows of five are each
 *  strips of their own.
 *  http://1.bp.blogspot.com/-T6PVoBeO8eE/UPRzck7HbFI/AAAAAAAAAFs/gpek2YMpmbk/s1600/breadboard_90.png
 * 
 */

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define ADC_CHANNEL 0
#define LEFT_LED_PIN 6
#define RIGHT_LED_PIN 7

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(36, LEFT_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(36, RIGHT_LED_PIN, NEO_GRB + NEO_KHZ800);

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

const int PASSIVE_MAX = 50;

int r = PASSIVE_MAX;
int g = 0;
int b = 0;
int phase = 0;

void setup() {

  Serial.begin(9600);

  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show();
  
}

void loop() {
  double soundLevel = getVolume();
  if(soundLevel > 0.25){
    active(80, soundLevel * 200);
  } else {
    passive(20);
  }

}

void passive(int spd){
  const int STEP = 1;
    showAll();
    delay(spd);
    switch(phase % 6){
      //red to orange to yellow
      case 0:
      g += STEP;
      if(g >= PASSIVE_MAX){
        ++phase;
      }
      break;
      // yellow to green
      case 1:
      r -= STEP;
      if(r <= 0){
        ++phase;
      }
      break;
      // green to cyan
      case 2:
      b += STEP;
      if(b >= PASSIVE_MAX){
        ++phase;
      }
      break;
      // cyan to blue
      case 3:
      g -= STEP;
      if(g <= 0){
        ++phase;
      }
      break;
      // blue to purple
      case 4:
      r += STEP;
      if(r >= PASSIVE_MAX){
        ++phase;
      }
      break;
      // purple to red
      case 5:
      b -= STEP;
      if(b <= 0){
        ++phase;
      }
      break;
    }
}

// blue: 0, 0, 255
// maize: 255, 128, 0
void active(int spd, int soundLevel){
  for (int i = 0; i <= strip1.numPixels(); i++){
    strip1.setPixelColor(i, 0, 0, soundLevel);
    strip2.setPixelColor(i, soundLevel, soundLevel / 2, 0);
  }
  strip1.show();
  strip2.show();
  delay(spd);
}

void showAll(){
  for (int i = 0; i <= strip1.numPixels(); i++){
    strip1.setPixelColor(i, r, g, b);
    strip2.setPixelColor(i, g, b, r);
  }
  strip1.show();
  strip2.show();
}

double getVolume(){
     unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
 
   return volts;
}
