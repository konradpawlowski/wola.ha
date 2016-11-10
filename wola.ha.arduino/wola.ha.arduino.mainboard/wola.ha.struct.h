#pragma once
typedef struct MyStruct
{
	float Temperature;
	float Humidity;

} TMyStruct;

typedef struct I2CMessageFrame
{
	int Operation;
	int TempSensor;
	int Pin;
	bool value;
	byte SensorAddress[8];





} TI2CMessageFrame;
