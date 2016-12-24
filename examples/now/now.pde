// now.pde
// last edit 24.Dec.2016  /klatoo  - forked from now.pde from SodaqMoja
//
// Using DateTime from RtcLib to send current date and time via serial interface
// 

#include "RtcLibHelper.h"
#include "DS3231.h"


// Date and time functions using RX8025 RTC connected via I2C and Wire lib

//#include <Wire.h>


char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

void setup () 
{
    Serial.begin(57600);
    //Wire.begin();
    rtc.begin();
}

long oldTime;

void loop () 
{
    DateTime now = rtc.now(); //get the current date-time
    long time = now.secondstime();

    if (oldTime == 0 || oldTime != time) {
	oldTime = time;

	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(' ');
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.print(' ');
	Serial.print(weekDay[now.dayOfTheWeek()]);
	Serial.println();
	Serial.print("Seconds since Unix Epoch: "); 
	Serial.print(time, DEC);
	Serial.println();
    }
    delay(1000);
}
