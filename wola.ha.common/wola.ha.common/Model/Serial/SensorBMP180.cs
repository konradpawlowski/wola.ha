using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace wola.ha.common.Model.Serial
{
   public class SensorBmp180 : ISenssor
    {
        public DateTime Date { get; set; }
        public float SensorType { get; set; }
        public float Temperature { get; set; }
        public float Pressure { get; set; }
        public override string ToString()
        {
            StringBuilder str = new StringBuilder();
            str.Append("Sensor type BMP180: ");
            str.Append(SensorType);
            str.AppendLine();
            
            str.AppendLine();
            str.Append("Temperature: \t");
            str.Append(Temperature);
            str.AppendLine();
            str.Append("Pressure: \t");
            str.Append(Pressure);
            str.AppendLine();
            str.Append("Date: \t");
            str.Append(Date.ToLocalTime());
            return str.ToString();
        }
    }
}
