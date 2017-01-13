
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <EEPROM.h>
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

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "global.h"
#include "floatToString.h"

#define TEMP1 14 
#define TEMP2 16 
#define P1 4
#define P2 5

//TSensorsOnOffValue ValuesOnOff[10]; // wartoœci progowa czujnika
TParameters settings = { "" };
TSensorValue Temp1Value;// = { 0,0 };
TSensorValue Temp2Value;// = { 0,0 };

TRoomSensor Sensors[10] ; //wartoœci czujnikó

OneWire oneWireTemp1(TEMP1);
OneWire oneWireTemp2(TEMP2);

int i = 0;
void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	EEPROM.begin(512);
	//WiFiManager
	//Local intialization. Once its business is done, there is no need to keep it around
	WiFiManager wifiManager;
	//reset saved settings
	//  wifiManager.resetSettings();

	//set custom ip for portal
	//   wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

	//fetches ssid and pass from eeprom and tries to connect
	//if it does not connect it starts an access point with the specified name
	//here  "AutoConnectAP"
	//and goes into a blocking loop awaiting configuration
	wifiManager.autoConnect("AutoConnectAP");
	//or use this for auto generated name ESP + ChipID
	//wifiManager.autoConnect();


	//if you get here you have connected to the WiFi
	Serial.println("connected...yeey :)");
	settings = ReadSettings();
	
	


	if (settings.Temp1.Typ == Ds18b20)
	{

		DallasTemperature sensorsTemp1(&oneWireTemp1);
	}
	if (settings.Temp1.Typ == Ds18b20)
	{
		OneWire oneWireTemp1(settings.Temp1.Pin);
		DallasTemperature sensorsTemp1(&oneWireTemp1);
	}



	createWebServer();
	server.begin();

	Serial.println("Server started");

	// Print the IP address
	Serial.println(WiFi.localIP());

	Serial.print("reading settings");
	//settings = ReadSettings();
	Serial.println(settings.ServerAddress);


	// Ustawienie GPIO przekaznika
	pinMode(P1, OUTPUT);
	pinMode(P2, OUTPUT);
}

void loop() {
	i++;
	server.handleClient();

	if (i == 100) {
		Serial.println("Temp:");
		GetTemp();
		i = 0;
		SetTempInServer(); 
		SetRelay();
	}
	
	
	delay(100);
}
void SetTempInServer() {
	char buff[100];
	if (settings.Temp1.Enable) {
		WiFi.localIP().toString().toCharArray(Sensors[settings.Temp1.Id].Address, 16);
		String(settings.Temp1.Name).toCharArray(Sensors[settings.Temp1.Id].Name,20);
		Sensors[settings.Temp1.Id].Temp = Temp1Value.Temp;
		Sensors[settings.Temp1.Id].Time = millis();
		

	}

	if (settings.Temp2.Enable) {
		WiFi.localIP().toString().toCharArray(Sensors[settings.Temp2.Id].Address, 16);
		String(settings.Temp2.Name).toCharArray(Sensors[settings.Temp2.Id].Name,20);
		Sensors[settings.Temp2.Id].Temp = Temp2Value.Temp;
		Sensors[settings.Temp2.Id].Time = millis();

	}
}
void GetTemp() {
	if (settings.Temp1.Enable) {
		switch (settings.Temp1.Typ)
		{
		case Ds18b20:
			Temp1Value.Temp = GetTempDs18b20(Temp1);
			break;
		case Dht:
			Temp1Value = readTempDht(settings.Temp1.Pin);
			break;

		}
	}
	if (settings.Temp2.Enable) {
		switch (settings.Temp2.Typ)
		{
		case Ds18b20:
			Temp2Value.Temp = GetTempDs18b20(Temp2);
			break;
		case Dht:
			Temp2Value = readTempDht(settings.Temp2.Pin);
			break;

		}
	}
}
void SaveSettings(TParameters param) {
	storeStruct(&param, sizeof(param));
	//EEPROM_writeAnything(0, param);
	Serial.println("Zapis");
	printTParameters(param);
}
TParameters ReadSettings() {

	TParameters param;

	param = loadStruct();
	//EEPROM_readAnything(0, param);
	Serial.println("Odczyt");
	printTParameters(param);

	return param;
}
float GetTempDs18b20(TempSensorEnum temp) {
	DallasTemperature sensors;

	switch (temp)
	{
	case Temp1:
		sensors.setOneWire(&oneWireTemp1);
		break;
	case Temp2:
		sensors.setOneWire(&oneWireTemp2);
		break;

	}

	// Request temperature conversion (traditional)
	Serial.println("Before blocking requestForConversion");
	unsigned long start = millis();

	sensors.requestTemperatures();

	unsigned long stop = millis();
	Serial.println("After blocking requestForConversion");
	Serial.print("Time used: ");
	Serial.println(stop - start);

	// get temperature
	Serial.print("Temperature: ");
	Serial.println(sensors.getTempCByIndex(0));
	Serial.println("\n");
	Serial.println("Address:");
	Serial.print(sensors.getDeviceCount(), DEC);
	// Request temperature conversion - non-blocking / async
	Serial.println("Before NON-blocking/asyndc requestForConversion");
	start = millis();
	sensors.setWaitForConversion(false);  // makes it async
	sensors.requestTemperatures();
	sensors.setWaitForConversion(true);
	stop = millis();
	Serial.println("After NON-blocking/async requestForConversion");
	Serial.print("Time used: ");
	Serial.println(stop - start);


	// 9 bit resolution by default 
	// Note the programmer is responsible for the right delay
	// we could do something usefull here instead of the delay
	int resolution = 10;
	delay(750 / (1 << (12 - resolution)));

	// get temperature
	Serial.print("Temperature: ");
	Serial.println(sensors.getTempCByIndex(0));
	Serial.println("\n\n\n\n");
	return sensors.getTempCByIndex(0);
}
TSensorValue readTempDht(int pin) {
	TSensorValue result;

	int type = 0;

	Serial.print("Dht type: ");
	Serial.print(type);
	Serial.print("    pin: ");
	Serial.print(pin);

	//DHT dht(pin, type);
	DHT dht;
	dht.setup(pin);
	delay(dht.getMinimumSamplingPeriod());
	float h = dht.getHumidity();
	float t = dht.getTemperature();
	if (isnan(t) || isnan(h)) {
		Serial.println("Failed to read from DHT");
		Serial.println(dht.getStatusString());

	}
	else {

		result.Temp = t;
		result.Humi = h;

	}
	return result;
}
byte CheckTemp() {
	byte val = LOW;
	for (int i = 0; i < settings.CountOfSensors; i++) {

		// sprawdzenie czasu nieaktywnosci czujnika
		// jesli czas ==0 lub czas nieaktywanosci wynosi wiecej niz 10 min
		if (Sensors[i].Time == 0 || ((millis() - Sensors[i].Time) / 1000) > 600)
			val = LOW;
		else {
			if (Sensors[i].IsOutside) {
				if (Sensors[i].Temp > settings.sens[i].Temp)
					return LOW;

			}
			else
			{
				if (Sensors[i].Temp < settings.sens[i].Temp)
					val = HIGH;
			}
		}
	}
	return val;
}
void SetRelay() {
	byte value = CheckTemp();
	
	switch (settings.SelectedRelay) {
	case Relay1:
		digitalWrite(P1, value);
		break;
	case Relay2:
		digitalWrite(P2, value);
		break;
	
	}
	Serial.print("Przekaznik: ");
	Serial.println(value);
}
