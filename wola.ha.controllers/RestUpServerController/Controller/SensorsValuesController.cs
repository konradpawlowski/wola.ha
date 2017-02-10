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
    class SensorsValuesController
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
                await Context.Instance.Connection.InsertAsync(new SensorTemperatureValues { Date = DateTime.Now, SensorId = 1, Value = 1.55m });
              //  Sensors sens = await Context.Instance.Connection.FindAsync<Sensors>(w => w.Id == id);
                List<SensorTemperatureValues> values = await Context.Instance.Connection.Table<SensorTemperatureValues>().Where(w => w.SensorId == id).ToListAsync();

                return new GetResponse(GetResponse.ResponseStatus.OK, values);

            }
            catch (Exception ex)
            {
                Log.e(ex);
               
            }

            return new GetResponse(GetResponse.ResponseStatus.OK);
        }
        //[UriFormat("/Backup")]
        //public async Task<IGetResponse> BackupDatabase()
        //{

        //    try
        //    {
        //        //OneDriveConnector one = new OneDriveConnector();
        //        //var ret = await one.LoginAsync("b2408249-7c76-4bf8-8232-0df10ee07af8", "Files.ReadWrite", $"https://onedrive.live.com/?id=A94142E2A64C6428%215120&cid=A94142E2A64C6428", "963F126E8A18B84ACF840D51E670CF5FD2845C9D");
                
        //        return new GetResponse(GetResponse.ResponseStatus.OK);

        //    }
        //    catch (Exception ex)
        //    {
        //        Log.e(ex);

        //    }

        //    return new GetResponse(GetResponse.ResponseStatus.OK);
        //}
    }
}

