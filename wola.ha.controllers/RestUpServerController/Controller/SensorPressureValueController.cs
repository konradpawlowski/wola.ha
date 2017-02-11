using Restup.Webserver.Attributes;
using Restup.Webserver.Models.Contracts;
using Restup.Webserver.Models.Schemas;
using RestUpServerController.Model;
using SQLite.Net;
using System;
using System.Diagnostics;
using wola.ha.common.DataModel;
using System.Collections.Generic;
using wola.ha.common.Model;
using System.Threading.Tasks;
using wola.ha.common;
using wola.ha.common.Devices.Storage;

namespace RestUpServerController.Controller
{
    [RestController(InstanceCreationType.PerCall)]
    class SensorPressureValueController 
    {




        [UriFormat("/SensorPressureValue")]
        public async Task<IPostResponse> AddSensorPressuereValue([FromContent] SensorPressureValues data)
        {

            try
            {
                var ret = await Context.Instance.Connection.InsertAsync(data);
                return new PostResponse(PostResponse.ResponseStatus.Created, $"SensorPressureValue/{ret}");
            }
            catch (Exception ex)
            {
                Log.e(ex);
                return new PostResponse(PostResponse.ResponseStatus.Conflict, $"SensorPressureValue/{data}");
            }

        }

    }
}

