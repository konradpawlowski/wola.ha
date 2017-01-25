
using System;
using System.Threading.Tasks;
using Windows.Devices.I2c;

namespace wola.ha.common.Devices.PCF8591
{
    public class PCF8591 : IDisposable

    {
        /// <summary>
        /// The address of the device is 0x90, coded on 7 bits.
        /// To get the matching value om 8 bits, shift (>>) the bits by 1.
        /// </summary>
        private const byte addr_PCF8591 = (0x90 >> 1);
        private I2cDevice device;

        /// <summary>
        /// private constructor for internal use only.
        /// To instanciate a PCF8591 object, please use the static method PCF8591.Create();
        /// </summary>
        private PCF8591() { }

        /// <summary>
        /// Instanciate asyncronously a new PCF8591 object.
        /// </summary>
        /// <returns>A new PCF8591 object instance.</returns>
        public static Windows.Foundation.IAsyncOperation<PCF8591> Create()
        {
            return CreateAsync(I2cBusSpeed.StandardMode, I2cSharingMode.Exclusive).AsAsyncOperation();
        }

        /// <summary>
        /// Instanciate asyncronously a new PCF8591 object.
        /// </summary>
        /// <param name="BusSpeed">The I2C Bus Speed. Default value: StandardMode </param>
        /// <returns>A new PCF8591 object instance.</returns>
        public static Windows.Foundation.IAsyncOperation<PCF8591> Create(I2cBusSpeed BusSpeed)
        {
            return CreateAsync(BusSpeed, I2cSharingMode.Exclusive).AsAsyncOperation();
        }

        /// <summary>
        /// Instanciate asyncronously a new PCF8591 object.
        /// </summary>
        /// <param name="BusSpeed">The I2C Bus Speed. Default value: StandardMode </param>
        /// <param name="SharingMode">The I2C Sharing Mode. Default value is Exclusive. To use with caution </param>
        /// <returns>A new PCF8591 object instance.</returns>
        public static Windows.Foundation.IAsyncOperation<PCF8591> Create(I2cBusSpeed BusSpeed, I2cSharingMode SharingMode)
        {
            return CreateAsync(BusSpeed, SharingMode).AsAsyncOperation();
        }

        /// <summary>
        /// Instanciate asyncronously a new PCF8591 object.
        /// As System.Threading.Tasks.Task are not valid Windows Runtime type supported, this method has been set to private and is publicly exposed through the IAsyncOperation method "Create".
        /// </summary>
        /// <param name="BusSpeed">The I2C Bus Speed. Default value: StandardMode </param>
        /// <param name="SharingMode">The I2C Sharing Mode. Default value is Exclusive. To use with caution </param>
        /// <returns></returns>
        async static private Task<PCF8591> CreateAsync(I2cBusSpeed BusSpeed, I2cSharingMode SharingMode)
        {
            PCF8591 newADC = new PCF8591();
            /// advanced query syntax used to find devices on the RaspberryPi.
            string AQS = Windows.Devices.I2c.I2cDevice.GetDeviceSelector();
            var DevicesInfo = await Windows.Devices.Enumeration.DeviceInformation.FindAllAsync(AQS);
            if (DevicesInfo.Count == 0) throw new Exception("No Device Information were found with query: " + AQS);
            // I2C bus settings
            var settings = new Windows.Devices.I2c.I2cConnectionSettings(addr_PCF8591);
            settings.BusSpeed = BusSpeed;
            settings.SharingMode = SharingMode;
            // Reteives the device from the I2C bus with the given ID.
            newADC.device = await Windows.Devices.I2c.I2cDevice.FromIdAsync(DevicesInfo[0].Id, settings);
            if (newADC.device == null) throw new Exception("No I2C Device were found with ID " + DevicesInfo[0].Id);
            return newADC;
        }

        public void Dispose()
        {
            this.device.Dispose();
        }

        /// <summary>
        /// Returns an int value from 0 to 255 (included).
        /// </summary>
        /// <param name="InputPin">The Input pin on the PCF8591 to read analog value from</param>
        /// <returns></returns>
        public int ReadI2CAnalog(PCF8591_AnalogPin InputPin)
        {
            byte[] b = new byte[2];
            device.WriteRead(new byte[] { (byte)InputPin }, b);
            return b[1];
        }

        /// <summary>
        /// Returns an double value from 0 to 1.
        /// </summary>
        /// <param name="InputPin">The Input pin on the PCF8591 to read analog value from</param>
        /// <returns></returns>
        public double ReadI2CAnalog_AsDouble(PCF8591_AnalogPin InputPin)
        {
            return ReadI2CAnalog(InputPin) / 255d;
        }
    }

    /// <summary>
    /// The 4 available analog input pins on the PCF8591.
    /// Value defined by their internal address.
    /// </summary>
    public enum PCF8591_AnalogPin
    {
        A0 = 0x40,
        A1 = 0x41,
        A2 = 0x42,
        A3 = 0x43
    }
}
