// interrupts.pde
// last edit 26.Dec.2016  /klatoo  - forked from interrupts.pde from SodaqMoja
//
// Using interrupts with the DS3231 class
//
// Please make sure that you connected pin 2 (INT0) of Arduino Uno to SQW pin of DS3231
// otherwise you will not be able to see the interrupt

#include "RtcLibHelper.h"
#include "DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
long oldTime;

void setup () 
{
     // Initialize INT0 for accepting interrupts, check if pin is available on your board 
     PORTD |= 0x04; 
     DDRD &=~ 0x04;
  
     Serial.begin(57600);
        
     rtc.begin();
     attachInterrupt(digitalPinToInterrupt(2), timerISR, FALLING);   // check if pin is available on your board 
     
     // Enable periodic interrupt (possible values are: DS3231::Periodicity::EverySecond, EveryMinute, EveryHour)
     rtc.enableInterrupts(DS3231::Periodicity::EveryMinute); 
     
     // it is also possible to fire an interrupt on a certain time, e.g. for an alarm clock
     //rtc.enableInterrupts(14,35,0);    // interrupt at (h,m,s)
}


void loop () 
{

    DateTime now = rtc.now();   // get the current date-time
    long time = now.secondstime();

    if (oldTime == 0 || oldTime != time) 
	{
		oldTime = time;

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
    }
    rtc.clearINTStatus();
 
} 

  
//Interrupt service routine for external interrupt on INT0 pin conntected to /INT
void timerISR()
{
  //Keep this as short as possible. Possibly avoid using function calls
  
   Serial.println("*** External Interrupt detected ***");
}
