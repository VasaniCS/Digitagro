// GSMOperations.h
#ifndef _GSMOPERATIONS_h
#define _GSMOPERATIONS_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define GSM_PIN ""
#if TINY_GSM_USE_GPRS && not defined TINY_GSM_MODEM_HAS_GPRS
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS false
#define TINY_GSM_USE_WIFI true
#endif
#if TINY_GSM_USE_WIFI && not defined TINY_GSM_MODEM_HAS_WIFI
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#endif
#define LED_PIN 13
#include "MqttBroker.h"
#include <TinyGsmClient.h>

class GSMOperationsClass
{
protected:
public:
	
	int ledStatus = LOW;
	PubSubClient mqttSC;
	uint16_t n = 0;
	void init();
	boolean isGsmConnected();
	String getGSMTime();
};

extern GSMOperationsClass GSMOperations;
#endif

