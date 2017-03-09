using Restup.Webserver.Attributes;
using Restup.Webserver.Models.Contracts;
using Restup.Webserver.Models.Schemas;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wola.ha.common.DataModel;
using wola.ha.common.Helper;
using wola.ha.common.Model;

namespace RestUpServerController
{
    [RestController(InstanceCreationType.PerCall)]
    class SensorOnOffValueController
    {
        [UriFormat("/SensorOnOffValue")]
        public async Task<IPostResponse> AddSensorHumidityValue([FromContent] SensorOnOffValue data)
        {

            try
            {
                var ret = await Context.Instance.Connection.InsertAsync(data);
                return new PostResponse(PostResponse.ResponseStatus.Created, $"SensorOnOffValue/{ret}");
            }
            catch (Exception ex)
            {
                LoggerFactory.LogExceptionStack(ex);
                return new PostResponse(PostResponse.ResponseStatus.Conflict, $"SensorOnOffValue/{data}");
            }

        }
    }
}
