using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace wola.ha.common
{
    public struct ArduinoI2CResponse
    {
        public float Temperature { get; set; }
        public float Humidity { get; set; }
        public short Value { get; set; }
        public short Status { get; set; }

    };
}
