#include "MemoryOperations.h"
#define FS_NO_GLOBALS
#include <FS.h> 
#include "Keys.h"


//////////////////////////////////SPIFF
bool MemoryOperationsClass::readConfigFile()
{
	if (!SPIFFS.begin()) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return false;
	}
	// this opens the config file in read-mode
	Serial.println("CONFIGURATION FILE OPEN START------------");
	FileS  f = SPIFFS.open(CONFIG_FILE, "r");

	if (!f)
	{
		Serial.println("Configuration file not found");
		//writeConfigFile();
		return false;
	}
	else
	{
		// we could open the file
		size_t size = f.size();
		// Allocate a buffer to store contents of the file.
		std::unique_ptr<char[]> buf(new char[size]);

		// Read and store file contents in buf
		f.readBytes(buf.get(), size);
		// Closing file
		f.close();
		Serial.println("FILE READ AND CLOSED--------------");

#if (ARDUINOJSON_VERSION_MAJOR >= 6)
		DynamicJsonDocument json(1024);
		auto deserializeError = deserializeJson(json, buf.get());
		if (deserializeError)
		{
			Serial.println("JSON parseObject() failed");
			return false;
		}
		serializeJson(json, Serial);
#else
		DynamicJsonBuffer jsonBuffer;
		// Parse JSON string
		JsonObject& json = jsonBuffer.parseObject(buf.get());
		// Test if parsing succeeds.
		if (!json.success())
		{
			Serial.println("JSON parseObject() failed");
			return false;
		}
		json.printTo(Serial);
#endif
		Serial.println("------------READING KEYS FROM FILE DATA--------------");
		// Parse all config file parameters, override 
		// local config variables with parsed values
		if (json.containsKey(key1_machineId))
		{
			String mi = json[key1_machineId];
			MachineId = (char*)mi.c_str();
			Serial.println("-----------KEY1 READ--------------\n" + mi);
		}
		if (json.containsKey(key2_authkey))
		{
			String au = json[key2_authkey];
			authkey=(char*)au.c_str();
			Keys.setAuthKey(authkey);
			Serial.println("-----------KEY2 READ AU--------------\n" +(String)authkey);

			 
			//Serial.println("-----------KEY2 READ AU --------------\n" + au);
		}
		if (json.containsKey(key3_oxParam))
		{
			String op = json[key3_oxParam];
			oxParam = (char*)op.c_str();
			Serial.println("-----------KEY3 READ--------------\n" + op);
		}
		if (json.containsKey(key4_phParam))
		{
			String ph = json[key4_phParam];
			phParam = (char*)ph.c_str();
			Serial.println("-----------KEY4 READ--------------\n" + ph);
		}
		if (json.containsKey(key5_brokerName))
		{
			String b = json[key5_brokerName];
			//Keys.broker = "prawn - test.optimustech.io";//(char*)b.c_str();
			Serial.println("-----------KEY5 READ--------------\n" +(String)broker);
		}
		if (json.containsKey(key6_isRegistered))
		{
			String r = json[key6_isRegistered];
			isRegistered = (char*)r.c_str();
			Serial.println("-----------KEY6 READ--------------\n" + r);
		}
	}

	SPIFFS.end();

	Serial.println("\nConfig file was successfully parsed-************************");
	return true;
}

bool MemoryOperationsClass::writeConfigFile()
{

	Serial.println("Saving config file");
	if (!SPIFFS.begin()) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return  false;
	}

#if (ARDUINOJSON_VERSION_MAJOR >= 6)
	DynamicJsonDocument json(1024);
#else
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
#endif

	// JSONify local configuration parameters
	json[key1_machineId] = MachineId;
	json[key2_authkey] = authkey;
	json[key3_oxParam] = oxParam;
	json[key4_phParam] = phParam;
	json[key5_brokerName] = broker;
	json[key6_isRegistered] = isRegistered;

	 


	// Open file for writing
	fs::FileS f = SPIFFS.open(CONFIG_FILE, "w");

	if (!f)
	{
		Serial.println("Failed to open config file for writing");
		return false;
	}

