// MqttBroker.h

#ifndef _MQTTBROKER_h
#define _MQTTBROKER_h
#define SerialMon Serial
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "ExternVars.h"
#include "Keys.h"
#include <PubSubClient.h>
 


class MqttBrokerClass
{
protected:
public:
	// MQTT details
	long lastReconnectAttempt = 0;
	//const char* broker = "prawn-test.optimustech.io";//tcp://prawn-test.optimustech.io
   // String authKey = "74859fe7a54d32e7da5b03fdec688461";
   // String deviceID = "TEST01";  // PAWN : PhysicalDevice Address and  Unique Id for this device 

	void init(PubSubClient m);
	void loopMqtt();
	void publishMessage(const char* topic, const char* payload);
	void subscribeTopic(const char* topic);
	boolean mqttConnect();
	boolean isMqttConnected();

	void setupDevice();
	void saveParametersToSever();
	boolean sendFeed(String timeStamp, String temprature, String PH, String DO);




};

extern MqttBrokerClass MqttBroker;

#endif

