
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
//#define LED 13
#define P1 4
#define P2 5

/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)
/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

static int  IntervalTemp = 10000;
static int IntervalRelay = 1000;

//TSensorsOnOffValue ValuesOnOff[10]; // wartoœci progowa czujnika
TParameters settings = { "" };
TSensorValue Temp1Value;// = { 0,0 };
TSensorValue Temp2Value;// = { 0,0 };

TRoomSensor Sensors[10]; //wartoœci czujnikó

OneWire oneWireTemp1(TEMP1);
OneWire oneWireTemp2(TEMP2);
unsigned long iStart = 0;
unsigned long iTempStop = 0;
unsigned long iRelayStop = 0;
String ipAddress;


int i = 0;
void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	EEPROM.begin(512);
	settings = ReadSettings();
	

	if (settings.Wifi.IsWifiAp) {
		Serial.print("Tworze wifi");

		if (WiFi.softAP(settings.Wifi.Ssid,settings.Wifi.Pass))
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
		ipAddress = WiFi.localIP().toString();
	}

	
	createWebServer();
	server.begin();

	Serial.println("Server started");

	// Print the IP address
	Serial.println(ipAddress);

	Serial.print("reading settings");
	//settings = ReadSettings();
	Serial.println(settings.ServerAddress);


	// Ustawienie GPIO przekaznika
	pinMode(P1, OUTPUT);
	pinMode(P2, OUTPUT);
	digitalWrite(P1, LOW);
	digitalWrite(P2, LOW);

}

void loop() {
	iStart = millis();
	server.handleClient();

	if (iStart - iTempStop >= IntervalTemp) {
		Serial.println(F("Temp:"));
		GetTemp();
		SetTempInServer();

		iTempStop = millis();
	}

	if (iStart - iRelayStop >= IntervalRelay) {
		Serial.println(F("Set relay"));
		SetRelay();
		iRelayStop = millis();
	}
	CheckMemory();


}
void SetTempInServer() {
	char buff[100];
	if (settings.Temp1.Enable) {
		ipAddress.toCharArray(Sensors[settings.Temp1.Id].Address, 16);
		String(settings.Temp1.Name).toCharArray(Sensors[settings.Temp1.Id].Name, 20);
		Sensors[settings.Temp1.Id].Temp = Temp1Value.Temp;
		Sensors[settings.Temp1.Id].Time = millis();


	}

	if (settings.Temp2.Enable) {
		ipAddress.toCharArray(Sensors[settings.Temp2.Id].Address, 16);
		String(settings.Temp2.Name).toCharArray(Sensors[settings.Temp2.Id].Name, 20);
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
byte CheckTempSensor(int i) {
	byte val = LOW;

	// sprawdzenie czasu nieaktywnosci czujnika
	// jesli czas ==0 lub czas nieaktywanosci wynosi wiecej niz 10 min
	if (Sensors[i].Time == 0 || ((millis() - Sensors[i].Time) / 1000) > 600)
		val = LOW;
	else {
		if (Sensors[i].IsOutside) {
			if (Sensors[i].Temp > settings.sens[i].Temp)
				return LOW;
			else
				val = HIGH;
		}
		else
		{
			// jesli jest w³aczony piec
			/*Serial.print(digitalRead(settings.SelectedRelay));
			Serial.print(" temp z czujnika ");
			Serial.print(Sensors[i].Temp);
			Serial.print(" temp ustalona ");
			Serial.println(settings.sens[i].Temp);*/
			if (digitalRead(settings.SelectedRelay)) {
				if (Sensors[i].Temp > settings.sens[i].Temp + 0.5)
				{
					val = LOW;
					/*Serial.print(Sensors[i].Temp);
					Serial.print(" > ");
					Serial.println(settings.sens[i].Temp + 0.5);*/
				}
				else
					val = HIGH;
			}
			else {
				if (Sensors[i].Temp < settings.sens[i].Temp)
					val = HIGH;
			}
		}
	}
	return val;
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
				// jesli jest w³aczony piec
			/*	Serial.print(digitalRead(settings.SelectedRelay));
				Serial.print(" temp z czujnika ");
				Serial.print(Sensors[i].Temp);
				Serial.print(" temp ustalona ");
				Serial.println(settings.sens[i].Temp);*/

				if (digitalRead(settings.SelectedRelay)) {

				/*	Serial.print(Sensors[i].Temp > settings.sens[i].Temp + 0.5);
					Serial.print(Sensors[i].Temp);
					Serial.print(" < ");
					Serial.println(settings.sens[i].Temp + 0.5);*/

					if (Sensors[i].Temp < settings.sens[i].Temp + 0.5)
						val = HIGH;
				
				}
				else {
					/*Serial.print(Sensors[i].Temp < settings.sens[i].Temp);
					Serial.print(Sensors[i].Temp);
					Serial.print(" < ");
					Serial.println(settings.sens[i].Temp);*/
					if (Sensors[i].Temp < settings.sens[i].Temp)
						val = HIGH;


				}
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
String printDigits(byte digits) {
	String val = ":";
	// utility function for digital clock display: prints colon and leading 0
	Serial.print(":");
	if (digits < 10)
	{
		Serial.print('0');
		val += "0";
	}
	val += String(digits);
	Serial.print(digits, DEC);
	return val;
}
void CheckMemory() {
	if (ESP.getFreeHeap() < 5000)
		ESP.restart();
}
