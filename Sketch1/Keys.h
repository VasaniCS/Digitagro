// Keys.h

#ifndef _KEYS_h
#define _KEYS_h

#include "ExternVars.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#define SSID_MAX_LENGTH           32
#define PASSWORD_MAX_LENGTH       32
#define key1_machineId "key1_machineId"
#define key2_authkey   "key2_authkey"
#define key3_oxParam   "key3_oxParam"
#define key4_phParam   "key4_phParam"
#define key5_brokerName "key5_brokerName"
#define key6_isRegistered "key6_isRegistered"
//
//extern char*  _AUTH_KEY ;
//extern char* Router_SSID ;
//extern char* Router_Pass ;
//extern char*  currentCommand;
//extern char* MachineId;
//extern char* authkey ;
//extern char* oxParam ;
//extern char* phParam ;
//extern int    deviceState ;
//extern const	char* broker;
//extern char* isRegistered; //"TRUE" or "FALSE"
//extern char* topicCreateDevice;
//extern char* _topicRegistration;
//extern char* _topic_Setup;
//extern char* _topic_Command;
//extern char* topicSetupDevice;
//extern char* topicSensorData;
//extern char* topicDeviceParameter;
//extern char* mqttUserName;
//extern char* mqttPassword;

class KeysClass
{
private:

protected:


public:
	
	//	 Default Config Portal SID and Password
	//	 SSID and PW for Config Portal



	static String DefaultPortalSSID;
	static char PortalSSID[SSID_MAX_LENGTH + 1];

	// Use in case PortalSSID or PortalPassword is invalid (NULL)
	static String DefaultPortalPassword;
	static char PortalPassword[PASSWORD_MAX_LENGTH + 1];

	

	// SSID and PW for your Router
	
	void init();
	 void setAuthKey(char* &auth);
	//char*   getAuthKey();
};

extern KeysClass Keys;

#endif

