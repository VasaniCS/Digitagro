// ExternVars.h

#ifndef _EXTERNVARS_h
#define _EXTERNVARS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

extern char* _AUTH_KEY;
extern char* Router_SSID;
extern char* Router_Pass;
extern char*  currentCommand;
extern char* MachineId;
extern char* authkey;
extern char* oxParam;
extern char* phParam;
extern int deviceState;
extern  char* broker;
extern char* isRegistered; //"TRUE" or "FALSE"
extern char* topicCreateDevice;
extern char* _topicRegistration;
extern char* _topic_Setup;
extern char* _topic_Command;
extern char* topicSetupDevice;
extern char* topicSensorData;
extern char* topicDeviceParameter;
extern char* mqttUserName;
extern char* mqttPassword;
extern char* CONFIG_FILE;

#endif

