float temp = 0;
static int MEMORY_SIZE = 512;

ESP8266WebServer server(80);
String html;
String st; 
String content;
int statusCode;


typedef struct Parameters{
  char Name[10];
  int Id;
  char ServerAddress[16];
}TParameters;




