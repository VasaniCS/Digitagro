// Oxyzen.h

#ifndef _OXYZEN_h
#define _OXYZEN_h
#define SerialMon Serial
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DO_PIN  34   //oxy    
#define VREF 3300           
#define LED_PIN 2 
const int TRIGGER_PIN = 0;

class OxyzenClass
{
protected:

public:
	//OX Variables 

	static boolean _doCalibrationFinishFlag;
	static boolean _enterCalibrationFlag;
	float SaturationDoVoltage, SaturationDoTemperature;
	float averageVoltage;
	int analogBufferIndex = 0, copyIndex = 0;

	const float SaturationValueTab[41] PROGMEM = {      //saturation dissolved oxygen concentrations at various temperatures
	14.46, 14.22, 13.82, 13.44, 13.09,
	12.74, 12.42, 12.11, 11.81, 11.53,
	11.26, 11.01, 10.77, 10.53, 10.30,
	10.08, 9.86,  9.66,  9.46,  9.27,
	9.08,  8.90,  8.73,  8.57,  8.41,
	8.25,  8.11,  7.96,  7.82,  7.69,
	7.56,  7.43,  7.30,  7.18,  7.07,
	6.95,  6.84,  6.73,  6.63,  6.53,
	6.41,
	};

	float doValue;               //Current dissolved oxygen value, unit; mg/L

	void initOx();

	void oxLoop();

	byte uartParse();

	boolean serialDataAvailable(void);

	void doCalibration(byte mode);

	int getMedianNum(int bArray[], int iFilterLen);

};

extern OxyzenClass Oxyzen;

#endif

