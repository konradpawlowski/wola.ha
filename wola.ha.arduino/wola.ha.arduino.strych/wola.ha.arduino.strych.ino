#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <Wire.h>

#pragma region definitions
#define OneWirePin 2
#define TEMPERATURE_PRECISION 10
#define BLUE 13
#define GREEN 14
#define RED 12

OneWire oneWire(OneWirePin);
DallasTemperature sensors(&oneWire);

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

#pragma endregion

String ipAddress;


void setup() {
	SetupSerial();
	SetupLed();
	SetupDs182b();
	SetupWifi();

}
void loop() {
	
	CheckWifi();
	ReadTempFromDs(10000);
	SendDs18b20(60000);

}

#pragma Setup 
void SetupLed() {
	pinMode(BLUE, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(RED, OUTPUT);
	digitalWrite(BLUE, LOW);
	digitalWrite(RED, LOW);
	digitalWrite(GREEN, LOW);
}
void SetupDs182b() {
	sensors.begin();						  // locate devices on the bus
	Blink2(RED, 1, 1000);
	Sprint("Locating devices pin 2 ...");
	SensorsCount = sensors.getDeviceCount();
	Sprint("Found ");
	Sprint(SensorsCount);
	Sprintln(" devices.");
	SensorsCount = sensors.getDeviceCount();
		
	Blink2(RED, SensorsCount,500);
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
	unsigned long milis = millis();
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
		Blink(GREEN, 1);
		if (millis() - milis > 60000)
		{
			Serial.println("Restart");
			ESP.restart();
		}
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
void CheckWifi() {
	//sygnalizacja wifi
	if (WiFi.status() == WL_CONNECTED)
		digitalWrite(GREEN, HIGH);
	else
	{
		digitalWrite(GREEN, LOW);
		SetupWifi();
	}
}
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
			if (temp != -127 && temp != 85)
				temps[i] = temp;
			//	}
			Blink(RED,1);
			delay(500);
		}
		iTempStop = millis();
	}
}
bool SendMessage(String postData) {

	if (WiFi.status() == WL_CONNECTED) {   //Check WiFi connection status
		Blink(BLUE, 1);
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
		{
			Blink(BLUE, 1);
			return true;
		}
		else
		{

			Blink(BLUE, 2);
			return false;
		}

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
void Blink2(uint8_t pin, int count, int time ) {
	
	for (int i = 0; i < count; i++)
	{
		digitalWrite(pin, HIGH);
		delay(time);
		digitalWrite(pin, LOW);
		delay(time);
	}

}
void Blink(uint8_t pin, int count) {
	int time = 100;
	for (int i = 0; i < count; i++)
	{
		digitalWrite(pin, HIGH);
		delay(time);
		digitalWrite(pin, LOW);
		delay(time);
	}

}
#pragma endregion




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



