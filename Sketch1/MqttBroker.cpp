#include "MqttBroker.h"
#include "GSMOpetarions.h"
#include "JsonOperations.h"
#include <ArduinoJson.h>  
#include "MemoryOperations.h"
#include "Keys.h"

PubSubClient mqtt;
JsonOperationsClass jsn;

void  mqttCallback(char* topic, byte* payload, unsigned int len) {
	SerialMon.print("Message Received [");
	SerialMon.print(topic);
	SerialMon.print("]: ");
	SerialMon.write(payload, len);
	SerialMon.println();

	//STEP 1 CREATE DEVICE RESPONSE topic = Keys.MachineId
	if (String(topic) == (MachineId)) {
		String registerStatus = jsn.getJsonKeyValueFromJsonString((char*)payload, "status");
		if (registerStatus == "200" || registerStatus == "409")
		{
			isRegistered = "TRUE";
			SerialMon.println("Registration status: " + registerStatus + " --- ");
			MqttBroker.setupDevice();
		}
	}

	//STEP 2 SETUP DEVICE Get AuthKey  RESPONSE topic : Keys.MachineId-setup
	if (String(topic) == (MachineId + (String)_topic_Setup)) {
		//authKey
		String setupStatus = jsn.getJsonKeyValueFromRootNode((char*)payload, "data");
		//if (setupStatus == "200")
		//{
		String Data = jsn.getJsonKeyValueFromJsonString((char*)payload, "data");
		String authKey = jsn.getJsonKeyValueFromJsonString((char*)Data.c_str(), "authKey");
		authkey = (char*)authKey.c_str();
		SerialMon.println("+AUTH KEY RECEIVED +" + authKey);
		MemoryOperations.writeConfigFile();
		//}
	}

	//STEP 5 SERVER SENDS CUSTOM COMMAND (//   shutdown//	'calibrate'//	'calibrate-do'//'calibrate-ph'//	'restart')	if (String(topic) == (MachineId + (String)_topic_Command)) {
		String commandFromServer = jsn.getJsonKeyValueFromJsonString((char*)payload, "command");
		currentCommand =  (char*)commandFromServer.c_str();
		SerialMon.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++ COMMAND RECEIVED +:");
		SerialMon.println(currentCommand);
		SerialMon.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++ COMMAND PRINTED:");

		//shutdown
		if (commandFromServer == "shutdown" || commandFromServer == "restart")
		{
			SerialMon.println("+CUSTOM COMMAND RECEIVED +:" + commandFromServer);
		}
		//calibrate
		if (commandFromServer == "calibrate")
		{
			String str = jsn.getJsonKeyValueFromJsonString((char*)payload, "authKey");
			SerialMon.println("+CUSTOM COMMAND RECEIVED +:" + commandFromServer);
		}
		if (commandFromServer == "calibrate-do")
		{
			String str = jsn.getJsonKeyValueFromJsonString((char*)payload, "authKey");
			SerialMon.println("+CUSTOM COMMAND RECEIVED +:" + commandFromServer);
		}
		if (commandFromServer == "calibrate-ph")
		{
			String str = jsn.getJsonKeyValueFromJsonString((char*)payload, "authKey");
			SerialMon.println("+CUSTOM COMMAND RECEIVED +:" + commandFromServer);
		}
	}
}

void MqttBrokerClass::setupDevice()
{
	//SETUP DEVICE 
	//topic: setup-device
	String setupTopic = MachineId + (String)_topic_Setup;
	String createDeviceJsonString = "{\"machineId\": \"" + (String)MachineId + "\"}";
	SerialMon.println("SETUP DEVICE PUBLISH_TOPIC :" + setupTopic + " _DATA_ :" + createDeviceJsonString);
	mqtt.subscribe((char*)setupTopic.c_str());
	mqtt.publish(topicSetupDevice, createDeviceJsonString.c_str());
}


void MqttBrokerClass::saveParametersToSever()
{
	String saveParameterJson = "{\"machineId\":\"" + (String)MachineId + "\",\"authKey\" : \""
		+ authkey + "\",\"ph\" : \"" + phParam + "\",\"ox\" : \"" + oxParam + "\"}";	mqtt.publish(topicDeviceParameter, saveParameterJson.c_str());
}

boolean MqttBrokerClass::sendFeed(String timeStamp, String temprature, String PH, String DO)
{
	String SensorData =
		"{\"deviceRefId\":\"" + (String)MachineId
		+ "\",\"authKey\" : \"" + authkey		+ "\",\"timestamp\" : \"" + timeStamp		+ "\",\"temperature\" : \"" + temprature		+ "\",\"ph\" : \"" + PH		+ "\",\"do\" : \"" + DO		+ "\"}";	deviceState = 1;//TEMP	if (deviceState == 1)	{
		return	mqtt.publish(topicSensorData, SensorData.c_str());		 
	}
	else
	{
		return false;
	}
}


void MqttBrokerClass::init(PubSubClient m)
{
	mqtt = m;
	mqtt.setServer(broker, 1883);
	mqttConnect();
	mqtt.setCallback(mqttCallback);
}

boolean MqttBrokerClass::mqttConnect() {
	SerialMon.print("Connecting to *----MQTT BROKER----* ");
	SerialMon.print(broker);
	//Connect to MQTT Broker
	//boolean status = mqtt.connect("GsmClientTest");
	// Or, if you want to authenticate MQTT:

	boolean status = mqtt.connect("enddevice", mqttUserName, mqttPassword);

	if (status == false) {
		SerialMon.println("<<<<MQTT Connection failed>>>>");
		return false;
	}
	SerialMon.println("<<<<MQTT Connection success>>>>");


	return mqtt.connected();
}


void MqttBrokerClass::loopMqtt()
{
	if (!mqtt.connected()) {
		SerialMon.println("=== MQTT NOT CONNECTED LOOP===");
		// Reconnect every 10 seconds
		unsigned long t = millis();
		if (t - lastReconnectAttempt > 10000L) {
			lastReconnectAttempt = t;
			if (mqttConnect()) {
				lastReconnectAttempt = 0;
			}
		}
		delay(100);
		return;
	}
	mqtt.loop();
}

void MqttBrokerClass::publishMessage(const char* topic, const char* payload)
{
	mqtt.publish(topic, payload);
}

void MqttBrokerClass::subscribeTopic(const char* topic)
{
	mqtt.subscribe(topic);
}

boolean MqttBrokerClass::isMqttConnected()
{
	return mqtt.connected();
}

MqttBrokerClass MqttBroker;

