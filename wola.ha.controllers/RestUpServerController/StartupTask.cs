using System;
using Windows.ApplicationModel.Background;
using Restup.Webserver.Rest;
using Restup.Webserver.Http;
using Restup.Webserver.File;
using RestUpServerController.Controller;
using wola.ha.common.Model;
using RestUpServerController.Controller.Sensors;
using Restup.Webserver;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

namespace RestUpServerController
{
    public sealed class StartupTask : IBackgroundTask
    {
        private HttpServer _httpServer;
        private BackgroundTaskDeferral _deferral;
        public async void Run(IBackgroundTaskInstance taskInstance)
        {
            // 
            // TODO: Insert code to perform background work
            //
            // If you start any asynchronous methods here, prevent the task
            // from closing prematurely by using BackgroundTaskDeferral as
            // described in http://aka.ms/backgroundtaskdeferral
            //
            var restRouteHandler = new RestRouteHandler();

            try
            {
                _deferral = taskInstance.GetDeferral();

                // initialize sqlite context
                await Context.Initialize();


                restRouteHandler.RegisterController<SensorTemperatureValueControler>();
                restRouteHandler.RegisterController<SensorHumidityValueController>();
                restRouteHandler.RegisterController<SensorPressureValueController>();
                restRouteHandler.RegisterController<SensorsController>();
                restRouteHandler.RegisterController<SensorValuesController>();
                restRouteHandler.RegisterController<SensorOnOffValueController>();
                


                var configuration = new HttpServerConfiguration()
              .ListenOnPort(8800)
                .RegisterRoute("api", restRouteHandler)
                
              //  .RegisterRoute(new StaticFileRouteHandler(@"wola.ha\wola.ha.view.web\Web"))
              .RegisterRoute(new StaticFileRouteHandler(@"wola.ha.view.web\Web"))
              .EnableCors(); // allow cors requests on all origins
                             //  .EnableCors(x => x.AddAllowedOrigin("http://specificserver:<listen-port>"));

                var httpServer = new HttpServer(configuration);
                _httpServer = httpServer;
              //  LogManager.SetLogFactory(new DebugLogFactory());
              
                await httpServer.StartServerAsync();
            }
            catch (Exception ex)
            {

                throw ex;
            }

           


        }
    }
}
