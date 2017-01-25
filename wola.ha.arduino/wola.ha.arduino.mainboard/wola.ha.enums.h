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
	Dht,
	Ds18B20

};
enum I2CResponseStatus {
	BRAK,
	OK,
	ERROR,
	WARRNING
};