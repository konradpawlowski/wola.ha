using Restup.Webserver.Attributes;
using Restup.Webserver.Models.Contracts;
using Restup.Webserver.Models.Schemas;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wola.ha.common;
using wola.ha.common.DataModel;
using wola.ha.common.Model;

namespace RestUpServerController.Controller
{
    [RestController(InstanceCreationType.PerCall)]
    class SensorTemperatureValueControler
    {
       [UriFormat("/SensorTempValue")]
        public async Task<IPostResponse> AddSensorTemperatureValue([FromContent] SensorTemperatureValues data)
        {

            try
            {
                var ret = await Context.Instance.Connection.InsertAsync(data);
                return new PostResponse(PostResponse.ResponseStatus.Created, $"SensorTemperatureValues/{ret}");
            }
            catch (Exception ex)
            {
                Log.e(ex);
                return new PostResponse(PostResponse.ResponseStatus.Conflict, $"SensorTemperatureValues/{data}");
            }

        }
        [UriFormat("/GetLastSensorValue/{id}")]
        public async Task<IGetResponse> EncodingDecodingTestAsync(int id)
        {

            try
            {
                List<SensorTemperatureValues> values = await Context.Instance.Connection.Table<SensorTemperatureValues>().Where(w => w.SensorId == id).OrderByDescending(o => o.Date).ToListAsync();

                return new GetResponse(GetResponse.ResponseStatus.OK, values);

            }
            catch (Exception ex)
            {
                Log.e(ex);

            }

            return new GetResponse(GetResponse.ResponseStatus.OK);
        }
    }
}
