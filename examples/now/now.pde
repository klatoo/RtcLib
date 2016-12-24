// now.pde
// last edit 25.Dec.2016  /klatoo  - forked from now.pde from SodaqMoja
//
// sample application using DateTime from RtcLib to send current 
// date and time via serial interface
// 

#include "RtcLibHelper.h"
#include "DS3231.h"


char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
long oldTime;


void setup () 
{
    Serial.begin(57600);
    rtc.begin();
}


void loop () 
{
    DateTime now = rtc.now();   // get the current date-time
    long time = now.secondstime();

    if (oldTime == 0 || oldTime != time) 
	{
		oldTime = time;

		Serial.print(weekDay[now.dayOfWeek()]);

		Serial.print(now.day(), DEC);
		Serial.print('.');
		Serial.print(now.month(), DEC);
		Serial.print('.');
		Serial.print(now.year(), DEC);
		
		Serial.print("    ");
		
		Serial.print(now.hour(), DEC);
		Serial.print(':');
		Serial.print(now.minute(), DEC);
		Serial.print(':');
		Serial.println(now.second(), DEC);
				
		Serial.print("unix-time: "); 
		Serial.print(now.unixtime());
		Serial.println();
    }
    delay(1000);
}
