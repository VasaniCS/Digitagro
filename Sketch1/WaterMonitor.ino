#include "ExternVars.h"
#include "Keys.h"
#include "RTC.h"
#include "PhTemprature.h"
#include "Oxyzen.h"
#include "MqttBroker.h"
#include "MemoryOperations.h"
#include "JsonOperations.h"
#include "GSMOpetarions.h"
#include "Keys.h"
#include "RTC.h" 
#include "JsonOperations.h"
#include "MemoryOperations.h"
#include "Keys.h"
#include "MqttBroker.h"
#include "PhTemprature.h"
#include "Oxyzen.h"
#include "GSMOpetarions.h"
#define USE_AVAILABLE_PAGES     false
#include <ArduinoJson.h> 
// Your WiFi connection credentials, if applicable
const char wifiSSID[] = "free net";
const char wifiPass[] = "Veer543210";
 char* CONFIG_FILE;
 char*  _AUTH_KEY;
 char* Router_SSID;
 char* Router_Pass;
 char*  currentCommand;
 char* MachineId;
 char* authkey;
 char* oxParam;
 char* phParam;
 int    deviceState;
 char* broker;
 char* isRegistered; //"TRUE" or "FALSE"
 char* topicCreateDevice;
 char* _topicRegistration;
 char* _topic_Setup;
 char* _topic_Command;
 char* topicSetupDevice;
 char* topicSensorData;
 char* topicDeviceParameter;
 char* mqttUserName;
 char* mqttPassword;


KeysClass keysO;
MemoryOperationsClass memOp;
OxyzenClass Oxy;
PhTempratureClass phTemprature;
MqttBrokerClass mqttbroker;
GSMOperationsClass gsmOps;
RTCClass rtc;

unsigned long next_pol = 0;
//*********************************Variable Declaration ends******************************************
void setup() {
	// Set console baud rate
	SerialMon.begin(115200);
	 
	
	pinMode(LED_PIN, OUTPUT);
	//wifi manager mate
	pinMode(TRIGGER_PIN, INPUT_PULLUP);
	SerialMon.println("------############# S T A R T ##################-----");
	SerialMon.println("------Initializing Variables-----");
	keysO.init();
	SerialMon.println("------CONFIGFILE NAME:"+ (String)CONFIG_FILE);
	MachineId = "TEST01";
	SerialMon.println(authkey);
	SerialMon.print("MACHINE UNIQUE ID :");
	SerialMon.println(MachineId);
 	memOp.readConfigFile();
	delay(100);
	//SerialMon.print("AUTHENTICATION KEY :");
	//SerialMon.println(authkey);
	//memOp.deleteConfigFile();//temporary

	SerialMon.println("\n------Initializing GSM-----");
	gsmOps.init();
	SerialMon.println("------Initializing DO SENSOR-----");
	Oxy.initOx();
	SerialMon.println("------Initializing PH-TEMPRATURE SENSOR-----");
	phTemprature.initPh();
	// MQTT Broker setup
	SerialMon.println("------INITIALIZING MQTT BROKER CONNECTION-----");
	mqttbroker.init(gsmOps.mqttSC);
	SerialMon.println("------INITIALIZING RTC-----");
	rtc.init();
	//rtc.setTimeToRtc(gsmOps.getGSMTime());
	
	SerialMon.println("------INITIALIZING SD CARD-----");
	memOp.init();


	/*SerialMon.println("%%%%%%%%%%%%%%ALL VARIBLES%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
	
	SerialMon.println(MachineId);
	SerialMon.println(authkey);
	SerialMon.println(broker);
	SerialMon.println(phParam);
	SerialMon.println(oxParam);
	SerialMon.println("%%%%%%%%%%%%%%ALL VARIBLES%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
	*/
	//1. CREATE(REGISTER) DEVICE IF NOT CREATED
	isRegistered = "TRUE";//temporary
	if (isRegistered == "FALSE")
	{
		if (mqttbroker.isMqttConnected()) {
			SerialMon.println("------Publishing For the firsttime deviceId-----");
			mqttbroker.subscribeTopic(MachineId);
			String createDeviceJsonString = "{\"machineId\": \"" + (String)MachineId + "\"}";
			mqttbroker.publishMessage(topicCreateDevice, createDeviceJsonString.c_str());
			memOp.writeConfigFile();
		}
	}
	else
	{
		if (mqttbroker.isMqttConnected()) {
			//STATE REGISTERED BUT NOT SETUP
			if (authkey == "000")
			{
				mqttbroker.setupDevice();
				memOp.writeConfigFile();
			}
		}
		else  //if setup done device can  publishmessages
		{
			deviceState = 1;
		}
	}

	//MEMORY CARD
	SerialMon.println("------SD CARD OPERATIONS BEGIN ---");
//	memOp.listDir(SD, "/PAWN", 0);
}

void loop() {
	//SerialMon.print("LOOP @@@@@@@@@@@@@@@@@@@@@@@@@@@AUTHENTICATION KEY :");
	//SerialMon.println(authkey);

	//already in mqttbroker.mqttloop
	//else {
	//if (!mqttbroker.isMqttConnected())mqttbroker.mqttConnect();
	//}
	if (currentCommand == "calibrate-do") {
		Oxy.doCalibration(1);
		if (deviceState)MqttBroker.saveParametersToSever();
	}
	if (currentCommand == "calibrate-ph") {
		//phTemprature(1); 
		if (deviceState)MqttBroker.saveParametersToSever();
	}

	//if (currentCommand == "calibrate") ;
	Oxy.oxLoop();
	PhTemprature.phLoop();
 //	rtc.setTimeToRtc();
	String timeStamp = rtc.getCurrentTimeStamp();
	//SerialMon.println("RTC DATETIME :");
	//SerialMon.println(rtc.getCurrentTimeStamp());

	//every 5 mins 
	if (millis() > next_pol)
	{
		next_pol = millis() + 5000;
		SerialMon.println("------SENDING DATA FEED-----: TEMP :" + (String)phTemprature.temperature + " \n PH :" + (String)phTemprature.phValue + "\n DO" + (String)Oxy.doValue);
		if (!mqttbroker.sendFeed("timeStamp", (String)phTemprature.temperature, (String)phTemprature.phValue, (String)Oxy.doValue))
		{
			String fileName = "/" + (String)"PAWN" + "/" + timeStamp + (String)".txt";
			SerialMon.println("CREATING FILE : " + fileName);
			String SensorData =
				"{\"deviceRefId\":\"" + (String)MachineId
				+ "\",\"authKey\" : \"" + authkey				+ "\",\"timestamp\" : \"" +  timeStamp.c_str();				+ "\",\"temperature\" : \"" + (String)phTemprature.temperature				+ "\",\"ph\" : \"" + (String)phTemprature.phValue				+ "\",\"do\" : \"" + (String)Oxy.doValue				+ "\"}";
			SerialMon.println("With Sensor Data :" + SensorData);
			memOp.writeFile(SD,  fileName.c_str(), SensorData.c_str());
		}
	}

	mqttbroker.loopMqtt();
	if (!gsmOps.isGsmConnected())gsmOps.init();
}



 