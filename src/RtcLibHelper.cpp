// RtcLibHelper.cpp
//
// last edit 24.Dec.2016   /klatoo  created
//           27.Dec.2016   /klatoo added additional TimerClock24::setup variant
//

// helper classes fro the RTC lib:
// DateTime, TimeSpan, TimerClock
//

#include "RtcLibHelper.h"
#include <avr/pgmspace.h>



////////////////////////////////////////////////////////////////////////////////
// utility code, some of this could be exposed in the DateTime API if needed

const uint8_t daysInMonth[] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

// number of days since 2000/01/01, valid for 2001..2099
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
	if (y >= 2000)
		y -= 2000;
	uint16_t days = d;
	for (uint8_t i = 1; i < m; ++i)
		days += pgm_read_byte(daysInMonth + i - 1);
	if (m > 2 && y % 4 == 0)
		++days;
	return days + 365 * y + (y + 3) / 4 - 1;
}

static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
	return ((days * 24L + h) * 60 + m) * 60 + s;
}

////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second

DateTime::DateTime(uint32_t t) {
	t -= TimeSpan::SECONDS_FROM_1970_TO_2000;    // bring to 2000 timestamp from 1970

	ss = t % 60;
	t /= 60;
	mm = t % 60;
	t /= 60;
	hh = t % 24;
	uint16_t days = t / 24;
	uint8_t leap;
	for (yOff = 0; ; ++yOff) {
		leap = yOff % 4 == 0;
		if (days < 365 + leap)
			break;
		days -= 365 + leap;
	}
	for (m = 1; ; ++m) {
		uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
		if (leap && m == 2)
			++daysPerMonth;
		if (days < daysPerMonth)
			break;
		days -= daysPerMonth;
	}
	d = days + 1;
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
	if (year >= 2000)
		year -= 2000;
	yOff = year;
	m = month;
	d = day;
	hh = hour;
	mm = min;
	ss = sec;
}

DateTime::DateTime(const DateTime& copy) :
	yOff(copy.yOff),
	m(copy.m),
	d(copy.d),
	hh(copy.hh),
	mm(copy.mm),
	ss(copy.ss)
{}

