#ifndef _SMARTLED_H_
#define _SMARTLED_H_

#include "FastLED.h"

class SmartLED {
private:
    enum LED_State {Solid_Blue, Solid_Red, Solid_White, Fading_To_Red, Fading_To_White, Fading_To_Blue};

    CRGB current;
    CRGB target;
    LED_State state;
    int stepNum;
    const int stepsPerState = 25;

public:
    SmartLED();
    void step();
    void step(int steps);

    CRGB getCurrent();

private:
    void nextState();
    void fade();
};

#endif