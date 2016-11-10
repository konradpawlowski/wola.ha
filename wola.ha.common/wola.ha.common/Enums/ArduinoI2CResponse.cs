using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace wola.ha.common
{
    struct ArduinoI2CResponse
    {
        public float Temperature { get; set; }
        public float Humidity { get; set; }
        public bool Value { get; set; }

    };
}
