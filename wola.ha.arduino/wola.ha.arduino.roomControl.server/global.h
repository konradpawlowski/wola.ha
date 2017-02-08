float temp = 0;
static int MEMORY_SIZE = 512;

ESP8266WebServer server(80);
String html;
String st; 
String content;
int statusCode;



typedef struct RoomSensor {
	int Id;
	char Name[20];
	char Address[16];
	float Temp;
	int Time;
	int IsOutside;

}TRoomSensor;
typedef struct TempDefinition {
	int Pin; //TempSensorEnum
	int Enable;
	int Typ; //TempSensorTypeEnum
	char Name[20];
	int Id;
	int IsOutside;//zewnêtrzny
	int IsInBox; // jesli jest w obudowie to odejmujemy od wyniku 2,9 stopnia C
	
} TTempDefinition;
typedef struct SensorsOnOffValue {
	int Id;
	float Temp;
	int IsOutside;
} TSensorsOnOffValue;
typedef struct WifiSettings {
	int IsWifiAp;
	char Ssid[20];
	char Pass[20];

} TWifiSettings;
typedef struct SensorValue {
	int Id;
	String Name;
	float Temp;
	float Humi;
	int IsOutside;


}TSensorValue;

typedef struct Parameters{
  char ServerAddress[16];
  
  TTempDefinition Temp1;
  TTempDefinition Temp2;

  int CountOfSensors;
  TSensorsOnOffValue sens[10];
  int SelectedRelay;
  TWifiSettings Wifi;
}TParameters;






 enum TempSensorEnum
{
	Temp1 = 14,
	Temp2 = 16

} ;

 enum PrzekaznikEnum
 {
	 Relay1 = 4,
	 Relay2 = 5
 };
enum TempSensorTypeEnum
{
	Dht,
	Ds18b20

};

void printTParameters(TParameters param) {


	Serial.print("Address:");
	Serial.println(param.ServerAddress);

	Serial.println(F("Temp1:"));
	Serial.print(F("Enble:"));
	Serial.println(param.Temp1.Enable);
	Serial.print(F("Id:"));
	Serial.println(param.Temp1.Id);
	Serial.print(F("Name:"));
	Serial.println(param.Temp1.Name);

	Serial.print(F("Pin:"));
	Serial.println(param.Temp1.Pin);
	Serial.print(F("Typ:"));
	Serial.println(param.Temp1.Typ);

	Serial.print(F("IsOutside:"));
	Serial.println(param.Temp1.IsOutside);

	Serial.println(F("Temp2:"));
	Serial.print(F("Enble:"));
	Serial.println(param.Temp2.Enable);
	Serial.print(F("Id:"));
	Serial.println(param.Temp2.Id);
	Serial.print(F("Name:"));
	Serial.println(param.Temp2.Name);
	Serial.print(F("Pin:"));
	Serial.println(param.Temp2.Pin);
	Serial.print(F("Typ:"));
	Serial.println(param.Temp2.Typ);
	Serial.print(F("IsOutside:"));
	Serial.println(param.Temp2.IsOutside);

	Serial.print(F("Selected relay:"));
	Serial.println(param.SelectedRelay);

	Serial.print(F("Iloœæ czujników :"));
	Serial.println(param.CountOfSensors);

	Serial.println(F("Ustawienia acesspoint:"));
	Serial.print("Access point is " + param.Wifi.IsWifiAp ? "Enable" : "Disable");
	Serial.print(F("SSID: "));
	Serial.print(param.Wifi.Ssid);
	Serial.print(F(" \t \t Pass:"));
	Serial.println(param.Wifi.Pass);



}



TParameters loadStruct(int address = 0)
{
	TParameters para;
	int size = sizeof(TParameters);
	byte data[sizeof(TParameters)];
//	EEPROM.begin(size);
	for (int i = 0; i < size; i++)
	{

		byte data1 = EEPROM.read(i+address);
		//Serial.println(data1);
		((byte *)data)[i] = data1;
	}
	//Serial.println((char*)data);
	memcpy(&para, data, size);

	return para;

}
TParameters ReadSettings() {

	TParameters param;

	param = loadStruct();
	Serial.println("Odczyt");
	printTParameters(param);

	return param;
}
void storeStruct(void *data_source, int size, int address = 0)
{

	//EEPROM.begin(512);
	//	Serial.print("Zapis");
	for (int i = 0; i < size; i++)
	{
		Serial.print("Zapisanie adres ");
		Serial.print(address + i);
		Serial.print(" - dane: ");


		byte data = ((byte *)data_source)[i + address];
		Serial.println(data);
		//	Serial.print(data);
		EEPROM.write(i, data);

	}
	EEPROM.commit();
	//	Serial.println("Koniec - Zapis");
	//	EEPROM.end();
}
void SaveSettings(TParameters param) {
	storeStruct(&param, sizeof(param));
	//EEPROM_writeAnything(0, param);
	Serial.println("Zapis");
	printTParameters(param);
}

void printValuesOnOff(TSensorsOnOffValue val) {
	Serial.println(F("Values ON/OFF:"));
	Serial.print(F("Id:"));
	Serial.println(val.Id);
	Serial.print(F("Temp:"));
	Serial.println(val.Temp);
	Serial.print(F("IsOutside:"));
	Serial.println(val.IsOutside);
}


void Blink(int ile = 3) {
	for (int i = 0; i < ile; i++) {
		digitalWrite(13, HIGH);
		delay(100);
		digitalWrite(13, LOW);
	}
}
void ClearEeprom() {
	TParameters param;
	int size = sizeof(TParameters);
	byte data[sizeof(TParameters)];
	EEPROM.begin(size);
	for (int i = 0; i < size; i++)
	{
		byte data1 = 0;
		((byte *)data)[i] = data1;
	}

	memcpy(&param, data, size);

	SaveSettings(param);
}