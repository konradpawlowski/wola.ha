using Restup.Webserver.Attributes;
using Restup.Webserver.Models.Contracts;
using Restup.Webserver.Models.Schemas;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using wola.ha.common.DataModel;
using wola.ha.common.Helper;
using wola.ha.common.Model;
using wola.ha.common.Model.Serial;

namespace RestUpServerController.Controller.Sensors
{
    [RestController(InstanceCreationType.PerCall)]
    class SensorValuesController
    {
        [UriFormat("/SensorDs18b20")]
        public async Task<IPostResponse> AddSensorDs18b20ValueAsync([FromContent] SensorDs8b20 data)
        {

            try
            {
                var sensor = await Context.Instance.Connection.Table<wola.ha.common.DataModel.Sensors>().Where(w => w.SensorType == data.SensorType && w.Address.ToUpper() == data.Address.ToUpper()).FirstOrDefaultAsync();
                if (sensor == null) return new PostResponse(PostResponse.ResponseStatus.Conflict, $"AddSensorDs18b20Value/{data}");

                var value = new SensorTemperatureValues
                {
                    SensorId = sensor.Id,
                    Value = (double)data.Temperature,
                    Date = DateTime.Now

                };


                var ret = Context.Instance.Connection.InsertAsync(value);
                return new PostResponse(PostResponse.ResponseStatus.Created, $"AddSensorDs18b20Value/{ret}");
            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);
                return new PostResponse(PostResponse.ResponseStatus.Conflict, $"AddSensorDs18b20Value/{data}");
            }

        }
        [UriFormat("/SensorDht22")]
        public async Task<IPostResponse> AddDht22Value([FromContent] SensorDht data)
        {
            int[] ret = new int[2];
            try
            {
                string pin = data.Pin.ToString();
                var sensors = await Context.Instance.Connection.Table<wola.ha.common.DataModel.Sensors>()
                    .Where(w => w.Pin == pin)
                    .ToListAsync();
                foreach (var item in sensors)
                {
                    switch (item.SensorKind)
                    {
                        case 1:
                            var temp = new SensorTemperatureValues
                            {
                                SensorId = item.Id,
                                Value = (double)data.Temperature,
                                Date = DateTime.Now

                            };
                            ret[0] = await Context.Instance.Connection.InsertOrReplaceAsync(temp);

                            break;

                        case 2:
                            var humi = new SensorHumidityValues
                            {
                                SensorId = item.Id,
                                Value = (decimal)data.Humidity,
                                Date = DateTime.Now

                            };
                            ret[1] = await Context.Instance.Connection.InsertOrReplaceAsync(humi);
                            break;
                        
                    }
                   
                }

                

              


               // var ret = await Context.Instance.Connection.InsertAsync(value);
                return new PostResponse(PostResponse.ResponseStatus.Created, $"AddDht22Value/{ret}");
            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);
                return new PostResponse(PostResponse.ResponseStatus.Conflict, $"SensorTemperatureValues/{data}");
            }

        }

        [UriFormat("/SensorBmp180")]
        public async Task<IPostResponse> AddSensorBmp180Value([FromContent] SensorBmp180 data)
        {

            try
            {
                var sensor = await Context.Instance.Connection.Table<wola.ha.common.DataModel.Sensors>().Where(w => w.SensorType == data.SensorType).FirstOrDefaultAsync();
                if (sensor == null) return new PostResponse(PostResponse.ResponseStatus.Conflict, $"AddSensorDs18b20Value/{data}"); ;

                var value = new SensorPressureValues
                {
                    SensorId = sensor.Id,
                    Value = (decimal)data.Pressure,
                    Date = DateTime.Now

                };


                var ret = await Context.Instance.Connection.InsertAsync(value);
                return new PostResponse(PostResponse.ResponseStatus.Created, $"AddSensorDs18b20Value/{ret}");
            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);
                return new PostResponse(PostResponse.ResponseStatus.Conflict, $"SensorTemperatureValues/{data}");
            }

        }

        [UriFormat("/SensorAcs712")]
        public async Task<IPostResponse> AddSensorAcs7120Value([FromContent] SensorAcs712 data)
        {

            try
            {
                var sensor = await Context.Instance.Connection.Table<wola.ha.common.DataModel.Sensors>().Where(w => w.SensorType == data.SensorType && w.Address == data.Address).FirstOrDefaultAsync();
                if (sensor == null) return new PostResponse(PostResponse.ResponseStatus.Conflict, $"AddSensorAcs7120Value/{data}"); ;

                var value = new SensorOnOffValue
                {
                    SensorId = sensor.Id,
                    Value = data.Value,
                    Date = DateTime.Now

                };


                var ret = await Context.Instance.Connection.InsertAsync(value);
                return new PostResponse(PostResponse.ResponseStatus.Created, $"AddSensorAcs7120Value/{ret}");
            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);
                return new PostResponse(PostResponse.ResponseStatus.Conflict, $"SensorTemperatureValues/{data}");
            }

        }
    }
}
