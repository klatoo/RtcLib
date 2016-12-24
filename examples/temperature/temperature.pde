// temperature.pde
// last edit 25.Dec.2016  /klatoo  - forked from now.pde from SodaqMoja
//
// sample application readomg the temperature from the DS3231 chip 
// 

#include "RtcLibHelper.h"
#include "DS3231.h"


void setup () 
{
    Serial.begin(57600);
    rtc.begin();
}

void loop () 
{
    rtc.convertTemperature();             // convert current temperature into registers
    Serial.print(rtc.getTemperature());   // read registers and display the temperature
    Serial.println(" °C");
    delay(1000);
}
