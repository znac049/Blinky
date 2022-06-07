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
  for (int i=0; i<15; i++) {
    for (int j=0; j<NUM_LEDS; j++) {
      leds[j] = colours[(i+j)%15];
    }

    FastLED.show();
    delay(250);
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

/*
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  leds.setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     leds.setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     leds.show();
     delay(SpeedDelay);
     if(OnlyOne) {
       leds.setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  leds.setPixel(Pixel,red,green,blue);
  leds.show();
  delay(SpeedDelay);
  leds.setPixel(Pixel,0,0,0);
}

byte *Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  leds.setAll(red,green,blue);
 
  int Pixel = random(NUM_LEDS);
  leds.setPixel(Pixel,0xff,0xff,0xff);
  leds.show();
  delay(SparkleDelay);
  leds.setPixel(Pixel,red,green,blue);
  leds.show();
  delay(SpeedDelay);
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      leds.setPixel(i, *c, *(c+1), *(c+2));
    }
    leds.show();
    delay(SpeedDelay);
  }
}

void theaterChaseRainbow(int SpeedDelay) {
  byte *c;
 
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < NUM_LEDS; i=i+3) {
          c = Wheel( (i+j) % 255);
          leds.setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        leds.show();
       
        delay(SpeedDelay);
       
        for (int i=0; i < NUM_LEDS; i=i+3) {
          leds.setPixel(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}

void BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;
 
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
 
  for (int i = 0 ; i < BallCount ; i++) {  
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
 
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
 
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
 
    for (int i = 0 ; i < BallCount ; i++) {
      leds.setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    }
   
    leds.show();
    leds.setAll(0,0,0);
  }
}
*/