// wola_ha_common.h
//#include <DallasTemperature.h>
#ifndef _WOLA_HA_COMMON_h
#define _WOLA_HA_COMMON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//String getStringAddress(DeviceAddress deviceAddress)
//{
//	String ret;
//	for (uint8_t i = 0; i < 8; i++)
//	{
//		// zero pad the address if necessary
//		if (deviceAddress[i] < 16)
//			ret += "0";
//		ret += (String(deviceAddress[i], HEX));
//	}
//	return ret;
//}

#endif

