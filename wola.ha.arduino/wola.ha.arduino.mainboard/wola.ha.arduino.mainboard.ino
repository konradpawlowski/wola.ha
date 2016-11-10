/*
Name:		IotSensor.ino
Created:	10/19/2016 10:33:10 PM
Author:	konrad
*/


#include <DHT.h>
#include "DallasTemperature.h"
#include <Wire.h>
#include <OneWire.h>
#include "wola.ha.struct.h"
#include "wola.ha.enums.h"

#define SLAVE_ADDRESS 0x40   // Define the i2c address
#define SIZE 32
#define TEMPERATURE_PRECISION 12

byte	ReceivedData[SIZE];
char	Response2[SIZE];
bool	DataReceived;


String returnMessage;
// the setup function runs once when you press reset or power the board
void setup() {
	Wire.begin(SLAVE_ADDRESS); //Jesteœmy slave numer 17
	Wire.onReceive(receiveEvent); //Zg³aszane przy wysy³aniu (nadawca: Write)
	Wire.onRequest(sendData); //Zg³aszane przy ¿¹daniu odczytu (nadawca: Read)

	Serial.begin(115200);
	Serial.println("Inicjalizacja");

	DataReceived = false;
}

// the loop function runs over and over again until power down or reset
void loop() {

}

#pragma region I2C 
void receiveEvent(int howMany) {  //Odebranie I2C, howMany - ile bajtów
	int j = 0;
	Serial.print("reciving...");
	Serial.println(howMany);
	while (Wire.available()) {
		ReceivedData[j] = Wire.read();
		j++;
	}
	DataReceived = true;
	Serial.println("Recived data:");
	Serial.println((char*)ReceivedData);



	TI2CMessageFrame tmp; //Re-make the struct
	memcpy(&tmp, ReceivedData, sizeof(tmp));

	recivedDataManager(tmp);

}

void sendData() {
	Serial.println("Sending respomse");
	Wire.write(Response2, sizeof(Response2));

	memset(Response2, 0, sizeof(Response2));

}
#pragma endregion

#pragma region Manager
void recivedDataManager(TI2CMessageFrame message) {

	switch (message.Operation)
	{
	case Read:
		Serial.println("Weszlo do read:");
		MyStruct result;
		result = readTemp(message);

		//char tempe[SIZE];
		//floatToString(Response2, result.Temperature, 2);

		memcpy(Response2, &result, sizeof(result));


		Serial.print("Humidity: ");
		Serial.print(result.Humidity);
		Serial.print(" %\t");
		Serial.print("Temperature: ");
		Serial.print(result.Temperature);
		Serial.println(" *C");

		Serial.println(Response2);
		break;

	default:
		break;
	}

}

TMyStruct readTemp(TI2CMessageFrame message) {
	TMyStruct result;
	switch (message.TempSensor)
	{
	case Dht:
		result = readTempDht(message.Pin);
		break;

	case Ds18B20:
		result = readTempDs18b20(message.Pin, message.SensorAddress);
		break;

	default:
		break;

	}


	return result;

}



TMyStruct readTempDs18b20(int pin, byte address[8]) {

	OneWire onewire(pin);
	DallasTemperature sensors(&onewire);
	float temperature;
	Serial.println("POmiar temperatury");
	sensors.begin();

	// arrays to hold device addresses
	DeviceAddress insideThermometer;
	for (int i = 0; i < 8; i++)
	{
		insideThermometer[i] = (uint8_t)address[i];
	}
	if (!onewire.search(insideThermometer)) Serial.println("Unable to find address for insideThermometer");

	// locate devices on the bus
	Serial.print("Locating devices...");
	Serial.print("Found ");
	Serial.print(sensors.getDeviceCount(), DEC);
	Serial.println(" devices.");

	// report parasite power requirements
	Serial.print("Parasite power is: ");
	if (sensors.isParasitePowerMode()) Serial.println("ON");
	else Serial.println("OFF");

	sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
	Serial.print("Device 0 Resolution: ");
	Serial.print(sensors.getResolution(insideThermometer), DEC);
	Serial.println();

	
	float tempC = 0;
	TMyStruct result;
	result.Temperature = 0;
	result.Humidity = 0;
	if (sensors.requestTemperaturesByAddress(address))
	{
		delay(500);
		tempC = sensors.getTempC(insideThermometer);


		Serial.print(tempC);
		Serial.println(F(" 'C"));


		result.Temperature = tempC;

	}
	else
	{
		Serial.println("B³ad odczytu ");
	}
	return result;
}

TMyStruct readTempDht(int pin) {
	TMyStruct result;

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
	}
	else {

		result.Temperature = t;
		result.Humidity = h;

	}
	return result;
}
#pragma endregion

