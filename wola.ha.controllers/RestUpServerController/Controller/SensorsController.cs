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
using wola.ha.common.Helper;
using wola.ha.common.Model;

namespace RestUpServerController.Controller
{
    [RestController(InstanceCreationType.PerCall)]
    class SensorsController
    {
        [UriFormat("/GetSensors")]
        public async Task<IGetResponse> GetSensors()
        {

            try
            {
                List<Sensors> values = await Context.Instance.Connection.Table<Sensors>().ToListAsync();

                foreach (Sensors item in values)
                {
                    await item.GetEx();
                }

                return new GetResponse(GetResponse.ResponseStatus.OK, values);

            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);

            }

            return new GetResponse(GetResponse.ResponseStatus.OK);
        }

        [UriFormat("/GetSensors/ByType/{type}")]
        public async Task<IGetResponse> GetSensorsByType(int type)
        {

            try
            {
                List<Sensors> values = await Context.Instance.Connection.Table<Sensors>().ToListAsync();
                foreach (Sensors item in values)
                {
                    await item.GetEx();
                }
                return new GetResponse(GetResponse.ResponseStatus.OK, values.Where(w=>w.SensorType == type).ToList());

            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);

            }

            return new GetResponse(GetResponse.ResponseStatus.OK);
        }
        [UriFormat("/GetSensors/ByKind/{type}")]
        public async Task<IGetResponse> GetSensorsByKind(int type)
        {

            try
            {
                List<Sensors> values = await Context.Instance.Connection.Table<Sensors>().ToListAsync();
                foreach (Sensors item in values)
                {
                    await item.GetEx();
                }
                return new GetResponse(GetResponse.ResponseStatus.OK, values.Where(w => w.SensorKind == type).ToList());

            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);

            }

            return new GetResponse(GetResponse.ResponseStatus.OK);
        }
        [UriFormat("/GetSensors/ByDataBus/{type}")]
        public async Task<IGetResponse> GetSensorsByDataBus(int type)
        {

            try
            {
                List<Sensors> values = await Context.Instance.Connection.Table<Sensors>().ToListAsync();
                foreach (Sensors item in values)
                {
                    await item.GetEx();
                }
                return new GetResponse(GetResponse.ResponseStatus.OK, values.Where(w => w.DataBus == type).ToList());

            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);

            }

            return new GetResponse(GetResponse.ResponseStatus.OK);
        }
    }
}
