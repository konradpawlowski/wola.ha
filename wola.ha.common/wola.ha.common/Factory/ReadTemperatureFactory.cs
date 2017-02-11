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

namespace wola.ha.common.Factory
{
    public  class ReadTemperatureFactory
    {
        public async Task<SensorTemperatureValues> ReadTemperature(Sensors sensor)
        {
            try
            {
                SensorTemperatureValues val = new SensorTemperatureValues();
                I2CMessageFrame message = new I2CMessageFrame()
                {
                    Operation = I2COperation.Temp,
                    TempSensor = sensor.SensorType == 1 ? SensorTypeEnum.Ds18B20 : SensorTypeEnum.test,
                    Pin = sensor.Pin != null ? short.Parse(sensor.Pin) : (short)0,
                    SensorAddress = StringToByteArray(string.IsNullOrEmpty(sensor.Address) ? "" : sensor.Address)
                };
                ArduinoI2CResponse response = await GetTemperatureFromArduino(Convert.ToInt32(sensor.DataBusEx.Address, 16), message);
                if (response.Status == (short)I2CResponseStatus.Ok)
                {
                    val.Value = (decimal)response.Temperature;
                    val.Date = DateTime.Now;
                    val.SensorId = sensor.Id;
                }
                else
                {
                    Debug.WriteLine("Błąd, status: " + response.Status.ToString());
                }
                return val;
            }
            catch (Exception)
            {

                throw;
            }
          





            
        }
        public static byte[] StringToByteArray(String hex)
        {
            int numberChars = hex.Length;
            byte[] bytes = new byte[numberChars / 2];
            for (int i = 0; i < numberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        private async Task<ArduinoI2CResponse> GetTemperatureFromArduino(int adress, I2CMessageFrame message)
        {
                      
            try
            {
                byte[] mahByteArray = message.ToArray();
                byte[] aaa = await I2CHelper.WriteRead(adress, mahByteArray);

                ArduinoI2CResponse ph = (ArduinoI2CResponse) GetObjectFromBytes(aaa, typeof(ArduinoI2CResponse));
                return ph;
            }
            catch (Exception e)
            {
                return new ArduinoI2CResponse {Status = (short)I2CResponseStatus.Error };
            }


        }
        public static object GetObjectFromBytes(byte[] buffer, Type objType)
        {
            object obj = null;
            if ((buffer != null) && (buffer.Length > 0))
            {
                IntPtr ptrObj = IntPtr.Zero;
                try
                {
                    int objSize = Marshal.SizeOf(objType);
                    if (objSize > 0)
                    {
                        if (buffer.Length < objSize)
                            throw new Exception(
                                String.Format("Buffer smaller than needed for creation of object of type {0}", objType));
                        ptrObj = Marshal.AllocHGlobal(objSize);
                        if (ptrObj != IntPtr.Zero)
                        {
                            Marshal.Copy(buffer, 0, ptrObj, objSize);
                            obj = Marshal.PtrToStructure(ptrObj, objType);
                        }
                        else
                            throw new Exception(String.Format("Couldn't allocate memory to create object of type {0}",
                                objType));
                    }
                }
                finally
                {
                    if (ptrObj != IntPtr.Zero)
                        Marshal.FreeHGlobal(ptrObj);
                }
            }
            return obj;
        }
    }
}
