using System.IO;
using wola.ha.common.Enums;

namespace wola.ha.common.Devices.I2c
{
    internal struct I2CMessageFrame
    {
        public I2COperation Operation { get; set; }
        public TempSensorEnum TempSensor { get; set; }
        public short Pin { get; set; }
        public byte[] SensorAddress { get; set; }
        public bool Value { get; set; }


        public byte[] ToArray()
        {
            var stream = new MemoryStream();
            var writer = new BinaryWriter(stream);

            writer.Write((short) this.Operation);
            writer.Write((short) this.TempSensor);
            writer.Write(this.Pin);
            writer.Write(this.SensorAddress);
            writer.Write(this.Value);

            return stream.ToArray();
        }
    }
}