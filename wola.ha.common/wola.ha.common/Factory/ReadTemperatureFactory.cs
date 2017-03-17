using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;

using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using wola.ha.common.DataModel;
using wola.ha.common.Devices.I2c;
using wola.ha.common.Enums;
using wola.ha.common.Helper;

namespace wola.ha.common.Factory
{
    public  class ReadTemperatureFactory
    {

        public async Task<SensorTemperatureValues> ReadTemperature(Sensors sensor)
        {
            SensorTemperatureValues value = new SensorTemperatureValues();

            switch (sensor.DataBusEx.Type)
            {
                case 4://arduino
                    value =  await ReadTemperatureFromArduino(sensor);
                    break;
                default:
                    break;
            }

            return value;
        }


        public async Task<SensorTemperatureValues> ReadTemperatureFromArduino(Sensors sensor)
        {
            try
            {
             //   LoggerFactory.LogInfo("Wysyłanie zapytanie o temperatur");
                SensorTemperatureValues val = new SensorTemperatureValues();
                I2CMessageFrame message = new I2CMessageFrame()
                {
                    Operation = I2COperation.Temp,
                    TempSensor = (SensorTypeEnum)sensor.SensorType,
                    Pin = sensor.Pin != null ? short.Parse(sensor.Pin) : (short)0,
                    SensorAddress = string.IsNullOrEmpty(sensor.Address) ? new byte[8] : I2CHelper.StringToByteArray(sensor.Address)
                };

                LoggerFactory.LogInfo("Wysyłanie zapytanie o temperature","Sensor:",new { sensor});
                ArduinoI2CResponse response = await GetTemperatureFromArduino(Convert.ToInt32(sensor.DataBusEx.Address, 16), message);
                if (response.Status == (short)I2CResponseStatus.Ok)
                {
                    val.Value = (double)response.Temperature;
                    val.Date = DateTime.Now;
                    val.SensorId = sensor.Id;
                }
                else
                {
                    LoggerFactory.LogError("Błąd, status: " + response.Status.ToString());
                    Debug.WriteLine("Błąd, status: " + response.Status.ToString());
                }
                return val;
            }
            catch (Exception ex)
            {
                LoggerFactory.LogExceptionStack(ex);
                throw;
            }
        }


        private async Task<ArduinoI2CResponse> GetTemperatureFromArduino(int adress, I2CMessageFrame message)
        {
                      
            try
            {
                byte[] mahByteArray = I2CHelper.PrepareMessageToSend(message);
                byte[] responseData = await I2CHelper.WriteRead(adress, mahByteArray);

               // ArduinoI2CResponse ph = (ArduinoI2CResponse) GetObjectFromBytes(aaa, typeof(ArduinoI2CResponse));
                return I2CHelper.VerifyIncomingData(responseData);
            }
            catch (Exception e)
            {
                return new ArduinoI2CResponse {Status = (short)I2CResponseStatus.Error };
            }


        }
      
    }
}
