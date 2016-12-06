using System;

namespace wola.ha.common.I2C
{
    internal class I2CAddressException : Exception
    {
        public I2CAddressException() :
            base("The address on I2C Controller is currently in use.")
        { }

        public I2CAddressException(string message) : base(message)
        { }

        public I2CAddressException(string message, Exception innerException) : base(message, innerException)
        { }

        public I2CAddressException(object slaveAddress, object i2CControllerDeviceId) :
            base(string.Format("The address {0} on I2C Controller {1} is currently in use.", slaveAddress, i2CControllerDeviceId))
        { }
    }
}