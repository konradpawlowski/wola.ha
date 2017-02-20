#include <DHT.h>
#include <DallasTemperature\DallasTemperature.h>
#include <Wire.h>
#include <OneWire.h>
#include "wola.ha.enums.h"
#include "FastCRC.h"
#include "wola.ha.struct.h"



#define OneWirePin 8
#define DhtPin 4
#define TEMPERATURE_PRECISION 10
#define I2cAddress 0x40



//t_i2cMessageFrame tMessage;
t_i2cResponse Response;
OneWire oneWire(OneWirePin);
DallasTemperature sensors(&oneWire);
int SensorsCount;
float temps[10] = { 0,0,0,0,0,0,0,0,0,0 };
bool ReciveMessage = false;

unsigned long iStart = 0;
unsigned long iTempStop = 0;
static int  IntervalTemp = 10000;


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
	//iStart = millis();
	//if (iStart - iTempStop >= IntervalTemp)
	//{
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
		//iTempStop = millis();
	//}
	delay(5000);
	//crc16test();
	
}

void crc16test()
{

	byte aa = 15;
	const int bbb = sizeof(t_i2cMessageFrame);

	Serial.println(aa);
	Serial.println(bbb);
	if (aa == (byte)bbb)
		Serial.println("true");
	//t_i2cResponse result;

	//result.Temperature = 15.5;
	//result.Humidity = 0;
	//result.Status = OK;
	//result.Value = false;
	////FastCRC16 CRC16;

	//Response = result;

	//sendData();

	//char buf[9] = { '1', '2', '3','4', '5', '6', '7', '8', '9' };
	//Serial.println("CRC Example");
	//Serial.println();

	//Serial.print("CCITT-CRC of \"");

	//for (unsigned int i = 0; i < sizeof(buf); i++) {
	//	Serial.print((char)buf[i]);
	//	
	//}
	//uint16_t value = CRC16.ccitt(buf, sizeof(buf));
	//byte lo = value & 0xFF;
	//byte hi = value >> 8;
	//Serial.print("\" is: 0x");
	//Serial.println(value, HEX);
	//Serial.print("Lo is: 0x");
	//Serial.println(lo, HEX);
	//
	//Serial.print("Hi is: 0x");
	//Serial.println(hi, HEX);
	//

	//byte hi1 = 0x29;
	//byte lo1 = 0xB1;

	//uint16_t value1 = lo1 | uint16_t(hi1) << 8;
	//Serial.println(value1, HEX);


	//if (value == value1)
	//	Serial.println("True");
}


#pragma region i2cComunication
void receiveEvent(int howMany) {  //Odebranie I2C, howMany - ile bajtˇw
	//int j = 0;
	//const int SizeAll = sizeof(t_i2cMessageFrame) + 4;
	//byte	ReceivedData[SizeAll];
//	const int SizeCrc = sizeof(t_i2cMessageFrame) + 2;
//	const int Size = sizeof(t_i2cMessageFrame);
	
//	byte	CrcData[SizeCrc];
//	byte	MessageData[Size];

	//FastCRC16 CRC16;
	//uint16_t  crc_val;

	//ReciveMessage = true;

	//Serial.print("reciving...");
	//Serial.println(howMany);
	//while (Wire.available()) {
	//	ReceivedData[j] = Wire.read();
	//	Serial.print(ReceivedData[j]);
	//	j++;
	//}
	//Serial.println("");
	//ReciveMessage = true;
	//for (int i = 0; i < SizeCrc; i++)
	//	CrcData[i] = ReceivedData[i];

	////obliczanie crc16
	//crc_val = CRC16.ccitt(CrcData, sizeof(CrcData));
	//byte lo = crc_val & 0xFF;
	//byte hi = crc_val >> 8;

	//if (!((ReceivedData[SizeAll - 1] == hi) && (ReceivedData[SizeAll - 2] == lo)) )
	//{ 
	//	Serial.println("CRC Error");
	//	Response.Status = CRC_ERROR;
	//	
	//	sendData();
	//	
	//}

	//if (!(ReceivedData[1] == (byte)1)) // jesli  typ rozny od message
	//{
	//	Serial.println("NIe jest typu message");
	//	Response.Status = ERROR;
	//	
	//	sendData();
	//}
	//for (int i = 0; i < (int)ReceivedData[0]; i++)
	//	MessageData[i] = ReceivedData[i + 2];

	//t_i2cMessageFrame tMessage; //Re-make the struct
	//memcpy(&tMessage, MessageData, sizeof(tMessage));
	//recivedDataManager(tMessage);


	int j = 0;
	//const int SizeAll = sizeof(t_i2cMessageFrame) + 4;
	int size = sizeof(t_i2cMessageFrame);
	byte *RecivedData;
	byte *MessageData;
	
	
	RecivedData = new byte[howMany];
	MessageData = new byte[size];
	byte *wReciveData = RecivedData;


	ReciveMessage = true;

	Serial.print("reciving...");
	Serial.println(howMany);
	while (Wire.available()) {
		*wReciveData = Wire.read();
		Serial.print((int)wReciveData);
		Serial.print(":");
		Serial.println(*wReciveData);
		wReciveData++;
	}
	Serial.println("::: Koniec odczytu");

	int verification = VerifiReciveMessage(RecivedData,howMany);
	if (verification == OK)
	{
		Serial.println("Weryfikacja crc OK");

		
		byte *wMessageData = MessageData;
		byte *wReciveData = &RecivedData[2];
		
		Serial.println("Przepisanie danych do tablicy CRC:");
		for (int i = 0; i < (int)RecivedData[0]; i++)
		{
			*wMessageData = *wReciveData;
			Serial.print((int)wMessageData);
			Serial.print(" :: ");
			Serial.println(*wMessageData);
			wMessageData++;
			wReciveData++;
		}
		Serial.println("Koniec przepisywania");
		t_i2cMessageFrame tMessage; //Re-make the struct
		memcpy(&tMessage, MessageData, sizeof(tMessage));
		//printMessage(tMessage);
		delete [] RecivedData;
		delete[] MessageData;
		//printMessage(tMessage);
		recivedDataManager(tMessage);
	}
	else
	{
		Response.Humidity = 0;
		Response.Temperature = 0;
		Response.Value = 0;
		Response.Status = verification;
		sendData();
	}

}

