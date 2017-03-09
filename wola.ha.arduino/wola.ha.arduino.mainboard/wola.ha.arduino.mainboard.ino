#include <RTClib.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <DallasTemperature\DallasTemperature.h>
#include <Wire.h>
#include <OneWire.h>
#include "wola.ha.enums.h"
#include "FastCRC.h"
#include "wola.ha.struct.h"
#include <math.h>


//#define Sprintln(a) (Serial.println(a))
//#define Sprint(a) (Serial.println(a))
#define Sprintln(a)
#define Sprint(a) 


#define OneWirePin 4
#define DhtPin 8
#define TEMPERATURE_PRECISION 10
//#define I2cAddress 0x40

#define IdentityDevice 1

t_i2cResponse Response;
OneWire oneWire(OneWirePin);
DallasTemperature sensors(&oneWire);
Adafruit_BMP085 bmp;
RTC_DS1307 rtc;

int SensorsCount;
float temps[10] = { 0,0,0,0,0,0,0,0,0,0 };
bool ReciveMessage = false;

#pragma region LoopIntervals
unsigned long iStart = 0;
unsigned long iTempStop = 0;
unsigned long iSendStop = 0;
unsigned long iPressureStop = 0;
#pragma endregion
String inputString;

void setup() {
	SetupSerial();
	SetupRtc();
	SetupDs182b();
	SetupBmp180();
}




void loop() {

	ReadTempFromDs(30000);
	SendData(60000);

	if (ReciveMessage) {
		Sprintln(inputString);
		ReciveMessage = false;
	}
}



#pragma Setup
void SetupRtc() {
	if (!rtc.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}

	if (!rtc.isrunning()) {
		Serial.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		//  rtc.adjust(DateTime(2017 , 2, 26, 17, 0, 0));
	}
}
void SetupDs182b() {
	sensors.begin();						  // locate devices on the bus
	SensorsCount = sensors.getDeviceCount();

	Sprint("Locating devices pin8 ...");
	Sprint("Found ");
	Sprint(String(sensors.getDeviceCount(), DEC));
	Sprintln(" devices.");

	//ustawienia rozdzielczosci
	for (uint8_t i = 0; i < SensorsCount; i++)
	{
		DeviceAddress  add;
		if (!sensors.getAddress(add, i)) Sprintln("Unable to find address for Device ");
		sensors.setResolution(add, TEMPERATURE_PRECISION);
		printResolution(sensors, add);
	}

}
void SetupSerial() {
	Serial.begin(115200);
	Sprintln("Inicjalizacja");
}
void SetupBmp180() {
	if (!bmp.begin())
	{
		Sprintln("Nie odnaleziono czujnika BMP085 / BMP180");
		//while (1) {}
	}
}

#pragma endregion

#pragma region Loop


// odczytywanie temperatury z czujnikow ds
void ReadTempFromDs(unsigned long interval) {

	if (millis() - iTempStop >= interval)
	{
		//if (!ReciveMessage) {


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
			temps[i] = sensors.getTempC(add);
			//	}

		}
		iTempStop = millis();
	}
}
void SendData(unsigned long interval) {
	Sprintln("weszlo do send");
	Sprint(millis() - iSendStop);
	Sprint(">=");
	Sprint(interval);
	if (millis() - iSendStop >= interval)
	{
		Sprintln("Wysylam");
		SendDs18b20();
		delay(200);
		SendDht();
		delay(200);

		SendBmp180();

		
		iSendStop = millis();

	}
}

#pragma endregion

#pragma region serial communication
void serialEvent() {

	while (Serial.available()) {
		// get the new byte:
		char inChar = (char)Serial.read();
		// add it to the inputString:
		inputString += inChar;

		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\n') {
			ReciveMessage = true;
		}

	}
	//println(inputString);

}
void SendDs18b20() {
	//Serial.println("Weszlo send ds");
	for (int i = 0; i < SensorsCount; i++) {
		String sensor = CreateDs18b20Json(i);
		String msg = PrepareMessage(SensorValues, Ds18B20, sensor);
		Serial.println(msg);
		delay(200);
	}

}
void SendBmp180() {
	//Serial.println("Weszlo send ds");
	String sensor = CreateBmp180Json();
	String msg = PrepareMessage(SensorValues, BMP180, sensor);
	Serial.println(msg);
	//delay(1000);

}
void SendDht() {
	//Serial.println("Weszlo send dht");
	String sensor = CreateDhtJson();
	String msg = PrepareMessage(SensorValues, Dht22, sensor);
	Serial.println(msg);
	//delay(1000);
}

