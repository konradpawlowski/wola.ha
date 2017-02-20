using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Http;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Restup.Webserver.Http;
using Restup.Webserver.Rest;
using wola.ha.common.DataModel;
using SQLite.Net;
using wola.ha.common.Enums;
using System.Diagnostics;
using wola.ha.common.Factory;
using Windows.UI.Xaml;
using Windows.System.Threading;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using wola.ha.common;
using wola.ha.common.Helper;
using Windows.System;
using System.Security.Claims;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

namespace SensorsTempReadController
{
    public sealed class StartupTask : IBackgroundTask
    {
        BackgroundTaskDeferral _defferal;

        public async void Run(IBackgroundTaskInstance taskInstance)
        {
            // 
            // TODO: Insert code to perform background work
            //
            // If you start any asynchronous methods here, prevent the task
            // from closing prematurely by using BackgroundTaskDeferral as
            // described in http://aka.ms/backgroundtaskdeferral
            //

            _defferal = taskInstance.GetDeferral();
            taskInstance.Canceled += TaskInstance_Canceled;
            LoggerFactory.InitialLogger();

            try
            {
                var timer = ThreadPoolTimer.CreatePeriodicTimer(Timer_Tick, TimeSpan.FromMinutes(5));
               // var timer1 = ThreadPoolTimer.CreatePeriodicTimer(Timer1_Tick, TimeSpan.FromMilliseconds(10000));


            }
            catch (Exception)
            {

                throw;
            }

           
        }

        private void Timer1_Tick(ThreadPoolTimer timer)
        {
            try
            {
                using (SQLiteConnection db = LocalDb.DbConnection)

                {
                    List<SensorTemperatureValues> values = db.Table<SensorTemperatureValues>().Where(w => w.SensorId == 1).OrderBy(o=>o.Date).ToList();
                    values.ForEach(s => Debug.WriteLine(s.Date));
                    
                }
            }
            catch (Exception)
            {

                throw;
            }
        }

        private async void Timer_Tick(ThreadPoolTimer timer)
        {
            await ReadTemperature();
        }



      
        


        private async Task ReadTemperature()
        {
            try
            {
                List<Sensors> tempSensors = await WolaClient.GetFilteredListFromControllerAction<Sensors>("GetSensors", "ByDataBus",1,null);
                    
                foreach (Sensors item in tempSensors)
                {
                   
                    SensorTemperatureValues val = await ReadTemperatureFromSensor(item);
                    if (val.Date != DateTime.MinValue)
                       await  PostTemp(val);
                }
            }
            catch(Exception ex)
            {
                throw;
            }
                
        }

        private async Task<SensorTemperatureValues> ReadTemperatureFromSensor(Sensors item)
        {
            ReadTemperatureFactory rtf = new ReadTemperatureFactory();
            return await rtf.ReadTemperature(item);
        }

        private async Task PostTemp(SensorTemperatureValues item)
        {
            try
            {
                await  WolaClient.PostItemToController<SensorTemperatureValues>("SensorTempValue", item);
            
            }
            catch (Exception ex)
            {

                Log.e(ex);
            }
     
        }

        private void TaskInstance_Canceled(IBackgroundTaskInstance sender, BackgroundTaskCancellationReason reason)
        {
            //a few reasons that you may be interested in.
            switch (reason)
            {
                case BackgroundTaskCancellationReason.Abort:
                    //app unregistered background task (amoung other reasons).
                    break;
                case BackgroundTaskCancellationReason.Terminating:
                    //system shutdown
                    break;
                case BackgroundTaskCancellationReason.ConditionLoss:
                    break;
                case BackgroundTaskCancellationReason.SystemPolicy:
                    break;
            }
            _defferal.Complete();
        }
    }
}