void sendData() {
	const int size = sizeof(t_i2cResponse);
	const int SizeAll = size + 4;
	const int SizeCrc = size + 2;

	byte bSize = size;
	FastCRC16 CRC16;
	
	byte	*Response2 = new byte[size];
	byte	*CrcData = new byte[SizeCrc];
	byte	*AllData = new byte[SizeAll];

	uint16_t  crc_val;
	Serial.println("============Response=====================");
	printResponse(Response);
	memcpy(Response2, &Response, size);
	
	byte *wCrcData = &CrcData[2];
	byte *wResponse2 = Response2;
	byte *wAllData = &AllData[2];
	Serial.println("============Przepisywanie do crc =====================");
	for (int i = 0; i < size; i++)
	{	

		*wCrcData = *wResponse2;
		*wAllData = *wResponse2;
		Serial.print((int)wCrcData);
		Serial.print(" :: ");
		Serial.println(*wCrcData);
		wCrcData++ ;
		wResponse2++;
		wAllData++;
	}
	CrcData[0] = (char)size;
	CrcData[1] = 2; // respose
	AllData[0] = (char)size;
	AllData[1] = 2; // respose
	Serial.println("============Wyliczanie crc=====================");
	//obliczanie crc16
	crc_val = CRC16.ccitt(CrcData, SizeCrc);
	Serial.print("Wartosc crc: ");
	Serial.println(crc_val);

	byte lo = crc_val & 0xFF;
	byte hi = crc_val >> 8;
	AllData[SizeAll - 2] = lo; // przypisanie mniej znaczacego bitu
	AllData[SizeAll - 1] = hi; // przypisanie bardziej znaczaego bitu
	byte *wCrcData2 = CrcData;
	wAllData = &AllData[0];
	//memcpy(AllData, &CrcData, SizeCrc);
	Serial.println("============Przekopiowanie danych z crc do alldata=====================");
	for (int i = 0; i < SizeCrc; i++)
	{
		Serial.print((int)wCrcData2);
		Serial.print(" :crc: ");
		Serial.println(*wCrcData2);

		//*wAllData = *wCrcData2;

		Serial.print((int)wAllData);
		Serial.print(" :AllData: ");
		Serial.println(*wAllData);
		
		wCrcData2++;
		
	}
	Serial.println(" ");

	Serial.println("============All data=====================");
	for (int i = 0; i < SizeAll; i++)
	{
		Serial.print(i);
		Serial.print(" :: ");
		Serial.println(AllData[i]);
	}
	Serial.println("Wysyłam");
	Wire.write(AllData, SizeAll);

	//memset(Response2, 0, size); //czyszczenie danych
	memset(&Response, 0, size); //czyszczenie danych

	delete[] AllData;
	delete[] Response2;
	delete[] CrcData;
	ReciveMessage = false;

	
}


#pragma endregion

#pragma region ObslugaZapytania

int VerifiReciveMessage(byte  *bMessage, int size) {
	const int SizeCrc = size - 2;
	byte *CrcData =  new byte[SizeCrc];;
	

	byte *wCrcData = CrcData;
	
	int pozLo = size - 2;
	int pozHi = size - 1;
	byte *wMessageDataLo = &bMessage[pozLo];
	byte *wMessageDataHi = &bMessage[pozHi];
	byte *wMessageType   = &bMessage[1];
	byte *wSizeData = bMessage;
	Serial.print("PozLo: "); Serial.print(pozLo); Serial.print(" :: val: "); Serial.println(*wMessageDataLo);
	Serial.print("PozHi: "); Serial.print(pozHi); Serial.print(" :: val: "); Serial.println(*wMessageDataHi);
	Serial.print("Pozmessegatype: "); Serial.print(1); Serial.print(" :: val: "); Serial.println(*wMessageType);
	Serial.print("PozSizeData: "); Serial.print(0); Serial.print(" :: val: "); Serial.println(*wSizeData);

	

	FastCRC16 CRC16;
	uint16_t  crc_val;

	Serial.print("Crc size");
	Serial.println(SizeCrc);
	for (int i = 0; i < SizeCrc; i++)
	{
		*wCrcData = bMessage[i];
		Serial.print(*CrcData);
		wCrcData++;
	}
	Serial.println("");
	
	//obliczanie crc16
	crc_val = CRC16.ccitt(CrcData, SizeCrc);
	byte lo = crc_val & 0xFF;
	byte hi = crc_val >> 8;
	Serial.print(lo); Serial.print("=="); Serial.print(*wMessageDataLo);
	Serial.print(hi); Serial.print("=="); Serial.print(*wMessageDataHi);
	if (!((*wMessageDataHi == hi) && (*wMessageDataLo == lo)))
	{
		Serial.println("CRC Error");
		return CRC_ERROR;

	}

	if (!(*wMessageType == (byte)1)) // jesli  typ rozny od message
	{
		Serial.println("NIe jest typu message");
		return ERROR;

	}

	delete [] CrcData;
	
	return OK;
}






void recivedDataManager(t_i2cMessageFrame message) {
	printMessage(message);
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
	case Dht11:
		result = readTempDht(message.Pin);
		break;
	case Dht22:
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


