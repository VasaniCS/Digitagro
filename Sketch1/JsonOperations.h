// JsonOperations.h

#ifndef _JSONOPERATIONS_h
#define _JSONOPERATIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include <ArduinoJson.h>  
class JsonOperationsClass
{
protected:


public:
	String getJsonKeyValueFromJsonString(char* jsonString, String key);
	String  getJsonKeyValueFromRootNode(char* jsonString, String rootKey);
	JsonObject&   getJsonroot(String jsond);
};

extern JsonOperationsClass JsonOperations;

#endif

