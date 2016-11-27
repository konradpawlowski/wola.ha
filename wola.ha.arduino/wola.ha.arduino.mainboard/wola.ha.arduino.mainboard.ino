#include <DHT.h>
#include "DallasTemperature.h"
#include <Wire.h>
#include <OneWire.h>
#include "wola.ha.enums.h"
#include "wola.ha.struct.h"

#define OneWirePin 8
#define DhtPin 4
#define TEMPERATURE_PRECISION 10
#define I2cAddress 0x41



//t_i2cMessageFrame tMessage;
t_i2cResponse Response;
OneWire oneWire(OneWirePin);
DallasTemperature sensors(&oneWire);
int SensorsCount;
float temps[10] = { 0,0,0,0,0,0,0,0,0,0 };
bool ReciveMessage = false;

void setup() {
	Serial.begin(115200);
	Serial.println("Inicjalizacja");
	Wire.begin(I2cAddress); //Jesteťmy slave numer 0x40
	Wire.onReceive(receiveEvent); //Zg│aszane przy wysy│aniu (nadawca: Write)
	Wire.onRequest(sendData); //Zg│aszane przy ┐╣daniu odczytu (nadawca: Read)


	sensors.begin();						  // locate devices on the bus
	SensorsCount = sensors.getDeviceCount();

	Serial.print("Locating devices pin8 ...");
	Serial.print("Found ");
	Serial.print(sensors.getDeviceCount(), DEC);
	Serial.println(" devices.");

	//ustawienia rozdzielczosci
	for (uint8_t i = 0; i < SensorsCount; i++)
	{
		DeviceAddress  add;
		if (!sensors.getAddress(add, i)) Serial.println("Unable to find address for Device ");
		sensors.setResolution(add, TEMPERATURE_PRECISION);
		printResolution(sensors, add);
	}




}

void loop() {

	if (!ReciveMessage) {


		Serial.println("Requesting temperatures...");
		sensors.requestTemperatures();

		for (uint8_t i = 0; i < SensorsCount; i++)
		{
			DeviceAddress  add;
			if (!sensors.getAddress(add, i))
			{
				Serial.print("Unable to find address for Device ");
				printAddress(add);
				Serial.println(".");
				continue;
			}
			printData(sensors, add);
			temps[i] = sensors.getTempC(add);
		}

		//readTempDht(4);
	}
	delay(5000);

}
#pragma region i2cComunication
void receiveEvent(int howMany) {  //Odebranie I2C, howMany - ile bajtˇw
	int j = 0;
	byte	ReceivedData[sizeof(t_i2cMessageFrame)];
	Serial.print("reciving...");
	Serial.println(howMany);
	while (Wire.available()) {
		ReceivedData[j] = Wire.read();
		j++;
	}
	ReciveMessage = true;



	t_i2cMessageFrame tMessage; //Re-make the struct
	memcpy(&tMessage, ReceivedData, sizeof(tMessage));
	printMessage(tMessage);
	recivedDataManager(tMessage);

}

void sendData() {

	const int size = sizeof(t_i2cResponse);
	char	Response2[size];
	printResponse(Response);
	memcpy(Response2, &Response, size);

	/*Serial.println("Sending respomse");
	for (size_t i = 0; i < size; i++)
	{
	Serial.print(i);
	Serial.print("\t");
	Serial.println((byte)Response2[i]);
	}*/
	Wire.write(Response2, size);

	memset(Response2, 0, size);
	memset(&Response, 0, size);

	ReciveMessage = false;
}
#pragma endregion

