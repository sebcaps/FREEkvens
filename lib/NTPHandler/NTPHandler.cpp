#include "NTPHandler.h"
#include <Arduino.h>

time_t customNtp()
{
    currentNow = time(nullptr);
    return currentNow;
}
