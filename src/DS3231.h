// DS3231.h
//
// last edit 24.Dec.2016 /klatoo  - created
//

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
//#include <stdint.h>


// These are the constants for periodicity of enableInterrupts() below.
#define EverySecond     0x01
#define EveryMinute     0x02
#define EveryHour       0x03


// RTC DS3231 chip connected via I2C and uses the Wire library.
// Only 24 Hour time format is supported in this implementation
class DS3231 {
public:
    uint8_t begin(void);

    void setDateTime(const DateTime& dt);  //Changes the date-time
    void setEpoch(uint32_t ts); // Set the RTC using timestamp (seconds since epoch)
    DateTime now();            //Gets the current date-time

    DateTime makeDateTime(unsigned long t);

    //Decides the /INT pin's output setting
    //periodicity can be any of following defines: EverySecond, EveryMinute, EveryHour 
    void enableInterrupts(uint8_t periodicity);
    void enableInterrupts(uint8_t hh24, uint8_t mm,uint8_t ss);
    void disableInterrupts();
    void clearINTStatus();

    void convertTemperature();
    float getTemperature();
private:
    uint8_t readRegister(uint8_t regaddress);
    void writeRegister(uint8_t regaddress, uint8_t value);
};

extern DS3231 rtc;

#endif