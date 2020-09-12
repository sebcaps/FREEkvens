#include "temperature.h"

int getTemp(DallasTemperature sensor)
{
    sensor.requestTemperatures();
    //assuming there is only one sensor on the bus...
    Serial.print("Read Temp :");
    Serial.println(sensor.getTempCByIndex(0));
    return sensor.getTempCByIndex(0);
}