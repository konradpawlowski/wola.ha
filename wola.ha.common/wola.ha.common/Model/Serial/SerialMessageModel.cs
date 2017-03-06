using wola.ha.common.Enums;

namespace wola.ha.common.Model.Serial
{
    public class SerialMessage
    {
        public SerialOperationEnum MessageType { get; set; }
        public SensorTypeEnum SensorKind { get; set; }
        public int Sender { get; set; }
        public int Reciver { get; set; }
        public int DataBus { get; set; }
        public string Message { get; set; }
    }

    public enum SerialOperationEnum
    {
        SensorValues = 1
    }
}
