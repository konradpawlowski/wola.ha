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



TParameters settings = { "" };
TSensorValue Temp1Value = { 0,0 };
TSensorValue Temp2Value = { 0,0 };


void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	//EEPROM.begin(MEMORY_SIZE);
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
	createWebServer();
	server.begin();

	Serial.println("Server started");

	// Print the IP address
	Serial.println(WiFi.localIP());

	Serial.print("reading settings");
	//settings = ReadSettings();
	Serial.println(settings.ServerAddress);

}

void loop() {
	temp = 10.1;
	server.handleClient();

}
String GetOpctionSite() {

	// Prepare the response
	String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";

	s += "\r\n";
	s += " <h1>Ustawienia</h1> ";
	s += "<p><strong>Nazwa</strong>: <input name=\"name\" type=\"text\" />&nbsp;</p> ";
	s += "<p><strong>Identyfikator</strong>: <input name=\"id\" type=\"text\" />&nbsp;</p> ";
	s += "<input type='submit' value='Zapisz'>";
	s += "\r\n";
	s += "</html>\n";

	return s;
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
void storeStruct(void *data_source, int size)
{

	EEPROM.begin(size);
//	Serial.print("Zapis");
	for (int i = 0; i < size; i++)
	{
		
		byte data = ((byte *)data_source)[i];
		
	//	Serial.print(data);
		EEPROM.write(i, data);
		
	}
	EEPROM.commit();
//	Serial.println("Koniec - Zapis");
}

TParameters loadStruct()
{
	TParameters para;
	int size = sizeof(TParameters) ;
	byte data[sizeof(TParameters)];
	EEPROM.begin(size);
	for (int i = 0; i < size; i++)
	{

		byte data1 = EEPROM.read(i);
		//Serial.println(data1);
		((byte *)data)[i] = data1;
	}
	//Serial.println((char*)data);
	memcpy(&para, data, size);

	return para;
	
}

void printTParameters(TParameters param) {


	Serial.print("Address:");
	Serial.println(param.ServerAddress);

	Serial.println("Temp1:");
	Serial.print("Enble:");
	Serial.println(param.Temp1.Enable);
	Serial.print("Id:");
	Serial.println(param.Temp1.Id);
	Serial.print("Name:");
	Serial.println(param.Temp1.Name);
	
	Serial.print("Pin:");
	Serial.println(param.Temp1.Pin);
	Serial.print("Typ:");
	Serial.println(param.Temp1.Typ);

	Serial.println("Temp2:");
	Serial.print("Enble:");
	Serial.println(param.Temp2.Enable);
	Serial.print("Id:");
	Serial.println(param.Temp2.Id);
	Serial.print("Name:");
	Serial.println(param.Temp2.Name);
	Serial.print("Pin:");
	Serial.println(param.Temp2.Pin);
	Serial.print("Typ:");
	Serial.println(param.Temp2.Typ);


	


}