#pragma region ObslugaZapytania
void recivedDataManager(t_i2cMessageFrame message) {

	switch (message.Operation)
	{
	case Temp:
		Serial.println("Weszlo do temp:");
		Response = readTemp(message);
		break;
	case Write:
		if (message.Pin != 8)
		{
			pinMode(message.Pin, OUTPUT);
			digitalWrite(message.Pin, message.Value);
			Response.Status = OK;
		}
		Response.Status = WARRNING;

		break;

	case Read:
		if (message.Pin != 8)
		{
			pinMode(message.Pin, INPUT);           // set pin to input
			Response.Value = digitalRead(message.Pin);       // turn on pullup resistors
			Response.Status = OK;
		}
		Response.Status = WARRNING;
		break;

	default:
		break;

	}


}


t_i2cResponse readTemp(t_i2cMessageFrame message) {
	t_i2cResponse result;
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
	printResponse(result);

	return result;

}
t_i2cResponse readTempDs18b20(int pin, byte address[8]) {
	t_i2cResponse result;
	float temperature;
	Serial.println("Wyszukiwanie wyniku: ");


	// arrays to hold device addresses
	DeviceAddress insideThermometer;
	for (int i = 0; i < 8; i++)
	{
		insideThermometer[i] = (uint8_t)address[i];
	}
	Serial.print("Szukany adres");
	printAddress(insideThermometer);
	Serial.println(".");

	for (int i = 0; i < SensorsCount; i++)
	{
		DeviceAddress add;

		//odczytanie adresu z tablicy
		if (sensors.getAddress(add, i)) {
			Serial.print("Pozyycja: ");
			Serial.print(i);
			Serial.print("\t adres: ");
			printAddress(add);
			Serial.println(".");
			// porównanie adresu 
			if (CompareDeviceAddress(insideThermometer, add))
			{
				result.Temperature = temps[i];
				result.Humidity = 0;
				result.Status = OK;
				result.Value = false;
				Serial.print("Odczytana temperatura:");
				Serial.println(result.Temperature);
				return result;
			}
		}

	}



	result.Temperature = 0;
	result.Humidity = 0;
	result.Status = ERROR;
	result.Value = false;


	return result;
}

t_i2cResponse readTempDht(int pin) {
	t_i2cResponse result;

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
		result.Status = ERROR;
	}
	else {

		result.Temperature = t;
		result.Humidity = h;
		result.Status = OK;
	}
	return result;
}

bool CompareDeviceAddress(DeviceAddress add1, DeviceAddress add2) {
	for (int i = 0; i < 8; i++)
	{
		if (add1[i] != add2[i]) {
			return false;
		}
	}
	return true;
}



#pragma endregion




#pragma region PrintData
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

// function to print the temperature for a device
void printTemperature(DallasTemperature sens, DeviceAddress deviceAddress)
{
	float tempC = sens.getTempC(deviceAddress);
	Serial.print("Temp C: ");
	Serial.print(tempC);
	Serial.print(" Temp F: ");
	Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DallasTemperature sens, DeviceAddress deviceAddress)
{
	Serial.print("Resolution: ");
	Serial.print(sens.getResolution(deviceAddress));
	Serial.println();
}

// main function to print information about a device
void printData(DallasTemperature sens, DeviceAddress deviceAddress)
{
	Serial.print("Device Address: ");
	printAddress(deviceAddress);
	Serial.print(" ");
	printTemperature(sens, deviceAddress);
	Serial.println();
}
void printMessage(t_i2cMessageFrame message) {
	Serial.println("Message:");
	Serial.print("Operacja:");
	Serial.println(message.Operation);
	Serial.print("Typ Czujnika:");
	Serial.println(message.TempSensor);
	Serial.print("Operacja:");
	Serial.println(message.Pin);
	Serial.print("Wartość pinu:");
	Serial.println(message.Value);
}

void printResponse(t_i2cResponse response) {
	Serial.println("Response:");
	Serial.print("Wilgotnosc: ");
	Serial.print(response.Humidity);
	Serial.print("\t Temp: ");
	Serial.print(response.Temperature);
	Serial.print("\t Staus: ");
	Serial.print(response.Status);
	Serial.print("\t Value: ");
	Serial.println(response.Value);

}
#pragma endregion


