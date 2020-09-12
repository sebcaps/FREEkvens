#include "brightness.h"
#include <Arduino.h>
#define outMin 1022
#define outMax 900
#define inMin 0
#define inMax 1024
/*!
    @brief Compute brightness to apply to panel according to ambiant light
    use with linear rule to compute.
    Max analog value read is 1024 mean full ambiant light, mean dark
    Panel brightness is from 0 (full brigth) to 1024 (no light)
    Acceptable range is 750 --> 1023
*/
int computebrightness(int value)
{
    float a = ((float)outMin - (float)outMax) / ((float)inMin - (float)inMax);
    return (outMin + a * value);
}