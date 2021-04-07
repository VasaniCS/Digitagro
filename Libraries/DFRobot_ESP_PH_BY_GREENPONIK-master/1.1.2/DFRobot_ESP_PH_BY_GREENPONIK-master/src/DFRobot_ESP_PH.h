 

#ifndef _DFROBOT_ESP_PH_H_
#define _DFROBOT_ESP_PH_H_

#include "Arduino.h"

#define PHVALUEADDR 0x00 //the start address of the pH calibration parameters stored in the EEPROM

#define PH_8_VOLTAGE 1122
#define PH_6_VOLTAGE 1478
#define PH_5_VOLTAGE 1654
#define PH_3_VOLTAGE 2010

#define ReceivedBufferLength 10 //length of the Serial CMD buffer

class DFRobot_ESP_PH
{
public:
    DFRobot_ESP_PH();
    ~DFRobot_ESP_PH();
  
    void calibration(float voltage, float temperature,byte commandByte);
    float readPH(float voltage, float temperature); // voltage to pH value, with temperature compensation
    void begin(float _neutral_v, float _acid_v);//initialization


    float _phValue;
    float _acidVoltage;
    float _neutralVoltage;
    float _voltage;
    float _temperature;

    char _cmdReceivedBuffer[ReceivedBufferLength]; //store the Serial CMD
    byte _cmdReceivedBufferIndex;

    boolean cmdSerialDataAvailable();
    void phCalibration(byte mode); // calibration process, wirte key parameters to EEPROM
    byte cmdParse(const char *cmd);
    byte cmdParse();
};

#endif
