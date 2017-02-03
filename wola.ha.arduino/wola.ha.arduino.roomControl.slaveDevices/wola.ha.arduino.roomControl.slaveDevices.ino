
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
#include "EEPROMAnything.h"
#include "floatToString.h"

#define TEMP1 14 
#define TEMP2 16 

/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)
/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

TParameters settings = { "" };
TSensorValue Temp1Value;// = { 0,0 };
TSensorValue Temp2Value;// = { 0,0 };
String ipAddress;

static int  IntervalTemp = 5000;
static int IntervalSend = 10000;

unsigned long iStart = 0;
unsigned long iTempStop = 0;

unsigned long iSendStop = 0;



OneWire oneWireTemp1(TEMP1);
OneWire oneWireTemp2(TEMP2);

int i = 0;
void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	EEPROM.begin(512);
	settings = ReadSettings();


	if (settings.Wifi.IsWifiAp) {
		Serial.print("Tworze wifi");

		if (WiFi.softAP(settings.Wifi.Ssid, settings.Wifi.Pass))
		{
			IPAddress myIP = WiFi.softAPIP();
			Serial.println("... " + myIP.toString());
			ipAddress = myIP.toString();
		}
		else
			Serial.println("... nie utworzono");
	}
	else {
		WiFiManager wifiManager;
		//WiFiManager
		//Local intialization. Once its business is done, there is no need to keep it around
		//use this for auto generated name ESP + ChipID
		wifiManager.autoConnect();
		//if you get here you have connected to the WiFi
		Serial.println("connected...yeey :)");
		ipAddress = ipAddress = WiFi.localIP().toString();
	}


	createWebServer();
	server.begin();

	Serial.println("Server started");

	// Print the IP address
	Serial.println(ipAddress);

	Serial.print("reading settings");
	//settings = ReadSettings();
	Serial.println(settings.ServerAddress);

}

void loop() {
	iStart = millis();
	server.handleClient();

	if (iStart - iTempStop >= IntervalTemp) {
		Serial.println(F("Temp:"));
		GetTemp();
		iTempStop = millis();
	}

	if (iStart - iSendStop >= IntervalSend) {
		if (settings.Temp1.Enable)
			SendTemp(Temp1Value);
		if (settings.Temp2.Enable)
			SendTemp(Temp2Value);
		iSendStop = millis();
	}
	CheckMemory();

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
		Temp1Value.Id = settings.Temp1.Id;
		Temp1Value.Name = String(settings.Temp1.Name);
		Temp1Value.IsOutside = settings.Temp1.IsOutside;
		
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
		Temp2Value.Id = settings.Temp2.Id;
		Temp2Value.Name = String(settings.Temp2.Name);
		Temp2Value.IsOutside = settings.Temp2.IsOutside;
	}
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
SensorValue readTempDht(int pin) {
	SensorValue result;

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
void SendTemp(TSensorValue val) {
	char* host = settings.ServerAddress;
		
	String address = "/temp?Id=" + String(val.Id);
	address += "&Name=" + val.Name;
	address += "&Temperatura="+ String(val.Temp);
	address += "&ipAddress=" + ipAddress;
	address += "&IsOutside=" + String(val.IsOutside);
	
	Serial.print("connecting to ");
	Serial.println(host);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	const int httpPort = 80;
	if (!client.connect(host, httpPort)) {
		Serial.println("connection failed");
		return;
	}

	Serial.print("Requesting URL: ");
	Serial.println(address);

	// This will send the request to the server
	client.print(String("GET ") + address + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection: close\r\n\r\n");
	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println(">>> Client Timeout !");
			client.stop();
			return;
		}
	}

	// Read all the lines of the reply from server and print them to Serial
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.print(line);
	}

	Serial.println();
	Serial.println("closing connection");


}
String time(long val) {
	String ret = "";
	int days = elapsedDays(val);
	int hours = numberOfHours(val);
	int minutes = numberOfMinutes(val);
	int seconds = numberOfSeconds(val);

	// digital clock display of current time
	Serial.print(days, DEC);
	ret = String(days);
	ret += printDigits(hours);
	ret += printDigits(minutes);
	ret += printDigits(seconds);
	Serial.println();

	return ret;
}
void CheckMemory() {
	if (ESP.getFreeHeap() < 5000)
		ESP.restart();
}

