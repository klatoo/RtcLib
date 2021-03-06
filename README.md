# RtcLib

This is an Arduino library for the DS3231 RTC (Real Time Clock).
The DS32331 class is forked from Sodaq_DS3231
DateTime and TimeSpan are derived from the adafruit/RTClib
and TimerClock24h was added by me

## Main goal
Provide simple RTC functionality for a timer clock 

## Datasheet for D3231
The datasheet can e.g. found here: https://datasheets.maximintegrated.com/en/ds/DS3231.pdf

## Version history
V 1.0
24./25.Dec.16	Created, splitted the files into RTC class and helper classes,
				Updated the samples
V 1.1
27.Dec.16       Added sample for TimerClock24h

V 1.2
28.Nov.20       Some refactoring, fixed typos and added info on original autors in code


## Old README

RDS3231 Class is by Seeed Technology Inc(http://www.seeedstudio.com) and used
in Seeeduino Stalker v2.1 for battery management(MCU power saving mode)
& to generate timestamp for data logging. DateTime Class is a modified
version supporting day-of-week.

Original DateTime Class and its utility code is by Jean-Claude Wippler at JeeLabs
http://jeelabs.net/projects/cafe/wiki/RTClib 

Released under MIT License http://opensource.org/licenses/mit-license.php

