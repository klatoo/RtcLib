// adjust.pde
// last edit 24.Dec.2016  /klatoo  - forked from adjust.pde from SodaqMoja
//
// Date and time functions using the RtcLib
// 

#include "RtcLibHelper.h"
#include "DS3231.h"

//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
DateTime dt(2011, 11, 10, 15, 18, 0, 5);


void setup () 
{
    Serial.begin(57600);
    rtc.begin();
    rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
}

void loop () 
{
    DateTime now = rtc.now(); //get the current date-time
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.date(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.print(weekDay[now.dayOfWeek()]);
    Serial.println();
    delay(1000);
}
