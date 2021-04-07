#include "JsonOperations.h"
String JsonOperationsClass::getJsonKeyValueFromJsonString(char* jsonString, String key)
{
#if (ARDUINOJSON_VERSION_MAJOR >= 6)
	DynamicJsonDocument json(1024);
	auto deserializeError = deserializeJson(json, jsonString);
	if (deserializeError)
	{
		Serial.println("JSON parseObject() failed");
		return false;
	}
	serializeJson(json, Serial);
#else
	DynamicJsonBuffer jsonBuffer;
	// Parse JSON string
	JsonObject& json = jsonBuffer.parseObject(jsonString);
	Serial.println("JSON  : " + (String)jsonString);
	// Test if parsing succeeds.
	if (!json.success())
	{
		Serial.println("JSON parseObject failed");
	}
	json.printTo(Serial);
#endif
	String valueOfKey = "";
	if (json.containsKey(key))
	{
		Serial.println("IT CONTAINS KEY : " + key);
		String vk = json[key];
		valueOfKey = (char*)vk.c_str();
	}
	return valueOfKey;
}


String JsonOperationsClass::getJsonKeyValueFromRootNode(char* jsonString, String rootKey)
{
	DynamicJsonBuffer jsonBuffer;

	JsonObject& jsonroot = JsonOperations.getJsonroot(jsonString);

	Serial.println("JSON  : " + (String)jsonString);
	// Test if parsing succeeds.
	if (!jsonroot.success())
	{
		Serial.println("JSON parse Object failed");
	}

	jsonroot.printTo(Serial);
	String valueOfKey = "";
	if (jsonroot.containsKey(rootKey))
	{
		Serial.println("IT CONTAINS KEY : " + rootKey);
		String vk = jsonroot[rootKey];
		valueOfKey = (char*)vk.c_str();
	}

	return valueOfKey;
}

JsonObject&   JsonOperationsClass::getJsonroot(String jsond) {
	StaticJsonBuffer<1000> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(jsond);

	if (root.success()) {
		Serial.println("parse Object  succeeded");
	}
	else {
		Serial.println("parse Object failed!");
	}

	return root;
}

JsonOperationsClass JsonOperations;
