#pragma once
typedef enum I2COperation
{
	Temp,
	Read,
	Write,
	Reset
} TOperation;


enum TempSensorEnum
{
	Ds18B20 = 1,
	Dht11 = 2,
	Dht22 = 3,
	BMP180 = 4,
	ACS712 = 5,
	OnOff = 6

};
enum I2CResponseStatus {
	BRAK,
	OK,
	ERROR,
	WARRNING,
	CRC_ERROR
};