using System;
using System.Text;

namespace wola.ha.common.Model.Serial
{
    public class SensorDs8b20 : ISenssor
    {
       
        public string Address { get; set; }
        public float Temperature { get; set; }
        public DateTime Date { get; set; }
        public override string ToString()
        {
            StringBuilder str = new StringBuilder();
            str.Append("Sensor type DS18B20");
            str.AppendLine();
            str.Append("Address \t");
            str.Append(Address);
            str.AppendLine();
            str.Append("Temperature: \t");
            str.Append(Temperature);
            str.AppendLine();
            str.Append("Date: \t");
            str.Append(Date.ToLocalTime());
            return str.ToString();
        }
    }
}
