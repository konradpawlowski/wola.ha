#pragma once

typedef struct I2CResponse
{
	float Temperature;
	float Humidity;
	int Value;
	int Status;
} t_i2cResponse;

typedef struct I2CMessageFrame
{
	int Operation;
	int TempSensor;
	int Pin;
	byte SensorAddress[8];
	bool Value;
} t_i2cMessageFrame;