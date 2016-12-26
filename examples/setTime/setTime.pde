// setTime.pde
// last edit 26.Dec.2016  /klatoo  - forked from adjust.pde from SodaqMoja
//
// Sample how to set the current time
// 

#include "RtcLibHelper.h"
#include "DS3231.h"

DateTime compileTime(__DATE__, __TIME__);
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

// alternative would be:
// DateTime newTime(year, month, day, hour, min, sec);
// DateTime newTime(2016, 12,     26,    1,  44, 30);
// writing any non-existent time-data may interfere with normal operation of the RTC.


void setup () 
{
    Serial.begin(57600);
    rtc.begin();
    rtc.setDateTime(compileTime); // set date and time to time when preprocessor was run  
}

void loop () 
{
    DateTime now = rtc.now();   // get the current date-time
      
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
	Serial.print(now.unixtime());
	Serial.println();
	Serial.println();
    
    delay(1000);
}
