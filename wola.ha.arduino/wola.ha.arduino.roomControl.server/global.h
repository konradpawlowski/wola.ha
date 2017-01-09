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


}TRoomSensor;


typedef struct TempDefinition {
	int Pin; //TempSensorEnum
	int Enable;
	int Typ; //TempSensorTypeEnum
	char Name[20];
	int Id;
} TTempDefinition;

typedef struct Parameters{
  char ServerAddress[16];
  
  TTempDefinition Temp1;
  TTempDefinition Temp2;
  
}TParameters;

typedef struct SensorValue {
	float Temp;
	float Humi;

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