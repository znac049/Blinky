#include <Arduino.h>
#include "FastLED.h"
#include "SmartLED.h"

#define NUM_LEDS 150
#define DATA_PIN 32

CRGB leds[NUM_LEDS];
SmartLED smartLeds[NUM_LEDS];

static const CRGB colours[] = {
  CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red, 
  CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::White, 
  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue
};

int getColour(CRGB colour) {
  return 0x10000000 | colour.r<<16 | colour.g<<8 | colour.b;
}

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  //FastLED.addLeds<WS2812B, DATA_PIN, BRG>(leds, NUM_LEDS);
  Serial.begin(115200);

  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = colours[i%15];
    smartLeds[i].step(i*5);
  }
}

int setLED(int offset, CRGB colour) {
  offset = offset%NUM_LEDS;
  leds[offset] = colour;

  return offset;
}


void simple() 
{
  while (1) {
    for (int i=0; i<15; i++) {
      for (int j=0; j<NUM_LEDS; j++) {
        leds[j] = colours[(i+j)%15];
      }

      FastLED.show();
      delay(250);
    }
  }
}

void jubilee() 
{
  for (int i=0; i<NUM_LEDS; i++) {
    smartLeds[i].step();
    leds[i] = smartLeds[i].getCurrent();
  }

  FastLED.show();
  delay(20);
}

void loop() 
{
  jubilee();
  Serial.print(".");
}