//
//#pragma region i2cComunication
//void receiveEvent(int howMany) {  //Odebranie I2C, howMany - ile bajtˇw
//
//
//
//	int j = 0;
//	//const int SizeAll = sizeof(t_i2cMessageFrame) + 4;
//	int size = sizeof(t_i2cMessageFrame);
//	byte *RecivedData;
//	byte *MessageData;
//
//
//	RecivedData = new byte[howMany];
//	MessageData = new byte[size];
//	byte *wReciveData = RecivedData;
//
//
//	ReciveMessage = true;
//
//	Sprint("reciving...");
//	Sprintln(howMany);
//	while (Wire.available()) {
//		*wReciveData = Wire.read();
//		Sprint((int)wReciveData);
//		Sprint(":");
//		Sprintln(*wReciveData);
//		wReciveData++;
//	}
//	Sprintln("::: Koniec odczytu");
//
//	int verification = VerifiReciveMessage(RecivedData, howMany);
//	if (verification == OK)
//	{
//		Sprintln("Weryfikacja crc OK");
//
//
//		byte *wMessageData = MessageData;
//		byte *wReciveData = &RecivedData[2];
//
//		Sprintln("Przepisanie danych do tablicy CRC:");
//		for (int i = 0; i < (int)RecivedData[0]; i++)
//		{
//			*wMessageData = *wReciveData;
//			Sprint((int)wMessageData);
//			Sprint(" :: ");
//			Sprintln(*wMessageData);
//			wMessageData++;
//			wReciveData++;
//		}
//		Sprintln("Koniec przepisywania");
//		t_i2cMessageFrame tMessage; //Re-make the struct
//		memcpy(&tMessage, MessageData, sizeof(tMessage));
//		//printMessage(tMessage);
//		delete[] RecivedData;
//		delete[] MessageData;
//		//printMessage(tMessage);
//		recivedDataManager(tMessage);
//	}
//	else
//	{
//		Response.Humidity = 0;
//		Response.Temperature = 0;
//		Response.Value = 0;
//		Response.Status = verification;
//		sendDataBySerial(&Response,sizeof(Response));
//	}
//
//}
//
//void sendDataBySerial(void *message, int size) {
//	//const int size = sizeof(t_i2cResponse);
//	const int SizeAll = size + 4;
//	const int SizeCrc = size + 2;
//
//	byte bSize = size;
//	FastCRC16 CRC16;
//
//	byte	*Data = new byte[size];
//	byte	*CrcData = new byte[SizeCrc];
//	byte	*AllData = new byte[SizeAll];
//
//	uint16_t  crc_val;
//	Sprintln("============Response=====================");
//	//printResponse(Response);
//	memcpy(Data, &message, size);
//
//	byte *wCrcData = &CrcData[2];
//	byte *wData = Data;
//	byte *wAllData = &AllData[2];
//	Sprintln("============Przepisywanie do crc =====================");
//	for (int i = 0; i < size; i++)
//	{
//
//		*wCrcData = *wData;
//		*wAllData = *wData;
//		Sprint((int)wCrcData);
//		Sprint(" :: ");
//		Sprintln(*wCrcData);
//		wCrcData++;
//		wData++;
//		wAllData++;
//	}
//	CrcData[0] = (char)size;
//	CrcData[1] = 2; // respose
//	AllData[0] = (char)size;
//	AllData[1] = 2; // respose
//	Sprintln("============Wyliczanie crc=====================");
//	//obliczanie crc16
//	crc_val = CRC16.ccitt(CrcData, SizeCrc);
//	Sprint("Wartosc crc: ");
//	Sprintln(crc_val);
//
//	byte lo = crc_val & 0xFF;
//	byte hi = crc_val >> 8;
//	AllData[SizeAll - 2] = lo; // przypisanie mniej znaczacego bitu
//	AllData[SizeAll - 1] = hi; // przypisanie bardziej znaczaego bitu
//	byte *wCrcData2 = CrcData;
//	wAllData = &AllData[0];
//	//memcpy(AllData, &CrcData, SizeCrc);
//	Sprintln("============Przekopiowanie danych z crc do alldata=====================");
//	for (int i = 0; i < SizeCrc; i++)
//	{
//		Sprint((int)wCrcData2);
//		Sprint(" :crc: ");
//		Sprintln(*wCrcData2);
//
//		//*wAllData = *wCrcData2;
//
//		Sprint((int)wAllData);
//		Sprint(" :AllData: ");
//		Sprintln(*wAllData);
//
//		wCrcData2++;
//
//	}
//	Sprintln(" ");
//
//	Sprintln("============All data=====================");
//	for (int i = 0; i < SizeAll; i++)
//	{
//		Sprint(i);
//		Sprint(" :: ");
//		Sprintln(AllData[i]);
//	}
//	Sprintln("Wysyłam");
//	//Wire.write(AllData, SizeAll);
//	Serial.write(AllData, SizeAll);
//
//	//memset(Response2, 0, size); //czyszczenie danych
//	//memset(&Response, 0, size); //czyszczenie danych
//
//	delete[] AllData;
//	delete[] Data;
//	delete[] CrcData;
//	ReciveMessage = false;
//
//
//}
//
//
//#pragma endregion
//
//#pragma region ObslugaZapytania
//
//int VerifiReciveMessage(byte  *bMessage, int size) {
//	const int SizeCrc = size - 2;
//	byte *CrcData = new byte[SizeCrc];;
//
//
//	byte *wCrcData = CrcData;
//
//	int pozLo = size - 2;
//	int pozHi = size - 1;
//	byte *wMessageDataLo = &bMessage[pozLo];
//	byte *wMessageDataHi = &bMessage[pozHi];
//	byte *wMessageType = &bMessage[1];
//	byte *wSizeData = bMessage;
//	//Sprint("PozLo: "); Sprint(pozLo); Sprint(" :: val: "); Sprintln(*wMessageDataLo);
//	//Sprint("PozHi: "); Sprint(pozHi); Sprint(" :: val: "); Sprintln(*wMessageDataHi);
//	//Sprint("Pozmessegatype: "); Sprint(1); Sprint(" :: val: "); Sprintln(*wMessageType);
//	//Sprint("PozSizeData: "); Sprint(0); Sprint(" :: val: "); Sprintln(*wSizeData);
//
//
//
//	FastCRC16 CRC16;
//	uint16_t  crc_val;
//
//	Sprint("Crc size");
//	Sprintln(SizeCrc);
//	for (int i = 0; i < SizeCrc; i++)
//	{
//		*wCrcData = bMessage[i];
//		Sprint(*CrcData);
//		wCrcData++;
//	}
//	Sprintln("");
//
//	//obliczanie crc16
//	crc_val = CRC16.ccitt(CrcData, SizeCrc);
//	byte lo = crc_val & 0xFF;
//	byte hi = crc_val >> 8;
//	//Sprint(lo); Sprint("=="); Sprint(*wMessageDataLo);
//	//Sprint(hi); Sprint("=="); Sprint(*wMessageDataHi);
//	if (!((*wMessageDataHi == hi) && (*wMessageDataLo == lo)))
//	{
//		Sprintln("CRC Error");
//		return CRC_ERROR;
//
//	}
//
//	if (!(*wMessageType == (byte)1)) // jesli  typ rozny od message
//	{
//		Sprintln("NIe jest typu message");
//		return ERROR;
//
//	}
//
//	delete[] CrcData;
//
//	return OK;
//}
//
//
//
//
//
//
//
//
//
//t_i2cResponse readTempDs18b20(int pin, byte address[8]) {
//	t_i2cResponse result;
//	float temperature;
//	Sprintln("Wyszukiwanie wyniku: ");
//
//
//	// arrays to hold device addresses
//	DeviceAddress insideThermometer;
//	for (int i = 0; i < 8; i++)
//	{
//		insideThermometer[i] = (uint8_t)address[i];
//	}
//	Sprint("Szukany adres");
//	printAddress(insideThermometer);
//	Sprintln(".");
//
//	for (int i = 0; i < SensorsCount; i++)
//	{
//		DeviceAddress add;
//
//		//odczytanie adresu z tablicy
//		if (sensors.getAddress(add, i)) {
//			Sprint("Pozyycja: ");
//			Sprint(i);
//			Sprint("\t adres: ");
//			printAddress(add);
//			Sprintln(".");
//			// porównanie adresu 
//			if (CompareDeviceAddress(insideThermometer, add))
//			{
//				result.Temperature = temps[i];
//				result.Humidity = 0;
//				result.Status = OK;
//				result.Value = false;
//				Sprint("Odczytana temperatura:");
//				Sprintln(result.Temperature);
//				return result;
//			}
//		}
//
//	}
//
//
//
//	result.Temperature = 0;
//	result.Humidity = 0;
//	result.Status = ERROR;
//	result.Value = false;
//
//
//	return result;
//}
//
#pragma endregion

