using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace wola.ha.common.Model.Serial
{
    public class SensorAcs712 : ISenssor
    {
        public DateTime Date { get; set; }
        public int SensorType { get; set; }
        public string Address { get; set; }
        public bool Value   { get; set; }
       
        public override string ToString()
        {
            StringBuilder str = new StringBuilder();
            str.Append("Sensor type Acs712: ");
            str.Append(SensorType);
            str.AppendLine();
            str.Append("Address: \t");
            str.Append(Address);
            str.AppendLine();
            str.Append("Value: \t");
            str.Append(Value);
            str.AppendLine();
            
            str.AppendLine();
            str.Append("Date: \t");
            str.Append(Date.ToLocalTime());
            return str.ToString();
        }
    }
}
