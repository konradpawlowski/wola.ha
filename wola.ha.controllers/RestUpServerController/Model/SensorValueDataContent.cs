using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wola.ha.common.Enums;

namespace RestUpServerController.Model
{
    public sealed class SensorValueDataContent
    {
        public int SensorType { get; set; }
        public int SensorKind { get; set; }
        public string SensorAddress { get; set; }
        public float Value { get; set; }
    }
}