#if (ARDUINOJSON_VERSION_MAJOR >= 6)
	serializeJsonPretty(json, Serial);
	// Write data to file and close it
	serializeJson(json, f);
#else
	json.prettyPrintTo(Serial);
	// Write data to file and close it
	json.printTo(f);
#endif

	f.close();
	SPIFFS.end();
	Serial.println("\nConfig file was successfully saved-----+++++++++++++++");
	return true;
}

void MemoryOperationsClass::deleteConfigFile()
{
	if (!SPIFFS.begin()) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}
	fs::FileS f = SPIFFS.open(CONFIG_FILE, "w");

	if (!f)
	{
		Serial.println("Failed to open config file for writing");
		return;
	}

	SPIFFS.remove(CONFIG_FILE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////MEMORYCARD

void MemoryOperationsClass::init()
{
	if (!SD.begin()) {
		Serial.println("Card Mount Failed");
		return;
	}
	uint8_t cardType = SD.cardType();
	_SDCARD_ACTIVE = 1;
	if (cardType == CARD_NONE) {
		Serial.println("No SD card attached");
		return;
	}

	Serial.print("SD Card Type: ");
	if (cardType == CARD_MMC) {
		Serial.println("MMC");
	}
	else if (cardType == CARD_SD) {
		Serial.println("SDSC");
	}
	else if (cardType == CARD_SDHC) {
		Serial.println("SDHC");
	}
	else {
		Serial.println("UNKNOWN");
	}

	uint64_t cardSize = SD.cardSize() / (1024 * 1024);
	Serial.printf("SD Card Size: %lluMB\n", cardSize);
	//Serial.print("ACTIVE STATUS : ");
	Serial.println(_SDCARD_ACTIVE);
	if (_SDCARD_ACTIVE == 1)
	{
		Serial.println("SD CARD MOUNTED");
		//	
		//	createDir(SD, "/");
		//	//String fileName = "/PAWN/test1" + (String)_FEEDDATA_FILE_EXTENSION;
		////	memOp.writeFile(SD, (char*)fileName.c_str(), "THIS IS TEST DATA ######## CHETAN #####AMIT####PARTH%%%%");
		//	Serial.println("------LISTING DIRECTORIES-----");
		//	listDir(SD, "/PAWN", 0);
		//	//	SerialMon.println("------READING FILE-----");
		////		memO p.readFile(SD, (char*)fileName.c_str());*/
	//	Serial.println("------MEMORY CARD INITIALIZATION COMPLETED****************************-----");
	}
	else
	{
		Serial.println("------//////////////////SD CARD NOT MOUNTED//////////////****************-----");
	}

}


void MemoryOperationsClass::make_write(String indata_fail)
{
	//CHECK MEMORY CARD AVAILABLE OR NOT
	if (_SDCARD_ACTIVE == 1)
	{
		//Serial.println("----------------------------------DT OBJECT CREATED------------------------------");
		char filenameDtstring[9];//[50];
		char filenametmstring[7];
		// char filenameDtstring[3];
		time_t ltime;
		time(&ltime);/* Get the time in seconds */
		struct tm * tmstruct = localtime(&ltime);/* Convert it to the structure tm */

		int hr = tmstruct->tm_hour;
		int mi = (tmstruct->tm_min) + 30;

		if (mi >= 60)
		{
			mi -= 60;
			hr += 1;
		}
		// snprintf_P(filenameDtstring, countof(filenameDtstring), PSTR("%02u%02u%02u/%02u%02u%02u/"),year(),month(), day(),hour(), minute(), second());
		snprintf_P(filenameDtstring, countof(filenameDtstring), PSTR("/%02u%02u%02u/"), (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday);//date
		snprintf_P(filenametmstring, countof(filenametmstring), PSTR("/%02u%02u%02u/"), hr, mi, tmstruct->tm_sec);//time

		// snprintf_P(filenameDtstring, countof(filenameDtstring), PSTR("%02u%02u%02u%02u.CSV"), dt.Day(), dt.Hour(), dt.Minute(), dt.Second());
		// snprintf_P(filenameDtstring, countof(filenameDtstring), PSTR("%02u%02u%02u"),dt.Second(), dt.Minute(),dt.Hour());
		//snprintf_P(filenameDtstring, countof(filenameDtstring), PSTR("%02u"),dt.Hour());

		Serial.println("FILE OPENED SUCCESSFULLY NAME IS :");
		//------------daty directory create karva type convertion ( ch arry ->const char *) --------------------------
		char* date = filenameDtstring;//ch arry to char*.char*->date, ch araay-> filenameDtstring
		std::string str_date = date;//char* to string(small) convert. char*->date , string(small)- str_date
		//const char *cstr_date = str_date.c_str();//string(small) to const char *.string(small)->str_date , const char *->cstr_date

		//------------time directory create karva type convertion extra( ch arry ->const char *) --------------------------
		char* Time = filenametmstring;//ch arry to char*.char*->Time, ch araay-> filenametmstring
		std::string str_Time = Time;//char* to string(small) convert. char*->Time , string(small)- str_Time
		const char *cstr_Time = str_Time.c_str();//string(small) to const char *.string(small)->str_Time , const char *->cstr_Time
		// struct tm tmstruct1 ;
		// getLocalTime(&tmstruct1, 5000);

		 //changeDir(SD, cstr_date);

		//    if (esp_dt !=day())//esp clock no day change thayo
		//    {
		//     // wrfile.timestamp(T_WRITE, year(), month(), day(), hour(), minute(), second());
		//      if (sd.chdir("/")){Serial1.println("root directory path");}//working dir
		//      if (sd.mkdir(cstr_date)){Serial1.println("date dir make");}//make dir exist hase to nai banave
		//      if (sd.chdir(cstr_date)){Serial1.println("date dir path");}//working dir
		////       sd.chdir("/");//       sd.mkdir(cstr_date);//make dir//       sd.chdir(cstr_date);//working dir//       Serial1.println("directory create");
		//       esp_dt = day();
		//    }
		//
		char folder_name[34] = "/PAWN";
		const char *cstr_date = folder_name;//folder name
		createDir(SD, cstr_date);//folder create
		String filename = "";
		filename = String(filenametmstring) + String("_") + String(random(9999999)) + String(".txt");
		Serial.println(filename);
		//   strcat(myBigArray, myLittleArrayOne);
		char time_random[29];                      //String to const char * convert
		filename.toCharArray(time_random, 29);     //String to ch araay convert         filename->String       , time_random->charray[]
		strcat(folder_name, time_random);
		std::string str_time_random = folder_name;//ch araay to string(small) convert  time_random->charray[] , string(small)- str_time_random
		const char *cstr_time_random = str_time_random.c_str();//string(small) to const char * string(small)->str_time_random , const char *->cstr_time_random
		// struct tm tmstruct2 ;
		//  getLocalTime(&tmstruct2, 5000);
		MemoryOperations.writeFile(SD, cstr_time_random, indata_fail);

		//FileS wrfile(cstr_time_random, O_WRITE | O_CREAT | O_TRUNC);
		////
		//if (wrfile.isOpen())
		//{
		//	wrfile.print(indata_fail);
		//	Serial1.println("file write done");
		//	// wrfile.timestamp(T_WRITE, year(), month(), day(), hour(), minute(), second());
		//	wrfile.close();
		//}
		//else
		//{
		//	Serial1.println(F(" wr file error"));
		//}

	}
}


void MemoryOperationsClass::createDir(fs::FS &fs, const char * path) {

	Serial.printf("Creating Dir: %s\n", path); 
	if (!fs.exists(path))
	{
		if (fs.mkdir(path))
		{
			Serial.println("Dir created");
		}
		else 
		{
			Serial.println("mkdir failed");
		}
	}
	else {
		Serial.println("DIRECTORY "+ (String)path +" ALREADY EXISTS ");
	}
}

void MemoryOperationsClass::writeFile(fs::FS &fs, const char * path, String message) {
	Serial.printf("Writing file: %s\n", path);
	FileS file =fs.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("Failed to open file for writing");
		return;
	}
	if (file.print(message)) {
		Serial.println("File written");
	}
	else {
		Serial.println("Write failed");
	}
	file.close();
}
//
void MemoryOperationsClass::listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\n", dirname);
	FileS  root = fs.open(dirname);
	if (!root) //namni directory nahi hoy to ama jase
	{
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		Serial.println("Not a directory");
		return;
	}

	fs::FileS file = root.openNextFile();

	while (file) //jya sudhi file hase tya sudhi ahi rese
	{
		if (file.isDirectory()) //directory nu name 6e ke andar ni file nu name 6e
		{
			Serial.print("  DIR : ");
			Serial.println(file.name());  //  /PAWN ahi malse

			if (levels)
			{
				listDir(fs, file.name(), levels - 1);
			}
		}
		else
		{
			Serial.print("  FILE: ");
			Serial.println(file.name());//singe file nu name malse
			//File file1 = file.name();
			fs::FileS file1 = fs.open(file.name());

			if (file1)//file.isSubDir() ||sd.ls(LS_R);//dir and file list
			{
				Serial.println("-+++++++++++++++++++++++++++++++-myfile OBJECT CREATED for READ+++++++++++++++++++++++++++++++++++++");
				// char fBuffer[50]="";
				// F1.getName(fBuffer,50);//f buff ma file nu name nakhvanu 6e
				// Serial.println(fBuffer);
				const char* ext = strchr(file.name(), '.');
				if (strcmp(ext, ".txt") == 0)
				{
					Serial.println("FILE AVAILABLE CHE");
					String dataBuff = "";
					int16_t datab;
					Serial.println("READ KARVA JAAY CHE");
					while ((datab = file1.read()) >= 0) // read from the file until there's nothing else in it:
					{
						dataBuff += (char(datab));
					}
					Serial.println("-+++++++++++++++++++++++++++++++++CALLING PING_ONLYGET with data:");
					Serial.flush();
					Serial.println(dataBuff);//file name ave6e 221158_4255914.CSVheloo
					Serial.print("  SIZE: ");
					Serial.println(file.size());

					//              dataBuff="";
					//              file1.close();
					//              Serial.print("FILE NAME IS:--====--");
					//
					//              if( fs.remove(file.name()))
					//              {
					//               Serial.println("File deleted");
					//              }
					//               else
					//              {
					//                Serial.println("Delete failed");
					//              }


				}
				else
				{
					file1.close();
				}

			}
			file = root.openNextFile();//next file nu name malse jo folder hase to folder ma jase pa6i teni file lese
		}
	}
}
//
void MemoryOperationsClass::deleteFile(fs::FS &fs, const char * path)
{

	Serial.printf("Deleting file: %s\n", path);

}

//
String MemoryOperationsClass::readFile(fs::FS &fs, const char * path) {
	String fileData = "";
	Serial.printf("Reading file: %s\n", path);

	fs::FileS file = fs.open(path);

	if (!file) {
		Serial.println("Failed to open file for reading");
		return fileData;
	}

	Serial.println("+++++++++++Read from file:++++++++++++++ ");
	while (file.available()) {
		fileData = file.readString();
		Serial.println(fileData);
	}
	file.close();
	return fileData;
}

//
//void MemoryOperationsClass::read_del()
//{
//	listDir(SD, "/_MEMORYCARD_DATA_FOLDERNAME", 0);
//	//_SDCARD_ACTIVE VARIABLE FOR CHECK MEMORY CARD WORKING OR NOT
//	if (_SDCARD_ACTIVE == 1)
//	{
//
//	}
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



MemoryOperationsClass MemoryOperations;