#pragma region CreateJson
String CreateDhtJson() {
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	root.set("SensorType", (int)Dht22);
	root.set("Pin", DhtPin);
	t_i2cResponse dtha = readTempDht(DhtPin);
	root.set("Temperature", dtha.Temperature, 2);
	root.set("Humidity", dtha.Humidity, 2);
	root.set("Date", getDateTime());
	String msg2;
	root.printTo(msg2);


	return msg2;
}
String CreateBmp180Json() {
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	//JsonObject& dht = root.createNestedObject("Dht");
	root.set("SensorType", (int)BMP180);

	t_i2cResponse dtha = readTempDht(DhtPin);
	root.set("Pressure", bmp.readPressure());
	root.set("Temperature", bmp.readTemperature());

	root.set("Date", getDateTime());
	String msg2;
	root.printTo(msg2);


	return msg2;
}
String CreateDs18b20Json(int i) {
	DeviceAddress add;
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	

	if (sensors.getAddress(add, i)) {
		root.set("SensorType", (int)Ds18B20);
		root.set("Address", getStringAddress(add));
		root.set("Temperature", temps[i], 2);
		root.set("Date", getDateTime());
	}
	String msg2;
	root.printTo(msg2);


	return msg2;
}
String PrepareMessage(t_operation operation, SensorEnum sensor, String object) {
	DynamicJsonBuffer jsonBuffer1;
	JsonObject& root = jsonBuffer1.createObject();

	root.set("MessageType", (int)operation); // rodzaj wysyłanej wiadomosci
	root.set("SensorType", (int)sensor);
	root.set("Sender", IdentityDevice); // identyfikator kto wysyla
	root.set("Reciver", 0);  // identyfikator do kogowysyłam 0 -RPI
	root.set("DataBus", 1);
	root.set("Message", object);  // serializowany obiekt docelowy
								  //root.prettyPrintTo(Serial);
								  //char *msg = new char[jsonBuffer1.size()];
	String msg2;
	root.printTo(msg2);
	//	delete &jsonBuffer1;
	return msg2;
}

