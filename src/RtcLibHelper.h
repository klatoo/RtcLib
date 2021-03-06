// RtcLibHelper.h
// 
// last edit 24.Dec.2016 /klatoo  - created
//           26.Dec.2016 /klatoo  - moved some #defines to TimeSpan::xx  static const
//           28.Nov.2020 /klatto - added info where the original code came form
//                                 (was before only in readme.md), some refactoring

// helper classes for RtcLib:
// DateTime, TimeSpan, TimerClock

// DateTime and TimeSpan are derived from the adafruit/RTClib
// and TimerClock24h was added by me


#ifndef RTCLIBHELPER_H
#define RTCLIBHELPER_H

#include <Arduino.h>


// Timespan which can represent changes in time with seconds accuracy.
class TimeSpan {
public:
	static const long SECONDS_FROM_1970_TO_2000 = 946684800;
	static const long SECONDS_PER_DAY = 86400;
	static const int SECONDS_PER_HOUR = 3600;
	static const int SECONDS_PER_MINUTE = 60;
	static const int MINUTES_PER_HOUR = 60;
	static const int HOURS_PER_DAY = 24;

	TimeSpan(int32_t seconds = 0);
	TimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds);
	TimeSpan(const TimeSpan& copy);
	int16_t days() const { return _seconds / SECONDS_PER_DAY; }
	int8_t  hours() const { return _seconds / SECONDS_PER_HOUR % HOURS_PER_DAY; }
	int8_t  minutes() const { return _seconds / SECONDS_PER_MINUTE % MINUTES_PER_HOUR; }
	int8_t  seconds() const { return _seconds % SECONDS_PER_MINUTE; }
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
	uint8_t dayOfWeek() const;

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


class TimerClock24h
{
private:
	DateTime _start, _end;
	

public:
	TimerClock24h();
	void setup(uint8_t startHour, uint8_t startMin, uint8_t startSec, int8_t endHour, uint8_t endMin, uint8_t endSec);
	void setup(DateTime start, DateTime end);
	bool IsOn(DateTime timeToCheck);

protected:
	bool CheckInRange(DateTime start, DateTime end, DateTime timeToCheck);
};


#endif


