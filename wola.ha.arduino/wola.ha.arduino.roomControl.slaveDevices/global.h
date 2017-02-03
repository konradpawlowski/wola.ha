float temp = 0;
static int MEMORY_SIZE = 512;

ESP8266WebServer server(80);
String html;
String st; 
String content;
int statusCode;


typedef struct TempDefinition {
	int Pin; //TempSensorEnum
	int Enable;
	int Typ; //TempSensorTypeEnum
	char Name[20];
	int Id;
	int IsOutside;
} TTempDefinition;

typedef struct WifiSettings {
	int IsWifiAp;
	char Ssid[20];
	char Pass[20];

} TWifiSettings;
typedef struct Parameters{
  char ServerAddress[16];
  TTempDefinition Temp1;
  TTempDefinition Temp2;
  TWifiSettings Wifi;
}TParameters;


typedef struct SensorValue {
	int Id;
	String Name;
	float Temp;
	float Humi;
	int IsOutside;

}TSensorValue;

 enum TempSensorEnum
{
	Temp1 = 14,
	Temp2 = 16

} ;
enum TempSensorTypeEnum
{
	Dht,
	Ds18b20

};

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

	Serial.println(F("Ustawienia acesspoint:"));
	Serial.print("Access point is " + param.Wifi.IsWifiAp ? "Enable" : "Disable");
	Serial.print(F("SSID: "));
	Serial.print(param.Wifi.Ssid);
	Serial.print(F(" \t \t Pass:"));
	Serial.println(param.Wifi.Pass);
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
	int size = sizeof(TParameters);
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