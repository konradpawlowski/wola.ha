using System;
using System.Text;


namespace wola.ha.common.Model.Serial
{
    public class SensorDht : ISenssor
    {
       
        public int Pin { get; set; }
        public float Temperature { get; set; }
        public float Humidity { get; set; }
        public DateTime Date { get; set; }
        public override string ToString()
        {
            StringBuilder str = new StringBuilder();
            str.Append("Sensor type DHT");
            str.AppendLine();
            str.Append("Pin: \t");
            str.Append(Pin);
            str.AppendLine();
            str.Append("Temperature: \t");
            str.Append(Temperature);
            str.AppendLine();
            str.Append("Humidity: \t");
            str.Append(Humidity);
            str.AppendLine();
            str.Append("Date: \t");
            str.Append(Date.ToLocalTime());
            return str.ToString();
        }
    }
}
