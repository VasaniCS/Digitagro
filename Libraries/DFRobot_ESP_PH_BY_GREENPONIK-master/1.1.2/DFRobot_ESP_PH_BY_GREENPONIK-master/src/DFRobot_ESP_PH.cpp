

#include "Arduino.h"
#include "DFRobot_ESP_PH.h"
#include "EEPROM.h"

#define PH_3_VOLTAGE 2010

DFRobot_ESP_PH::DFRobot_ESP_PH()
{
	this->_temperature = 25.0;
	this->_phValue = 7.0;
	this->_acidVoltage = 2032.44;   //buffer solution 4.0 at 25C
	this->_neutralVoltage = 1500.0; //buffer solution 7.0 at 25C
	this->_voltage = 1500.0;
}

DFRobot_ESP_PH::~DFRobot_ESP_PH()
{
}

void DFRobot_ESP_PH::begin(float _neutral_v, float _acid_v)
{
	//check if calibration values (neutral and acid) are stored in eeprom
	this->_neutralVoltage = _neutral_v; //load the neutral (pH = 7.0)voltage of the pH board from the EEPROM
	if (this->_neutralVoltage == float() || isnan(this->_neutralVoltage))
	{
		this->_neutralVoltage = 1500.0; // new EEPROM, write typical voltage
	}

	this->_acidVoltage = _acid_v; //load the acid (pH = 4.0) voltage of the pH board from the EEPROM
	if (this->_acidVoltage == float() || isnan(this->_acidVoltage))
	{
		this->_acidVoltage = 2032.44; // new EEPROM, write typical voltage
	}
}

float DFRobot_ESP_PH::readPH(float voltage, float temperature)
{
	// Serial.print("_neutraVoltage:");
	// Serial.print(this->_neutralVoltage);
	// Serial.print(", _acidVoltage:");
	// Serial.print(this->_acidVoltage);
	float slope = (7.0 - 4.0) / ((this->_neutralVoltage - 1500.0) / 3.0 - (this->_acidVoltage - 1500.0) / 3.0); // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
	float intercept = 7.0 - slope * (this->_neutralVoltage - 1500.0) / 3.0;
	// Serial.print(", slope:");
	// Serial.print(slope);
	// Serial.print(", intercept:");
	// Serial.println(intercept);
	this->_phValue = slope * (voltage - 1500.0) / 3.0 + intercept; //y = k*x + b
	Serial.print("[readPH]... phValue ");
	Serial.println(this->_phValue);
	return this->_phValue;
}


void DFRobot_ESP_PH::calibration(float voltage, float temperature, byte commandByte)
{
	this->_voltage = voltage;
	this->_temperature = temperature;

	phCalibration(commandByte); // if received Serial CMD from the serial monitor, enter into the calibration mode

}

boolean DFRobot_ESP_PH::cmdSerialDataAvailable()
{
	char cmdReceivedChar;
	static unsigned long cmdReceivedTimeOut = millis();
	while (Serial.available() > 0)
	{
		if (millis() - cmdReceivedTimeOut > 500U)
		{
			this->_cmdReceivedBufferIndex = 0;
			memset(this->_cmdReceivedBuffer, 0, (ReceivedBufferLength));
		}
		cmdReceivedTimeOut = millis();
		cmdReceivedChar = Serial.read();
		if (cmdReceivedChar == '\n' || this->_cmdReceivedBufferIndex == ReceivedBufferLength - 1)
		{
			this->_cmdReceivedBufferIndex = 0;
			strupr(this->_cmdReceivedBuffer);
			return true;
		}
		else
		{
			this->_cmdReceivedBuffer[this->_cmdReceivedBufferIndex] = cmdReceivedChar;
			this->_cmdReceivedBufferIndex++;
		}
	}
	return false;
}

byte DFRobot_ESP_PH::cmdParse(const char *cmd)
{
	byte modeIndex = 0;
	if (strstr(cmd, "ENTERPH") != NULL)
	{
		modeIndex = 1;
	}
	else if (strstr(cmd, "EXITPH") != NULL)
	{
		modeIndex = 3;
	}
	else if (strstr(cmd, "CALPH") != NULL)
	{
		modeIndex = 2;
	}
	return modeIndex;
}

byte DFRobot_ESP_PH::cmdParse()
{
	byte modeIndex = 0;
	if (strstr(this->_cmdReceivedBuffer, "ENTERPH") != NULL)
	{
		modeIndex = 1;
	}
	else if (strstr(this->_cmdReceivedBuffer, "EXITPH") != NULL)
	{
		modeIndex = 3;
	}
	else if (strstr(this->_cmdReceivedBuffer, "CALPH") != NULL)
	{
		modeIndex = 2;
	}
	return modeIndex;
}

void DFRobot_ESP_PH::phCalibration(byte mode)
{
	char *receivedBufferPtr;
	static boolean phCalibrationFinish = 0;
	static boolean enterCalibrationFlag = 0;
	switch (mode)
	{
	case 1:
		enterCalibrationFlag = 1;
		phCalibrationFinish = 0;
		Serial.println();
		Serial.println(F(">>>Enter PH Calibration Mode<<<"));
		Serial.println(F(">>>Please put the probe into the 4.0 or 7.0 standard buffer solution<<<"));
		Serial.println();
		break;

	case 2:
		if (enterCalibrationFlag)
		{
			if ((this->_voltage > PH_8_VOLTAGE) && (this->_voltage < PH_6_VOLTAGE))
			{
				// buffer solution:7.0
				Serial.println();
				Serial.print(F(">>>Buffer Solution:7.0"));
				this->_neutralVoltage = this->_voltage;
				Serial.println(F(",Send EXITPH to Save and Exit<<<"));
				Serial.println();
				phCalibrationFinish = 1;
			}
			else if ((this->_voltage > PH_5_VOLTAGE) && (this->_voltage < PH_3_VOLTAGE))
			{
				//buffer solution:4.0
				Serial.println();
				Serial.print(F(">>>Buffer Solution:4.0"));
				this->_acidVoltage = this->_voltage;
				Serial.println(F(",Send EXITPH to Save and Exit<<<"));
				Serial.println();
				phCalibrationFinish = 1;				
			}
			else
			{
				Serial.println();
				Serial.print(F(">>>Buffer Solution Error Try Again<<<"));
				Serial.println(); // not buffer solution or faulty operation
				phCalibrationFinish = 0;
			}
		}

		////case 3 here 
		//if ((this->_voltage > PH_8_VOLTAGE) && (this->_voltage < PH_5_VOLTAGE))
		//{
		//	this->_neutralVoltage;
		//}
		//else if ((this->_voltage > PH_5_VOLTAGE) && (this->_voltage < PH_3_VOLTAGE))
		//{
		//	this->_acidVoltage;
		//}

		//Serial.print(F(">>>Calibration Successful"));
		Serial.println(F(",Exit PH Calibration Mode<<<"));
		Serial.println();
		phCalibrationFinish = 0;
		enterCalibrationFlag = 0;
		break;
	}
}
