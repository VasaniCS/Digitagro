#include "Oxyzen.h"

#define ReceivedBufferLengthOx 20
char receivedBufferOnline[ReceivedBufferLengthOx + 1];    // store the serial command
byte receivedBufferIndex = 0;
#define SCOUNT 30            // Sum of sample point
int analogBuffer[SCOUNT];    //Store the analog value in the array, readed from ADC
int analogBufferTemp[SCOUNT];

boolean OxyzenClass::_doCalibrationFinishFlag;
boolean OxyzenClass::_enterCalibrationFlag;

void OxyzenClass::initOx()
{
	pinMode(DO_PIN, INPUT);
	_doCalibrationFinishFlag = 0;
	_enterCalibrationFlag = 0;
}

void  OxyzenClass::oxLoop()
{
	//SerialMon.println("CHECKING OX VALUE__________Nanomine________");
	static unsigned long analogSampleTimepoint = millis();
	if (millis() - analogSampleTimepoint > 30U)     //every 30 milliseconds,read the analog value from the ADC
	{
		analogSampleTimepoint = millis();
		analogBuffer[analogBufferIndex] = analogRead(DO_PIN);    //read the analog value and store into the buffer
		analogBufferIndex++;
		if (analogBufferIndex == SCOUNT)
			analogBufferIndex = 0;
	}

	static unsigned long tempSampleTimepoint = millis();
	if (millis() - tempSampleTimepoint > 500U)  // every 500 milliseconds, read the temperature
	{
		tempSampleTimepoint = millis();
		//temperature = readTemperature();  // add your temperature codes here to read the temperature, unit:^C
	}

	static unsigned long printTimepoint = millis();
	if (millis() - printTimepoint > 1000U)
	{
		printTimepoint = millis();
		for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
		{
			analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
		}
		averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the value more stable by the median filtering algorithm

		doValue = pgm_read_float_near(&SaturationValueTab[0] + (int)(SaturationDoTemperature + 0.5)) * averageVoltage / SaturationDoVoltage;  //calculate the do value, doValue = Voltage / SaturationDoVoltage * SaturationDoValue(with temperature compensation)
	//	SerialMon.print(F(",  DO Value:"));
	//	SerialMon.print(doValue, 2);
	//	SerialMon.println(F("mg/L"));
	}

	////___________________OX FUNCTIONS ENDS____________________
}

boolean  OxyzenClass::serialDataAvailable(void)
{
	char receivedChar;
	static unsigned long receivedTimeOut = millis();
	while (SerialMon.available() > 0)
	{
		if (millis() - receivedTimeOut > 500U)
		{
			receivedBufferIndex = 0;
			memset(receivedBufferOnline, 0, (ReceivedBufferLengthOx + 1));
		}
		receivedTimeOut = millis();
		receivedChar = SerialMon.read();
		if (receivedChar == '\n' || receivedBufferIndex == ReceivedBufferLengthOx)
		{
			receivedBufferIndex = 0;
			strupr(receivedBufferOnline);
			return true;
		}
		else {
			receivedBufferOnline[receivedBufferIndex] = receivedChar;
			receivedBufferIndex++;
		}
	}
	return false;
}

byte  OxyzenClass::uartParse()
{
	byte modeIndex = 0;
	if (strstr(receivedBufferOnline, "CALIBRATION") != NULL)
		modeIndex = 1;
	else if (strstr(receivedBufferOnline, "EXIT") != NULL)
		modeIndex = 3;
	else if (strstr(receivedBufferOnline, "SATCAL") != NULL)
		modeIndex = 2;
	return modeIndex;
}

void  OxyzenClass::doCalibration(byte mode)
{
	float voltageValueStore;
	switch (mode)
	{
	case 0:
		if (_enterCalibrationFlag)
			SerialMon.println(F("Command Error"));
		break;

	case 1:
		_enterCalibrationFlag = 1;
		_doCalibrationFinishFlag = 0;
		SerialMon.println();
		SerialMon.println(F(">>>Enter Calibration Mode<<<"));
		SerialMon.println(F(">>>Please put the probe into the saturation oxygen water! <<<"));
		SerialMon.println();
		break;

	case 2:
		if (_enterCalibrationFlag)
		{
			SerialMon.println();
			SerialMon.println(F(">>>OX Saturation Calibration Finish!<<<"));
			SerialMon.println();
			//MEMORY WRITE averageVoltage  --
			//MEMORY WRITE temperature     --
			SaturationDoVoltage = averageVoltage;

			_doCalibrationFinishFlag = 1;
			memset(receivedBufferOnline, 0, sizeof(receivedBufferOnline));
		}
		break;

	case 3:
		if (_enterCalibrationFlag)
		{
			SerialMon.println();
			if (_doCalibrationFinishFlag)
				SerialMon.print(F(">>>OX Calibration Successful"));
			else
				SerialMon.print(F(">>>OX  Calibration Failed"));
			SerialMon.println(F(",Exit OX  Calibration Mode<<<"));
			SerialMon.println();
			_doCalibrationFinishFlag = 0;
			_enterCalibrationFlag = 0;
		}
		break;
	}
}

int  OxyzenClass::getMedianNum(int bArray[], int iFilterLen)
{
	int bTab[iFilterLen];
	for (byte i = 0; i < iFilterLen; i++)
	{
		bTab[i] = bArray[i];
	}
	int i, j, bTemp;
	for (j = 0; j < iFilterLen - 1; j++)
	{
		for (i = 0; i < iFilterLen - j - 1; i++)
		{
			if (bTab[i] > bTab[i + 1])
			{
				bTemp = bTab[i];
				bTab[i] = bTab[i + 1];
				bTab[i + 1] = bTemp;
			}
		}
	}
	if ((iFilterLen & 1) > 0)
		bTemp = bTab[(iFilterLen - 1) / 2];
	else
		bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
	return bTemp;
}


OxyzenClass Oxyzen;

