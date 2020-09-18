#include "NTPHandler.h"
#include <Arduino.h>

time_t utc = now();
time_t currentNow=now();

time_t customNtp()
{
    Serial.println("CALL TO CUSTOM NTP");
    currentNow = time(nullptr);
    Serial.println(localtime(&currentNow)->tm_hour);
    Serial.print("In custom NTP localtime MIN:");
    Serial.println(localtime(&currentNow)->tm_min);
    return currentNow;
}
