#include "RTC.h"
RtcDS3231<TwoWire> rtcobject(Wire);
void RTCClass::init()
{	
	Wire.begin(21, 22); // pin 0 and 2 for SDA, SCL //H_RTC
	rtcobject.Begin();
	RtcDateTime currentTime = RtcDateTime(20, 06, 27, 10,55, 0); //define date and time object
	//rtcobject.SetDateTime(currentTime); //configure the RTC with object
	SerialMon.println("Initializing RTC Done");
}

String	 RTCClass::getCurrentTimeStamp()
{
	char datestring[22];
	RtcDateTime now = rtcobject.GetDateTime();
	// SerialMon.println("*+++++++++++++++++++++RTC NOW DAY OF WEEK ");
	// SerialMon.println((String)now.DayOfWeek());
	int hr = now.Hour();
	int mi = now.Minute();
	if (mi >= 60)
	{
		hr -= 1;
		mi -= 60;
	}
	//Serial.println("Hrtc Time");
	 snprintf_P(datestring, countof(datestring), 
		 //PSTR("%02u-%02u-%02uT%02u:%02u:%02u"),
		 PSTR("%02u%02u%04u_%02u_%02u_%02u"),
		 now.Day(), now.Month(), now.Year(), hr, mi, now.Second());
	 
	
	  //SerialMon.println("*GSM DATE TIME :" + GSMOperations.getGSMTime());
	// SerialMon.println("*+++++++++++++++++++++RTC DATE TIME :+++++++++++++++++");
	// SerialMon.println(datestring);
	return  datestring;
}

void  RTCClass::setTimeToRtc(uint16_t year,uint8_t month,uint8_t dayOfMonth,uint8_t hour,uint8_t minute,uint8_t second)
{
	RtcDateTime currentTime = RtcDateTime(year,month, dayOfMonth, hour, minute, second); //define date and time object
	rtcobject.SetDateTime(currentTime); //configure the RTC with object
}

 
RTCClass RTC;

