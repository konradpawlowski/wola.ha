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
