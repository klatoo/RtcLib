// RtcLibHelper.h
// 
// last edit 24.Dec.2016 /klatoo  - created

// helper classes fro the RTC lib:
// DateTime, TimeSpan, TimerClock
//

#ifndef RTCLIBHELPER_H
#define RTCLIBHELPER_H

#include <Arduino.h>


// Timespan which can represent changes in time with seconds accuracy.
class TimeSpan {
public:
	TimeSpan(int32_t seconds = 0);
	TimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds);
	TimeSpan(const TimeSpan& copy);
	int16_t days() const { return _seconds / 86400L; }
	int8_t  hours() const { return _seconds / 3600 % 24; }
	int8_t  minutes() const { return _seconds / 60 % 60; }
	int8_t  seconds() const { return _seconds % 60; }
	int32_t totalseconds() const { return _seconds; }

	TimeSpan operator+(const TimeSpan& right);
	TimeSpan operator-(const TimeSpan& right);

protected:
	int32_t _seconds;
};

// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
class DateTime {
public:
	DateTime(uint32_t t = 0);
	DateTime(uint16_t year, uint8_t month, uint8_t day,
		uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
	DateTime(const DateTime& copy);
	DateTime(const char* date, const char* time);
	DateTime(const __FlashStringHelper* date, const __FlashStringHelper* time);
	uint16_t year() const { return 2000 + yOff; }
	uint8_t month() const { return m; }
	uint8_t day() const { return d; }
	uint8_t hour() const { return hh; }
	uint8_t minute() const { return mm; }
	uint8_t second() const { return ss; }
	uint8_t dayOfTheWeek() const;

	// 32-bit times as seconds since 1/1/2000
	long secondstime() const;
	// 32-bit times as seconds since 1/1/1970
	uint32_t unixtime(void) const;

	DateTime operator+(const TimeSpan& span);
	DateTime operator-(const TimeSpan& span);
	TimeSpan operator-(const DateTime& right);

protected:
	uint8_t yOff, m, d, hh, mm, ss;
};


class TimerClock
{
private:
	DateTime _dtStart, _dtEnd;
	enum bla { bla1, bla2, bla3 };

public:
	TimerClock();
	void setup(DateTime dtStart, DateTime dtEnd);
	bool IsOn(DateTime dtNow);

protected:
	bool CheckInRange(DateTime dtStart, DateTime dtEnd, DateTime dtNow);
};


#endif


