// MemoryOperations.h

#ifndef _MEMORYOPERATIONS_h
#define _MEMORYOPERATIONS_h
#include "Keys.h"
 
#ifdef ESP32
#include "SPIFFS.h"

#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#define getMachineID()   ((uint32_t)ESP.getEfuseMac())
#define LED_ON      HIGH
#define LED_OFF     LOW  
#else  
#define LED_ON      LOW //jarur nathi 
#define LED_OFF     HIGH// jarur nathi 
#endif
#include "Keys.h"
#include <ArduinoJson.h>  

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

 
#include "SD.h"
#include "ExternVars.h"
#define countof(a) (sizeof(a) / sizeof(a[0]))// a na content ,a na contnt nu starting address


 
class MemoryOperationsClass
{
protected:


public:
 
	uint16_t n = 0;
	bool _SDCARD_ACTIVE = 0;
	void  init();
	bool readConfigFile();
	bool writeConfigFile();
	void deleteConfigFile();


	///////////////MEMORY CARD 
	// void read_del();
	String  readFile(fs::FS &fs, const char * path);
	void  deleteFile(fs::FS &fs, const char * path);
	void  listDir(fs::FS &fs, const char * dirname, uint8_t levels);
	void writeFile(fs::FS &fs, const char * path, String message);
	void  createDir(fs::FS &fs, const char * path);
	void make_write(String indata_fail);


};

extern MemoryOperationsClass MemoryOperations;

#endif