static uint8_t conv2d(const char* p) {
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9')
		v = *p - '0';
	return 10 * v + *++p - '0';
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using F() would further reduce the RAM footprint, see below.
DateTime::DateTime(const char* date, const char* time) {
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	yOff = conv2d(date + 9);
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
	switch (date[0]) {
	case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
	case 'F': m = 2; break;
	case 'A': m = date[2] == 'r' ? 4 : 8; break;
	case 'M': m = date[2] == 'r' ? 3 : 5; break;
	case 'S': m = 9; break;
	case 'O': m = 10; break;
	case 'N': m = 11; break;
	case 'D': m = 12; break;
	}
	d = conv2d(date + 4);
	hh = conv2d(time);
	mm = conv2d(time + 3);
	ss = conv2d(time + 6);
}

// A convenient constructor for using "the compiler's time":
// This version will save RAM by using PROGMEM to store it by using the F macro.
//   DateTime now (F(__DATE__), F(__TIME__));
DateTime::DateTime(const __FlashStringHelper* date, const __FlashStringHelper* time) {
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	char buff[11];
	memcpy_P(buff, date, 11);
	yOff = conv2d(buff + 9);
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	switch (buff[0]) {
	case 'J': m = buff[1] == 'a' ? 1 : m = buff[2] == 'n' ? 6 : 7; break;
	case 'F': m = 2; break;
	case 'A': m = buff[2] == 'r' ? 4 : 8; break;
	case 'M': m = buff[2] == 'r' ? 3 : 5; break;
	case 'S': m = 9; break;
	case 'O': m = 10; break;
	case 'N': m = 11; break;
	case 'D': m = 12; break;
	}
	d = conv2d(buff + 4);
	memcpy_P(buff, time, 8);
	hh = conv2d(buff);
	mm = conv2d(buff + 3);
	ss = conv2d(buff + 6);
}

uint8_t DateTime::dayOfWeek() const {
	uint16_t day = date2days(yOff, m, d);
	return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

uint32_t DateTime::unixtime(void) const {
	uint32_t t;
	uint16_t days = date2days(yOff, m, d);
	t = time2long(days, hh, mm, ss);
	t += TimeSpan::SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000

	return t;
}

long DateTime::secondstime(void) const {
	long t;
	uint16_t days = date2days(yOff, m, d);
	t = time2long(days, hh, mm, ss);
	return t;
}

DateTime DateTime::operator+(const TimeSpan& span) {
	return DateTime(unixtime() + span.totalseconds());
}

DateTime DateTime::operator-(const TimeSpan& span) {
	return DateTime(unixtime() - span.totalseconds());
}

TimeSpan DateTime::operator-(const DateTime& right) {
	return TimeSpan(unixtime() - right.unixtime());
}

////////////////////////////////////////////////////////////////////////////////
// TimeSpan implementation

TimeSpan::TimeSpan(int32_t seconds) :
	_seconds(seconds)
{}

TimeSpan::TimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds) :
	_seconds((int32_t)days * 86400L + (int32_t)hours * 3600 + (int32_t)minutes * 60 + seconds)
{}

TimeSpan::TimeSpan(const TimeSpan& copy) :
	_seconds(copy._seconds)
{}

TimeSpan TimeSpan::operator+(const TimeSpan& right) {
	return TimeSpan(_seconds + right._seconds);
}

TimeSpan TimeSpan::operator-(const TimeSpan& right) {
	return TimeSpan(_seconds - right._seconds);
}



TimerClock24h::TimerClock24h()
{
	// set to a default time to avoid any issues if setup() was not called

	_dtStart = DateTime(0, 0, 0, 19, 0, 0);
	_dtEnd = DateTime(0, 0, 0, 20, 0, 0);
}

void TimerClock24h::setup(DateTime dtStart, DateTime dtEnd)
{
	_dtStart = dtStart;
	_dtEnd = dtEnd;
}

void TimerClock24h::setup(uint8_t startHour, uint8_t startMin, uint8_t startSec, int8_t endHour, uint8_t endMin, uint8_t endSec)
{
	_dtStart = DateTime(0, 0, 0, startHour, startMin, startSec);
	_dtEnd = DateTime(0, 0, 0, endHour, endMin, endSec);
}


bool TimerClock24h::CheckInRange(DateTime dtStart, DateTime dtEnd, DateTime dtNow)
{
	if (dtNow.secondstime() >= dtStart.secondstime())
	{
		if (dtNow.secondstime() <= dtEnd.secondstime())
		{
			return true;
		}
	}
	return false;
}

bool TimerClock24h::IsOn(DateTime dtNow)
{
	// if end is next day, i.e. crosses 24:00 hrs mark
	if (_dtStart.hour() > _dtEnd.hour())
	{
		// calculate from 0:00 to end

		if (CheckInRange(DateTime(dtNow.year(), dtNow.month(), dtNow.day(), 0, 0, 0),
			DateTime(dtNow.year(), dtNow.month(), dtNow.day(), _dtEnd.hour(), _dtEnd.minute(), _dtEnd.second()),
			dtNow)
			)
			return true;

		// calculate from start to 24:00

		if (CheckInRange(DateTime(dtNow.year(), dtNow.month(), dtNow.day(), _dtStart.hour(), _dtStart.minute(), _dtStart.second()),
			DateTime(dtNow.year(), dtNow.month(), dtNow.day(), 24, 0, 0),
			dtNow)
			)
			return true;

		return false;
	}
	else
	{
		// calculate from start to end

		if (CheckInRange(DateTime(dtNow.year(), dtNow.month(), dtNow.day(), _dtStart.hour(), _dtStart.minute(), _dtStart.second()),
			DateTime(dtNow.year(), dtNow.month(), dtNow.day(), _dtEnd.hour(), _dtEnd.minute(), _dtEnd.second()),
			dtNow)
			)
			return true;
	}

	return false;
}

