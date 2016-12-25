// DS3231.cpp
//
// last edit 24.Dec.2016 /klatoo  - created
//

// 
// Class DS3231 is derived from Sodaq_DS3231 Class, that is a modified version of
// DS3231 Class is by Seeed Technology Inc(http://www.seeedstudio.com) and used
// in Seeeduino Stalker v2.1 for battery management(MCU power saving mode)
// & to generate timestamp for data logging. DateTime Class is a modified
// version supporting day-of-week.

#include "DS3231.h"
#include <Wire.h>


#define DS3231_ADDRESS	      0x68 //I2C Slave address

/* DS3231 Registers. Refer Sec 8.2 of application manual */
#define DS3231_SEC_REG        0x00  
#define DS3231_MIN_REG        0x01  
#define DS3231_HOUR_REG       0x02
#define DS3231_WDAY_REG       0x03
#define DS3231_MDAY_REG       0x04
#define DS3231_MONTH_REG      0x05
#define DS3231_YEAR_REG       0x06

#define DS3231_AL1SEC_REG     0x07
#define DS3231_AL1MIN_REG     0x08
#define DS3231_AL1HOUR_REG    0x09
#define DS3231_AL1WDAY_REG    0x0A

#define DS3231_AL2MIN_REG     0x0B
#define DS3231_AL2HOUR_REG    0x0C
#define DS3231_AL2WDAY_REG    0x0D

#define DS3231_CONTROL_REG          0x0E
#define DS3231_STATUS_REG           0x0F
#define DS3231_AGING_OFFSET_REG     0x0F
#define DS3231_TMP_UP_REG           0x11
#define DS3231_TMP_LOW_REG          0x12

////////////////////////////////////////////////////////////////////////////////
// RTC DS3231 implementation

static uint8_t DS3231::bcd2bin(uint8_t val) 
{ 
	return val - 6 * (val >> 4); 
}

static uint8_t DS3231::bin2bcd(uint8_t val) 
{ 
	return val + 6 * (val / 10); 
}


uint8_t DS3231::readRegister(uint8_t regaddress)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write((byte)regaddress);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDRESS, 1);
    return Wire.read();
}

void DS3231::writeRegister(uint8_t regaddress,uint8_t value)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write((byte)regaddress);
    Wire.write((byte)value);
    Wire.endTransmission();
}

uint8_t DS3231::begin(void) {

  unsigned char ctReg=0;
  
  Wire.begin();
  ctReg |= 0b00011100; 
  writeRegister(DS3231_CONTROL_REG, ctReg);     //CONTROL Register Address
  delay(10);

  // set the clock to 24hr format  
  uint8_t hrReg = readRegister(DS3231_HOUR_REG);
  hrReg &= 0b10111111;
  writeRegister(DS3231_HOUR_REG, hrReg);       

  delay(10);

  return 1; 
}

//set the time-date specified in DateTime format
//writing any non-existent time-data may interfere with normal operation of the RTC
void DS3231::setDateTime(const DateTime& dt) {

  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write((byte)DS3231_SEC_REG);  //beginning from SEC Register address

  Wire.write((byte)bin2bcd(dt.second())); 
  Wire.write((byte)bin2bcd(dt.minute()));
  Wire.write((byte)bin2bcd((dt.hour()) & 0b10111111)); //Make sure clock is still 24 Hour
  Wire.write((byte)dt.dayOfWeek());
  Wire.write((byte)bin2bcd(dt.day()));
  Wire.write((byte)bin2bcd(dt.month()));
  Wire.write((byte)bin2bcd(dt.year() - 2000));  
  Wire.endTransmission();

}

DateTime DS3231::makeDateTime(unsigned long t)
{
  if (t < EPOCH_TIME_OFF)
    return DateTime(0);
  return DateTime(t - EPOCH_TIME_OFF);
}

// Set the RTC using timestamp (seconds since epoch)
void DS3231::setEpoch(uint32_t ts)
{
  setDateTime(makeDateTime(ts));
}

//Read the current time-date and return it in DateTime format
DateTime DS3231::now() {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write((byte)0x00);	
  Wire.endTransmission();
  
  Wire.requestFrom(DS3231_ADDRESS, 8);
  uint8_t ss = bcd2bin(Wire.read());
  uint8_t mm = bcd2bin(Wire.read());
   
  uint8_t hrreg = Wire.read();
  uint8_t hh = bcd2bin((hrreg & ~0b11000000)); //Ignore 24 Hour bit

  uint8_t wd =  Wire.read();
  uint8_t d = bcd2bin(Wire.read());
  uint8_t m = bcd2bin(Wire.read());
  uint16_t y = bcd2bin(Wire.read()) + 2000;
  
  return DateTime (y, m, d, hh, mm, ss);
}

