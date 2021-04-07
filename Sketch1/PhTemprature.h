// PhTemprature.h

#ifndef _PHTEMPRATURE_h
#define _PHTEMPRATURE_h
#define SerialMon Serial
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFRobot_ESP_PH.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#define ONE_WIRE_BUS 32  // temp sensor
#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3300 //the esp voltage supply value
#define PH_PIN 35       //the esp gpio data pin number  ph


class PhTempratureClass
{
protected:

public:
	float voltage, phValue, temperature = 25;
	void initPh();
	void phLoop();
	float  readTemperature();
};
extern PhTempratureClass PhTemprature;
#endif