#pragma endregion

#pragma region GetData
String getStringAddress(DeviceAddress deviceAddress)
{
	String ret;
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) 
			ret+= "0";
		ret += (String(deviceAddress[i], HEX));
	}
	return ret;
}
String getDateTime() {
	DateTime now = rtc.now();
	String ret = "";
	ret += String((int)now.year());
	ret += "-";
	if (now.month() < 10)
		ret += "0";
	ret += String(now.month());
	ret += "-";
	if (now.day() < 10)
		ret += "0";
	ret += String(now.day());
	ret += "T";
	if (now.hour() < 10)
		ret += "0";
	ret += String(now.hour());
	ret += ":";
	if (now.minute() < 10)
		ret += "0";
	ret += String(now.minute());
	ret += ":";
	if (now.second() < 10)
		ret += "0";
	ret += String(now.second());

	return ret;




}
t_i2cResponse readTempDht(int pin) {
	t_i2cResponse result;

	int type = 0;

	Sprint("Dht type: ");
	Sprint(type);
	Sprint("    pin: ");
	Sprint(pin);

	//DHT dht(pin, type);
	DHT dht;
	dht.setup(pin);

	delay(dht.getMinimumSamplingPeriod());
	float h = dht.getHumidity();
	float t = dht.getTemperature();
	if (isnan(t) || isnan(h)) {
		Sprintln("Failed to read from DHT");
		Sprintln(dht.getStatusString());
		result.Status = ERROR;
	}
	else {

		result.Temperature = t;
		result.Humidity = h;
		result.Status = OK;
	}
	return result;
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
void printMessage(t_i2cMessageFrame message) {
	Sprintln("Message:");
	Sprint("Operacja:");
	Sprintln(message.Operation);
	Sprint("Typ Czujnika:");
	Sprintln(message.TempSensor);
	Sprint("Operacja:");
	Sprintln(message.Pin);
	Sprint("Wartość pinu:");
	Sprintln(message.Value);
}

void printResponse(t_i2cResponse response) {
	Sprintln("Response:");
	Sprint("Wilgotnosc: ");
	Sprint(response.Humidity);
	Sprint("\t Temp: ");
	Sprint(response.Temperature);
	Sprint("\t Staus: ");
	Sprint(response.Status);
	Sprint("\t Value: ");
	Sprintln(response.Value);

}
#pragma endregion


