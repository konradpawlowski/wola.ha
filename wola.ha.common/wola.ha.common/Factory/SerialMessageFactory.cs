using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wola.ha.common.Enums;
using wola.ha.common.Helper;
using wola.ha.common.Model.Serial;

namespace wola.ha.common.Factory
{
    public class SerialMessageFactory
    {
        public async Task ManageMessage(SerialMessage message)
        {
            try
            {
                switch (message.MessageType)
                {
                    case SerialOperationEnum.SensorValues:
                       await ManageSensorValues(message);
                        break;
                    default:
                        break;
                }
            }
            catch (Exception ex)
            {

                LoggerFactory.LogExceptionStack(ex, "SerialMessageFactory.ManageMessage", new { message });
            }
        }


        async Task ManageSensorValues(SerialMessage message)
        {
            ISenssor val = new SensorDs8b20();
            switch (message.SensorType)
            {
                case SensorTypeEnum.test:
                    break;
                case SensorTypeEnum.Ds18B20:
                    val = JsonConvert.DeserializeObject<SensorDs8b20>(message.Message);
                    await WolaClient.PostItemToController<SensorDs8b20>("SensorDs18b20", (SensorDs8b20)val);
                   //     WolaClient.PostItemToController<SensorTemperatureValues>("SensorTempValue", item);
                    break;
                case SensorTypeEnum.Dht11:
                    val = JsonConvert.DeserializeObject<SensorDht>(message.Message);
                    await WolaClient.PostItemToController<SensorDht>("SensorDht22", (SensorDht)val);
                    break;
                case SensorTypeEnum.Dht22:
                    val = JsonConvert.DeserializeObject<SensorDht>(message.Message);
                    await WolaClient.PostItemToController<SensorDht>("SensorDht22", (SensorDht)val);
                    break;
                case SensorTypeEnum.BMP180:
                    val = JsonConvert.DeserializeObject<SensorBmp180>(message.Message);
                    await WolaClient.PostItemToController<SensorBmp180>("SensorBMP180", (SensorBmp180)val);
                    break;
                case SensorTypeEnum.ACS712:
                    break;
                case SensorTypeEnum.OnOff:
                    break;
                default:
                    break;
            }
            Debug.WriteLine(val.ToString());
        }
    }
}
