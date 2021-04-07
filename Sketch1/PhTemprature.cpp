#include "PhTemprature.h"

DFRobot_ESP_PH ph;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void PhTempratureClass::initPh()
{
	float 
	ph.begin(atof(phParam_neutral,phParam_acid));
	sensors.begin();
	//SerialMon.println("PH-TEMPRATURE SENSOR ");
}

void PhTempratureClass::phLoop()
{
	static unsigned long timepoint = millis();
	if (millis() - timepoint > 1000U) //time interval: 1s
	{
		timepoint = millis();
		//voltage = rawPinValue / esp32ADC * esp32Vin
		voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
		//SerialMon.print("Ph voltage:");
	   //	SerialMon.println(voltage, 4);

		temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
		//SerialMon.print("temperature:");
		//SerialMon.print(temperature, 1);
		//SerialMon.println("^C");

		phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
		//SerialMon.print("pH:");
		//SerialMon.println(phValue, 4);
	}
	
	ph.calibration(voltage, temperature); // calibration process by Serail CMD
}

float PhTempratureClass::readTemperature()
{
	//add your code here to get the temperature from your temperature sensor
	sensors.requestTemperatures();
	return sensors.getTempCByIndex(0);
}



PhTempratureClass PhTemprature;

