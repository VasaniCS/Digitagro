// 
// 
// 
 
#include "Keys.h"


	//char* KeysClass::currentCommand;
	//char* KeysClass::MachineId;
	// char* KeysClass::authkey;
	//char* KeysClass::oxParam;
	//char* KeysClass::phParam;
	//char* KeysClass::broker;
	//char* KeysClass::isRegistered; //"TRUE" or "FALSE"

	//char* KeysClass::topicCreateDevice;
	//char* KeysClass::_topicRegistration;
	//char* KeysClass::_topic_Setup;
	//char* KeysClass::_topic_Command;
	//char* KeysClass::topicSetupDevice;
	//char* KeysClass::topicSensorData;
	//char* KeysClass::topicDeviceParameter;
	//char* KeysClass::mqttUserName;
	//char* KeysClass::mqttPassword;

    //char* KeysClass::CONFIG_FILE;
	void KeysClass::init()
	{
		Serial.println("------SETTING VALUES FOR KEYS-----");
		MachineId = "TEST01";
		broker = "35.225.3.72";
		mqttUserName = "enddevice";
		mqttPassword = "enddevice";
		CONFIG_FILE = "/Config.json";
		_topicRegistration = "-registration";
		_topic_Setup = "-setup";//
		_topic_Command = "-command";

		topicCreateDevice = "create-device";
		topicSetupDevice = "setup-device"; //top
		topicSensorData = "sensor-data";
		topicDeviceParameter = "device-parameter";
//		Serial.print("SETTING AUTH KEY IN KEYS CLASS");
		currentCommand = "none";
		authkey = "000";
		oxParam = "0.00000";
		phParam = "0.0000";
		isRegistered = "FALSE"; //"TRUE" or "FALSE"
		deviceState = -1;
	}

	void KeysClass::setAuthKey(char* &auth)
	{
		 Serial.print("SETTING AUTH KEY to ******************");
		Serial.print(auth);
		authkey = auth;
		Serial.print(authkey);
		//_AUTH_KEY = authkey;
	}

	//char* KeysClass::getAuthKey()
	//{
	//	Serial.println("RETURNED:" + (String)authkey);
	//	return     authkey;
	//}



	KeysClass Keys;
 
