using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage.Streams;
using Windows.Devices.SerialCommunication;
using System.Threading;
using Windows.Devices.Enumeration;
using System.Runtime.InteropServices.WindowsRuntime;

namespace SensorsTempReadController
{

   // [ReadOnlyArray]
    public delegate void DataRecievedDlg([ReadOnlyArray] byte[] data);
    class USART
    {
        #region Private Vars
        DataReader dataReaderObject = null;
        private CancellationTokenSource ReadCancellationTokenSource;

        private uint _baudrate;
        private SerialParity _parity;
        private SerialStopBitCount _stopBits;
        private ushort _dataBits;
        #endregion
        #region Public Vars
        public SerialDevice serialPort { get; private set; }
        public event DataRecievedDlg DataRecieved;
        #endregion
        #region Private Functions
        public USART()
        {
            serialPort = null;
        }
        private async void Listen()
        {
            try
            {
                if (serialPort != null)
                {
                    dataReaderObject = new DataReader(serialPort.InputStream);
                    // keep reading the serial input
                    while (true)
                    {
                        byte[] x = await ReadAsync(ReadCancellationTokenSource.Token);
                        if (x.Length > 0)
                            DataRecieved(x);
                    }
                }
            }
            catch (Exception ex)
            {
                if (ex.GetType().Name != "TaskCanceledException")
                //{
                //    CloseDevice();
                //}
                //else
                {
                    throw new Exception(ex.Message);
                }
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
        private async Task<byte[]> ReadAsync(CancellationToken cancellationToken)
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
            byte[] bytes = new byte[bytesRead];
            if (bytesRead > 0)
                dataReaderObject.ReadBytes(bytes);
            return bytes;
        }
        #endregion
        #region Public Functions
        public void CloseDevice()
        {
            if (serialPort != null)
            {
                serialPort.Dispose();
            }
            serialPort = null;
        }
        public void StopListening()
        {
            if (ReadCancellationTokenSource != null)
            {
                if (!ReadCancellationTokenSource.IsCancellationRequested)
                {
                    ReadCancellationTokenSource.Cancel();
                }
            }
        }
        public async Task Initialize(uint baudrate, SerialParity parity, SerialStopBitCount stopBits, ushort dataBits)
        {
            try
            {
                string aqs = SerialDevice.GetDeviceSelector();
                var dis = await DeviceInformation.FindAllAsync(aqs);
                serialPort = await SerialDevice.FromIdAsync(dis[0].Id);
                serialPort.WriteTimeout = TimeSpan.FromMilliseconds(1000);
                serialPort.ReadTimeout = TimeSpan.FromMilliseconds(1000);
                serialPort.BaudRate = baudrate;
                serialPort.Parity = parity;
                serialPort.StopBits = stopBits;
                serialPort.DataBits = dataBits;
                serialPort.Handshake = SerialHandshake.None;

                _baudrate = baudrate;
                _parity = parity;
                _stopBits = stopBits;
                _dataBits = dataBits;
            }
            catch (Exception ex)
            {
                throw new Exception(ex.Message);
            }
        }
        public void StartListening()
        {
            ReadCancellationTokenSource = new CancellationTokenSource();
            Listen();
        }
        public async Task<uint> WriteAsync(byte[] data)
        {
            DataWriter dataWriter = new DataWriter();
            dataWriter.WriteBytes(data);
            uint bytesWritten = await serialPort.OutputStream.WriteAsync(dataWriter.DetachBuffer());
            return bytesWritten;
        }
        #endregion
    }

}
