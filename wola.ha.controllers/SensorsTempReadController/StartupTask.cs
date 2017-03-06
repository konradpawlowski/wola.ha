using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using wola.ha.common.DataModel;
using SQLite.Net;
using wola.ha.common.Enums;
using System.Diagnostics;
using wola.ha.common.Factory;
using Windows.System.Threading;
using Newtonsoft.Json;
using wola.ha.common;
using wola.ha.common.Helper;
using Windows.Devices.SerialCommunication;
using Windows.Devices.Enumeration;
using Windows.Storage.Streams;
using System.Threading;
using wola.ha.common.Model.Serial;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

namespace SensorsTempReadController
{
    public sealed class StartupTask : IBackgroundTask
    {
       //  USART Usart { get; set; }
        BackgroundTaskDeferral _defferal;
        private SerialDevice serialPort = null;
        DataWriter dataWriteObject = null;
        DataReader dataReaderObject = null;
        private CancellationTokenSource ReadCancellationTokenSource;
        private static Semaphore semaphore = new Semaphore(1, 1);
        public async void Run(IBackgroundTaskInstance taskInstance)
        {
            // 
            // TODO: Insert code to perform background work
            //
            // If you start any asynchronous methods here, prevent the task
            // from closing prematurely by using BackgroundTaskDeferral as
            // described in http://aka.ms/backgroundtaskdeferral
            //

          

            try
            {

           
                _defferal = taskInstance.GetDeferral();
                await InitSerial();
                ReadCancellationTokenSource = new CancellationTokenSource();
                Listen();
                LoggerFactory.InitialLogger();
                


             //   var timer = ThreadPoolTimer.CreatePeriodicTimer(Timer_Tick, TimeSpan.FromMinutes(5));
               // var timer1 = ThreadPoolTimer.CreatePeriodicTimer(Timer1_Tick, TimeSpan.FromMilliseconds(10000));


            }
            catch (Exception)
            {

                throw;
            }

           
        }

      #region configurationSerial
       
        private async Task InitSerial()
        {

            try
            {
                //pendSemphore();
                string portName = "UART0";

                string aqs = SerialDevice.GetDeviceSelector(portName);

                var myDevices = await DeviceInformation.FindAllAsync(aqs, null);

                if (myDevices.Count == 0)
                {
                    throw new Exception("Device not found!");

                }

                DeviceInformation entry = myDevices[0];


                serialPort = await SerialDevice.FromIdAsync(entry.Id);
                serialPort.WriteTimeout = TimeSpan.FromMilliseconds(1000);
                serialPort.ReadTimeout = TimeSpan.FromMilliseconds(1000);
                serialPort.BaudRate = 115200;
                serialPort.Parity = SerialParity.None;
                serialPort.StopBits = SerialStopBitCount.One;
                serialPort.DataBits = 8;
                serialPort.Handshake = SerialHandshake.None;
                LoggerFactory.LogInfo(serialPort.PortName);
               
            }
            catch (Exception ex)
            {
                //CancelReadTask();
                LoggerFactory.LogExceptionStack(ex,"Task");
                throw;
            }
           

        }
        /// <summary>
        /// - Create a DataReader object
        /// - Create an async task to read from the SerialDevice InputStream
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void Listen()
        {
            try
            {
                if (serialPort != null)
                {
                    dataReaderObject = new DataReader(serialPort.InputStream);

                    // keep reading the serial input';'[[;
                    while (true)
                    {
                        await ReadAsync(ReadCancellationTokenSource.Token);
                    }
                }
            }
            catch (TaskCanceledException tce)
            {
                LoggerFactory.LogException(tce);
                CloseDevice();
            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex);
            }
            finally
            {
                // Cleanup once complete
                if (dataReaderObject != null)
                {
                    dataReaderObject.DetachStream();
                    dataReaderObject = null;
                }
            }
        }

        /// <summary>
        /// ReadAsync: Task that waits on data and reads asynchronously from the serial device InputStream
        /// </summary>
        /// <param name="cancellationToken"></param>
        /// <returns></returns>
        private async Task ReadAsync(CancellationToken cancellationToken)
        {
            Task<UInt32> loadAsyncTask;

            uint ReadBufferLength = 1024;

            // If task cancellation was requested, comply
            cancellationToken.ThrowIfCancellationRequested();

            // Set InputStreamOptions to complete the asynchronous read operation when one or more bytes is available
            dataReaderObject.InputStreamOptions = InputStreamOptions.Partial;

            // Create a task object to wait for data on the serialPort.InputStream
            loadAsyncTask = dataReaderObject.LoadAsync(ReadBufferLength).AsTask(cancellationToken);

            // Launch the task and wait
            UInt32 bytesRead = await loadAsyncTask;
            if (bytesRead > 0)
            {
                
                try
                {
                    
                    string input = "";
                    input = dataReaderObject.ReadString(bytesRead);// + Environment.NewLine;
                    var aaa = input.Split('\n');
                    foreach (string item in aaa)
                    {
                        try
                        {
                            if (item.Length == 0) continue;
                            SerialMessage msg = JsonConvert.DeserializeObject<SerialMessage>(item);

                            if (msg.Reciver != 0) return; //komunikat nie jest dla mnie

                            SerialMessageFactory smf = new SerialMessageFactory();
                            await smf.ManageMessage(msg);
                        }
                        catch (Exception ex)
                        {

                            Debug.WriteLine(ex.Message);
                        }
                       
                    }
                                      
                }
                catch (Exception  ex)
                {

                    Debug.WriteLine(ex.Message);
                }
            }
        }

        /// <summary>
        /// CancelReadTask:
        /// - Uses the ReadCancellationTokenSource to cancel read operations
        /// </summary>
        private void CancelReadTask()
        {
            if (ReadCancellationTokenSource != null)
            {
                if (!ReadCancellationTokenSource.IsCancellationRequested)
                {
                    ReadCancellationTokenSource.Cancel();
                }
            }
        }

        /// <summary>
        /// CloseDevice:
        /// - Disposes SerialDevice object
        /// - Clears the enumerated device Id list
        /// </summary>
        private void CloseDevice()
        {
            if (serialPort != null)
            {
                serialPort.Dispose();
            }
            serialPort = null;

           // comPortInput.IsEnabled = true;
            //sendTextButton.IsEnabled = false;
           // rcvdText.Text = "";
           // listOfDevices.Clear();
        }

        #endregion
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
