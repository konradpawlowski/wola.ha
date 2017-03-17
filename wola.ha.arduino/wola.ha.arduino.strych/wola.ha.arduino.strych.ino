#include <ESP8266HTTPClient.h>



#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <RTClib.h>

#include <Wire.h>

#pragma region definitions
#define OneWirePin 2
#define TEMPERATURE_PRECISION 10


OneWire oneWire(OneWirePin);
DallasTemperature sensors(&oneWire);
//RTC_DS1307 rtc;



#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
//#define Sprintln(a)
//#define Sprint(a) 
#pragma endregion

int SensorsCount;
float temps[10] = { 0,0,0,0,0,0,0,0,0,0 };


#pragma region LoopIntervals
unsigned long iStart = 0;
unsigned long iTempStop = 0;
unsigned long iSendStop = 0;
unsigned long iPressureStop = 0;
unsigned long iPiecStop = 0;
#pragma endregion

String ipAddress;


void setup() {
	SetupSerial();

	SetupDs182b();
//	SetupRtc();
	SetupWifi();

}
void loop() {
	ReadTempFromDs(10000);
	SendDs18b20(60000);

}

#pragma Setup
//void SetupRtc() {
//	if (!rtc.begin()) {
//		Serial.println("Couldn't find RTC");
//		while (1);
//	}
//	//rtc.adjust(DateTime(2017, 3, 12, 23, 56, 30));
//	if (!rtc.isrunning()) {
//		Serial.println("RTC is NOT running!");
//		// following line sets the RTC to the date & time this sketch was compiled
//		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//		// This line sets the RTC with an explicit date & time, for example to set
//		// January 21, 2014 at 3am you would call:
//		rtc.adjust(DateTime(__DATE__,__TIME__));
//
//
//	}
//}
void SetupDs182b() {
	sensors.begin();						  // locate devices on the bus
	
	Sprint("Locating devices pin 2 ...");
	SensorsCount = sensors.getDeviceCount();
	Sprint("Found ");
	Sprint(SensorsCount);
	Sprintln(" devices.");
	//SensorsCount = sensors.getDeviceCount();
	int i = 0;
	while (SensorsCount < 5 || i == 10)
	{
		//ESP.restart();
		SensorsCount = sensors.getDeviceCount();
		Sprint("Found ");
		Sprint(SensorsCount);
		Sprintln(" devices.");
		i++;
	}
	//ustawienia rozdzielczosci
	for (uint8_t i = 0; i < SensorsCount; i++)
	{
		DeviceAddress  add;
		if (!sensors.getAddress(add, i)) Sprintln("Unable to find address for Device ");
		sensors.setResolution(add, TEMPERATURE_PRECISION);
		printResolution(sensors, add);
	}

}
void SetupWifi() {
//	WiFiManager wifiManager;
//	//WiFiManager
//	//Local intialization. Once its business is done, there is no need to keep it around
//	//use this for auto generated name ESP + ChipID
//	wifiManager.autoConnect();
//	//if you get here you have connected to the WiFi
//	Serial.println("connected...yeey :)");
//	ipAddress = WiFi.localIP().toString();

	const char* ssid = "kondziu3";
	const char* password = "hitman85";
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void SetupSerial() {
	Serial.begin(115200);
	Sprintln("Inicjalizacja");
}



#pragma endregion
#pragma region loop
void ReadTempFromDs(unsigned long interval) {

	if (millis() - iTempStop >= interval)
	{

		Sprintln("Requesting temperatures...");
		sensors.requestTemperatures();

		for (uint8_t i = 0; i < SensorsCount; i++)
		{
			DeviceAddress  add;
			if (!sensors.getAddress(add, i))
			{
				Sprint("Unable to find address for Device ");
				printAddress(add);
				Sprintln(".");
				continue;
			}
			printData(sensors, add);
			float temp = sensors.getTempC(add);
			if(temp != -127 && temp != 85)
				temps[i] = temp;
			//	}
			delay(500);
		}
		iTempStop = millis();
	}
}



bool SendMessage(String postData) {

	if (WiFi.status() == WL_CONNECTED) {   //Check WiFi connection status

		HTTPClient http;    //Declare object of class HTTPClient

		http.begin("http://192.168.200.10:8800/api/SensorDs18b20");      //Specify request destination
		http.addHeader("Content-Type", "application/json");  //Specify content-type header

		//int httpCode = http.POST("{\"SensorType\":1,\"Address\":\"28ff8bf682160302\",\"Temperature\":-22.50}");   //Send the request
		int httpCode = http.POST(postData);
		String payload = http.getString();                                        //Get the response payload

		Serial.println(httpCode);   //Print HTTP return code
		Serial.println(payload);    //Print request response payload

		http.end();  //Close connection
		if (httpCode >= 200 && httpCode <= 300)
			return true;
		else
			return false;
	}
	else {

		Serial.println("Error in WiFi connection");
		return false;
	}

}
String CreateDs18b20Json(int i) {
	DeviceAddress add;
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();



	if (sensors.getAddress(add, i)) {
		root.set("SensorType", 1);
		root.set("Address", getStringAddress(add));
		root.set("Temperature", temps[i], 2);
		//root.set("Date", getDateTime());
	}
	String msg2;
	root.printTo(msg2);


	return msg2;
}
#pragma endregion
//String getDateTime() {
//	DateTime now = rtc.now();
//	String ret = "";
//	ret += String((int)now.year());
//	ret += "-";
//	if (now.month() < 10)
//		ret += "0";
//	ret += String(now.month());
//	ret += "-";
//	if (now.day() < 10)
//		ret += "0";
//	ret += String(now.day());
//	ret += "T";
//	if (now.hour() < 10)
//		ret += "0";
//	ret += String(now.hour());
//	ret += ":";
//	if (now.minute() < 10)
//		ret += "0";
//	ret += String(now.minute());
//	ret += ":";
//	if (now.second() < 10)
//		ret += "0";
//	ret += String(now.second());
//
//	//return "2017-03-14T20:00:01";
//
//	return ret;
//
//
//
//}

void SendDs18b20(unsigned long interval) {
	if (millis() - iSendStop >= interval)
	{
		//Serial.println("Weszlo send ds");
		for (int i = 0; i < SensorsCount; i++) {
			String sensor = CreateDs18b20Json(i);
			Serial.println(sensor);
			if (SendMessage(sensor)) {
				Serial.println("Wys³ano");
				
			}
			delay(500);
		}
		iSendStop = millis();
	}
}




	String getStringAddress(DeviceAddress deviceAddress)
	{
		String ret;
		for (uint8_t i = 0; i < 8; i++)
		{
			// zero pad the address if necessary
			if (deviceAddress[i] < 16)
				ret += "0";
			ret += (String(deviceAddress[i], HEX));
		}
		return ret;
	}
#pragma region PrintData
	// function to print a device address
	void printAddress(DeviceAddress deviceAddress)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			// zero pad the address if necessary
			if (deviceAddress[i] < 16) Sprint("0");
			Sprint(String(deviceAddress[i], HEX));
		}
	}

	// function to print the temperature for a device
	void printTemperature(DallasTemperature sens, DeviceAddress deviceAddress)
	{
		float tempC = sens.getTempC(deviceAddress);
		Sprint("Temp C: ");
		Sprint(tempC);
		Sprint(" Temp F: ");
		Sprint(DallasTemperature::toFahrenheit(tempC));
	}

	// function to print a device's resolution
	void printResolution(DallasTemperature sens, DeviceAddress deviceAddress)
	{
		Sprint("Resolution: ");
		Sprint(sens.getResolution(deviceAddress));
		Sprintln();
	}

	// main function to print information about a device
	void printData(DallasTemperature sens, DeviceAddress deviceAddress)
	{
		Sprint("Device Address: ");
		printAddress(deviceAddress);
		Sprint(" ");
		printTemperature(sens, deviceAddress);
		Sprintln();
	}



#pragma endregion


