#pragma once
typedef enum Operation
{
	SensorValues = 1,
	Message = 2
} t_operation;


enum SensorEnum
{
	Ds18B20 = 1,
	Dht11 = 2,
	Dht22 = 3,
	BMP180 = 4,
	Acs712 = 5,
	OnOff = 6

};

enum MessageType {
	Info,
	Warrning,
	Error
};


enum ValueType {
	Temperature = 1,
	Humidity = 2,
	Presure = 3,
	Electricity = 4
};
enum I2CResponseStatus {
	BRAK,
	OK,
	ERROR,
	WARRNING,
	CRC_ERROR
};