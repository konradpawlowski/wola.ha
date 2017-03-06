using System;

namespace wola.ha.common.I2C
{
    internal class I2CDeviceNotFoundException : Exception
    {
        public I2CDeviceNotFoundException() :
            base("Could not find the I2C controller!")
        { }

        public I2CDeviceNotFoundException(string message) : base(message)
        { }

        public I2CDeviceNotFoundException(string message, Exception innerException) : base(message, innerException)
        { }
    }
}