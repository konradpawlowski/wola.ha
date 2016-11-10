using System;
using System.Diagnostics;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.I2c;

namespace wola.ha.common.I2C
{
    public class I2CHelper
    {
        private static string _aqs;
        private static DeviceInformationCollection _dis;

        public static async Task<byte[]> WriteRead(int slaveAddress,byte[] byteToBeSend)
        {
            byte[] receivedData = new byte[40];


            /* Arduino Nano's I2C SLAVE address */
      
            try
            {
                // Initialize I2C
                var settings = new I2cConnectionSettings(slaveAddress) {BusSpeed = I2cBusSpeed.StandardMode};

                if (_aqs == null || _dis == null)
                {
                 
                    _aqs = I2cDevice.GetDeviceSelector("I2C1");
                    _dis = await DeviceInformation.FindAllAsync(_aqs);
                }


                using (I2cDevice device = await I2cDevice.FromIdAsync(_dis[0].Id, settings))
                {
                   var send= device.WritePartial((byteToBeSend));
                    await Task.Delay(1000);
                    var a = device.ReadPartial(receivedData);
                 Debug.WriteLine(a.Status);
            
                }
            }
            catch (Exception)
            {
                // SUPPRESS ANY ERROR
            }

            /* Return received data or ZERO on error */
            return receivedData;
        }
    }
}