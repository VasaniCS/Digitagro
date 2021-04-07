// RTC.h

#ifndef _RTC_h
#define _RTC_h



#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include "GSMOpetarions.h"
#define countof(a) (sizeof(a) / sizeof(a[0]))
class RTCClass
{
protected:
public:
	void init();
	String getCurrentTimeStamp();
	void  setTimeToRtc(uint16_t year, uint8_t month, uint8_t dayOfMonth, uint8_t hour, uint8_t minute, uint8_t second);

	 
};

extern RTCClass RTC;

#endif

