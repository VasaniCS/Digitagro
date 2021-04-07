#include "GSMOpetarions.h"

HardwareSerial hwSerial(1);
TinyGsm modem(hwSerial);
TinyGsmClient GsmClient(modem);
PubSubClient m(GsmClient);
const char apn[] = "  www";
const char gprsUser[] = "";
const char gprsPass[] = "";


void GSMOperationsClass::init()
{
	mqttSC = m;
	SerialMon.println("Wait...");
	hwSerial.begin(115200, SERIAL_8N1, 4, 2, false);
	modem.restart();
	delay(3000);
	SerialMon.println("Initializing modem...");
	modem.restart();
	// modem.init();
	String modemInfo = modem.getModemInfo();
	SerialMon.print("Modem Info: ");
	SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
	// Unlock your SIM card with a PIN if needed
	if (GSM_PIN && modem.getSimStatus() != 3) {
		modem.simUnlock(GSM_PIN);
	}
#endif
	//
	//#if TINY_GSM_USE_WIFI
	//    // Wifi connection parameters must be set before waiting for the network
	//  SerialMon.print(F("Setting SSID/password..."));
	//  if (!modem.networkConnect(wifiSSID, wifiPass)) {
	//    SerialMon.println(" fail");
	//    delay(10000);
	//    return;
	//  }
	//  SerialMon.println(" wifi success");
	//#endif

#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
  // The XBee must run the gprsConnect function BEFORE waiting for network!
	modem.gprsConnect(apn, gprsUser, gprsPass);
#endif

	SerialMon.print("Waiting for network...");
	if (!modem.waitForNetwork()) {
		SerialMon.println(" Network fail");
		delay(10000);
		return;
	}
	SerialMon.println(" Gsm success");

	if (modem.isNetworkConnected()) {
		SerialMon.println("Network connected");
	}

#if TINY_GSM_USE_GPRS
	// GPRS connection parameters are usually set after network registration
	SerialMon.print(F("Connecting to "));
	SerialMon.print(apn);
	if (!modem.gprsConnect(apn, gprsUser, gprsPass))
	{
		SerialMon.println(" GPRS Connection fail");
		delay(10000);
		return;
	}
	SerialMon.println("GPRS success");

	if (modem.isGprsConnected()) {
		SerialMon.println("GPRS connected");
	}
#endif

}

boolean GSMOperationsClass::isGsmConnected()
{
	return (modem.isGprsConnected());
}


String GSMOperationsClass::getGSMTime()
{
	TinyGSMDateTimeFormat get;
	
	return	modem.getGSMDateTime(get);
}

GSMOperationsClass GSMOperations;

