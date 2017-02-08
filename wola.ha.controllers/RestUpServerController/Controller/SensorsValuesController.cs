using Restup.Webserver.Attributes;
using Restup.Webserver.Models.Contracts;
using Restup.Webserver.Models.Schemas;
using RestUpServerController.Model;
using System;
using System.Diagnostics;
namespace RestUpServerController.Controller
{
    [RestController(InstanceCreationType.PerCall)]
    class SensorsValuesController
    {
        [UriFormat("/SensorValue")]
        public IPostResponse AddSensorValue([FromContent] SensorValueDataContent data)
        {
            Debug.WriteLine($"Received counter value of {data.Value}");
            return new PostResponse(PostResponse.ResponseStatus.Created, $"SensorValue/{data.Value}");
        }

        [UriFormat("/GetLastSensorValue")]
        public IGetResponse EncodingDecodingTest([FromContent] SensorValueDataContent data)
        {
            Debug.WriteLine($"Received complex data with text {data.Value}");
            return new GetResponse(GetResponse.ResponseStatus.OK, new SensorValueDataContent() { Value = 0, SensorKind = 1, SensorType = 1 });
        }
    }
}
