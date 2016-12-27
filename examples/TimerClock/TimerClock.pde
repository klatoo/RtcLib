// timerClock.pde
// last edit 27.Dec.2016  /klatoo  - created
//
// sample application to show usage of TimerClock24
// 

#include "RtcLibHelper.h"
#include "DS3231.h"


char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
TimerClock24h timerClock;


void setup () 
{
    Serial.begin(57600);
    rtc.begin();
	timerClock.setup(15,15,0,15,18,0); // switching on at 15:15:00 and off at 15:18:00
}


void loop () 
{
    DateTime now = rtc.now();   // get the current date-time
    long time = now.secondstime();

	Serial.print(weekDay[now.dayOfWeek()]);
	Serial.print(' ');
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
	Serial.println(now.unixtime());
	if(timerClock.IsOn(now))
	   Serial.println("******* ON *******");
    else
	   Serial.println("------- OFF -------");
		
	Serial.println();

    delay(1000);
}