//Enable periodic interrupt at /INT pin. Supports only the level interrupt
//for consistency with other /INT interrupts. All interrupts works like single-shot counter
//Use refreshINTA() to re-enable interrupt.
void DS3231::enableInterrupts(periodicity period)
{

    unsigned char ctReg=0;
    ctReg |= 0b00011101; 
    writeRegister(DS3231_CONTROL_REG, ctReg);     //CONTROL Register Address
    
   switch(period) 
   {
       case EverySecond:
       writeRegister(DS3231_AL1SEC_REG,  0b10000000 ); //set AM1
       writeRegister(DS3231_AL1MIN_REG,  0b10000000 ); //set AM2
       writeRegister(DS3231_AL1HOUR_REG, 0b10000000 ); //set AM3
       writeRegister(DS3231_AL1WDAY_REG, 0b10000000 ); //set AM4

       break;

       case EveryMinute:
       writeRegister(DS3231_AL1SEC_REG,  0b00000000 ); //Clr AM1
       writeRegister(DS3231_AL1MIN_REG,  0b10000000 ); //set AM2
       writeRegister(DS3231_AL1HOUR_REG, 0b10000000 ); //set AM3
       writeRegister(DS3231_AL1WDAY_REG, 0b10000000 ); //set AM4

       break;

       case EveryHour:
       writeRegister(DS3231_AL1SEC_REG,  0b00000000 ); //Clr AM1
       writeRegister(DS3231_AL1MIN_REG,  0b00000000 ); //Clr AM2
       writeRegister(DS3231_AL1HOUR_REG, 0b10000000 ); //Set AM3
       writeRegister(DS3231_AL1WDAY_REG, 0b10000000 ); //set AM4

       break;
   }
}

//Enable HH/MM/SS interrupt on /INTA pin. All interrupts works like single-shot counter
void DS3231::enableInterrupts(uint8_t hh24, uint8_t mm, uint8_t ss)
{
    unsigned char ctReg=0;
    ctReg |= 0b00011101; 
    writeRegister(DS3231_CONTROL_REG, ctReg);     //CONTROL Register Address

    writeRegister(DS3231_AL1SEC_REG,  0b00000000 | bin2bcd(ss) ); //Clr AM1
    writeRegister(DS3231_AL1MIN_REG,  0b00000000 | bin2bcd(mm)); //Clr AM2
    writeRegister(DS3231_AL1HOUR_REG, (0b00000000 | (bin2bcd(hh24) & 0b10111111))); //Clr AM3
    writeRegister(DS3231_AL1WDAY_REG, 0b10000000 ); //set AM4
}

//Disable Interrupts. This is equivalent to begin() method.
void DS3231::disableInterrupts()
{
    begin(); //Restore to initial value.
}

//Clears the interrrupt flag in status register. 
//This is equivalent to preparing the DS3231 /INT pin to high for MCU to get ready for recognizing the next INT0 interrupt
void DS3231::clearINTStatus()
{
    // Clear interrupt flag 
    uint8_t statusReg = readRegister(DS3231_STATUS_REG);
    statusReg &= 0b11111110;
    writeRegister(DS3231_STATUS_REG, statusReg);

}

//force temperature sampling and converting to registers. If this function is not used the temperature is sampled once 64 Sec.
void DS3231::convertTemperature()
{
    // Set CONV 
    uint8_t ctReg = readRegister(DS3231_CONTROL_REG);
    ctReg |= 0b00100000; 
    writeRegister(DS3231_CONTROL_REG,ctReg); 
 

    //wait until CONV is cleared. Indicates new temperature value is available in register.
    do
    {
       //do nothing
    } while ((readRegister(DS3231_CONTROL_REG) & 0b00100000) == 0b00100000 ); 
 
}

//Read the temperature value from the register and convert it into float (deg C)
float DS3231::getTemperature()
{
    float fTemperatureCelsius;
    uint8_t tUBYTE  = readRegister(DS3231_TMP_UP_REG);  //Two's complement form
    uint8_t tLRBYTE = readRegister(DS3231_TMP_LOW_REG); //Fractional part
  
    if(tUBYTE & 0b10000000) //check if -ve number
    {
       tUBYTE  ^= 0b11111111;  
       tUBYTE  += 0x1;
       fTemperatureCelsius = tUBYTE + ((tLRBYTE >> 6) * 0.25);
       fTemperatureCelsius = fTemperatureCelsius * -1;
    }
    else
    {
        fTemperatureCelsius = tUBYTE + ((tLRBYTE >> 6) * 0.25); 
    }
 
    return (fTemperatureCelsius);
      
}

// define a global variable to access the RTC
DS3231 rtc;


