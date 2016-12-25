// DS3231.h
//
// last edit 24.Dec.2016 /klatoo  - created
//           25.Dec.2016 /klatto  - moved some defines to const and enum

// DS3231 - class to support DS3231 real time clock e.g. from
// DS3231 class is a modified version of Sodaq_DS3231 Class, 
// what was derived from DS3231 Class is by Seeed Technology Inc
// (http://www.seeedstudio.com) and used in Seeeduino Stalker v2.1 
// for battery management(MCU power saving mode)
// & to generate timestamp for data logging. 


#ifndef DS3231_H
#define DS3231_H

#include <Arduino.h>
#include "RtcLibHelper.h"


// RTC DS3231 chip connected via I2C and uses the Wire library.
// Only 24 Hour time format is supported in this implementation
class DS3231 {
public:
	enum class Periodicity: uint8_t  { EverySecond=0x01, EveryMinute=0x02, EveryHour=0x03}; // used for periodicity of enableInterrupts() below.
	const unsigned long EPOCH_TIME_OFF = 946684800;   // This is 2000-jan-01 00:00:00 in epoch time
    uint8_t begin(void);

    void setDateTime(const DateTime& dt);  //Changes the date-time
    void setEpoch(uint32_t ts); // Set the RTC using timestamp (seconds since epoch)
    DateTime now();            //Gets the current date-time

    DateTime makeDateTime(unsigned long t);

    //Decides the /INT pin's output setting
    //periodicity can be any of following defines: EverySecond, EveryMinute, EveryHour 
    void enableInterrupts(periodicity period);
    void enableInterrupts(uint8_t hh24, uint8_t mm,uint8_t ss);
    void disableInterrupts();
    void clearINTStatus();

    void convertTemperature();
    float getTemperature();

private:
    uint8_t readRegister(uint8_t regaddress);
    void writeRegister(uint8_t regaddress, uint8_t value);
	static uint8_t bcd2bin(uint8_t val);
	static uint8_t bin2bcd(uint8_t val); 

};

extern DS3231 rtc;

#endif
