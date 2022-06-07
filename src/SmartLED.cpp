#include "SmartLED.h"

SmartLED::SmartLED() 
{
    state = Solid_Blue;
    current = CRGB::Blue;
    stepNum = 0;
}

CRGB SmartLED::getCurrent() 
{
    return current;
}

void SmartLED::nextState() 
{
    switch (state) {
        case Solid_Blue:
            state = Fading_To_Red;
            target = CRGB::Red;
            break;

        case Solid_White:
            state = Fading_To_Blue;
            target = CRGB::Blue;
            break;

        case Solid_Red:
            state = Fading_To_White;
            target = CRGB::White;
            break;

        case Fading_To_Blue:
            state = Solid_Blue;
            current = CRGB::Blue;
            break;

        case Fading_To_White:
            state = Solid_White;
            current = CRGB::White;
            break;

        case Fading_To_Red:
            state = Solid_Red;
            current = CRGB::Red;
            break;

        default:
            state = Solid_Blue;
            stepNum = 0;
            break;
    }

    stepNum = 0;
}

void SmartLED::step(int numSteps)
{
    for (int i=0; i<numSteps; i++) {
        step();
    }
}

void SmartLED::step()
{
    stepNum++;

    if (stepNum >= stepsPerState) {
        stepNum = 0;

        nextState();
    }

    switch (state) {
        case Solid_Blue:
        case Solid_White:
        case Solid_Red:
            break;

        case Fading_To_Blue:
        case Fading_To_White:
        case Fading_To_Red:
            fade();
            break;

        default:
            break;
    }
}

void SmartLED::fade() 
{
    CRGB nextColour;
    int delta;
    int increment;
    
    if (current.r != target.r) {
        delta = target.r - current.r;
        increment = delta / (stepsPerState - stepNum);
        current.r += increment;
    }
    
    if (current.g != target.g) {
        delta = target.g - current.g;
        increment = delta / (stepsPerState - stepNum);
        current.g += increment;
    }
    
    if (current.b != target.b) {
        delta = target.b - current.b;
        increment = delta / (stepsPerState - stepNum);
        current.b += increment;
    }
}