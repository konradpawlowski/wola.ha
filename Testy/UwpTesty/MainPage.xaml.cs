using System;
using System.Diagnostics;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Porrey.Uwp.IoT.Devices;
using Porrey.Uwp.Ntp;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Windows.Devices.Gpio;
using WinRTXamlToolkit.Controls.DataVisualization.Charting;
using System.Collections.Generic;
using wola.ha.common.Helper;
using wola.ha.common.DataModel;
using System.Linq;
using System.ServiceModel.Channels;

//Szablon elementu Pusta strona jest udokumentowany na stronie https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x415

namespace UwpTesty
{
    /// <summary>
    /// Pusta strona, która może być używana samodzielnie lub do której można nawigować wewnątrz ramki.
    /// </summary>
    public sealed partial class MainPage : Page
    {

        private const int T1 = 5;
        private const int T2 = 6;
        private const int P1 = 19;
        private const int P2 = 13;
        GpioPin pt1;
        GpioPin pt2;
        GpioPin pp1;
        GpioPin pp2;



        public MainPage()
        {
            this.InitializeComponent();
            InitGPIO();
            
        }

        private async void Button_ClickAsync(object sender, RoutedEventArgs e)
        {

            Ds1307 ds = new Ds1307();
            NtpClient ntp = new NtpClient();
            try
            {
                

                
               // await ds.InitializeAsync();
               // Debug.Write(ds.DeviceAddress);
               // //////     DateTime? dt = await ntp.GetAsync(@"europe.pool.ntp.org");
               // if (ds.IsInitialized)
               //     Debug.WriteLine("zains");

               // var dto = new DateTimeOffset(DateTime.Now);
               // await ds.SetAsync(dto);

               //// var dtoa = await ds.GetAsync();
               //// Debug.WriteLine(dtoa.DateTime.ToLocalTime());



                await LoadChartContents();


            }

            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);


            }
            finally
            {
                ds.Dispose();
            }
        }
        private void InitGPIO()
        {
            var gpio = GpioController.GetDefault();

            // Show an error if there is no GPIO controller
            if (gpio == null)
            {
                //   pin = null;
                Debug.WriteLine("There is no GPIO controller on this device.");
                return;
            }

            pt1 = gpio.OpenPin(T1);
            pt2 = gpio.OpenPin(T2);
            pp1 = gpio.OpenPin(P1);
            pp2 = gpio.OpenPin(P2);
            pp1.SetDriveMode(GpioPinDriveMode.Output);
            pp2.SetDriveMode(GpioPinDriveMode.Output);
            pt1.SetDriveMode(GpioPinDriveMode.Output);
            pt2.SetDriveMode(GpioPinDriveMode.Output);

            pp1.Write(GpioPinValue.High);
            pp2.Write(GpioPinValue.High);
            pt1.Write(GpioPinValue.Low);
            pt2.Write(GpioPinValue.Low);



            Debug.WriteLine("GPIO pin initialized correctly.");

        }
        private async Task Test()
        {
            Ds1307 dtc = new Ds1307();
            NtpClient ntp = new NtpClient();
            try
            {

                await dtc.InitializeAsync();
                // ***
                // *** Create an NTP client and get the date and time
                // ***

                var ndt = await ntp.GetAsync("0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org", "3.pool.ntp.org");

                // ***
                // *** Update the clock if we have a result from the servers
                // ***
                if (ndt.HasValue)
                {
                    await dtc.SetAsync(ndt.Value);
                }
                // ***
                // *** Get the date and time from the clock
                // ***
                if (!dtc.IsInitialized) return;
                DateTimeOffset dt = await dtc.GetAsync();


            }
            catch (Exception ex)
            {

                Debug.WriteLine(ex.Message);
            }
            finally
            {
                dtc.Dispose();

            }

        }
        public static byte[] StringToByteArray(String hex)
        {
            int numberChars = hex.Length;
            byte[] bytes = new byte[numberChars / 2];
            for (int i = 0; i < numberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            pt1.Write(pt1.Read() == GpioPinValue.High ? GpioPinValue.Low : GpioPinValue.High);
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            pt2.Write(pt2.Read() == GpioPinValue.High ? GpioPinValue.Low : GpioPinValue.High);
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            pp1.Write(pp1.Read() == GpioPinValue.High ? GpioPinValue.Low : GpioPinValue.High);
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            pp2.Write(pp2.Read() == GpioPinValue.High ? GpioPinValue.Low : GpioPinValue.High);
        }

        private async Task LoadChartContents()
        {
            Random rand = new Random();
            List<Records> records = new List<Records>();
            records.Add(new Records()
            {
                Date = DateTime.Now,
                Value = 23.0
            });
            records.Add(new Records()
            {
                Date = DateTime.Now,
                Value = 21.5
            });
            records.Add(new Records()
            {
                Date = DateTime.Now,
                Value = 28.1
            });
            records.Add(new Records()
            {
                Date = DateTime.Now,
                Value = 25.6
            });
            //(PieChart.Series[0] as PieSeries).ItemsSource = records;
            //(ColumnChart.Series[0] as ColumnSeries).ItemsSource = records;
           // (lineChart.Series[0] as LineSeries).ItemsSource = records;
            List<SensorTemperatureValues> tempSensors2 = new List<SensorTemperatureValues>();
            tempSensors2.Add(new SensorTemperatureValues
            {
                Date = DateTime.Now,
                SensorId = 8,
                Value = 22.5
            });

            tempSensors2.Add(new SensorTemperatureValues
            {
                Date = DateTime.Now,
                SensorId = 8,
                Value = 24.4
            });

           
            tempSensors2.Add(new SensorTemperatureValues
            {
                Date = DateTime.Now,
                SensorId = 8,
                Value = 21.3
            });

            tempSensors2.Add(new SensorTemperatureValues
            {
                Date = DateTime.Now,
                SensorId = 8,
                Value = 27
            });

            try
            {
                      List<SensorTemperatureValues> tempSensors = await WolaClient.GetFilteredListFromControllerAction<SensorTemperatureValues>("GetLastSensorValue", 1, null);
                    List<SensorTemperatureValues> tempSensors1 = tempSensors.Where(w => w.Date.Date > DateTime.Today.AddDays(-3) ).ToList();//&& w.Date.Hour > 9
                                                                                                                                            // (lineChart.Series[0] as LineSeries).DependentRangeAxis = new LinearAxis() { Minimum = 200, Maximum = 300};
                                                                                                                                            // (lineChart.Series[0] as LineSeries).ItemsSource = tempSensors2;
                AddSeries(tempSensors1, "Spaliny");
                //AddSeries(records);

               //tempSensors = await WolaClient.GetFilteredListFromControllerAction<SensorTemperatureValues>("GetLastSensorValue", 6, null);
               //tempSensors1 = tempSensors.Where(w => w.Date.Date == DateTime.Today && w.Date.Hour > 9).ToList();
               // AddSeries(tempSensors1, "CUW");
               // tempSensors = await WolaClient.GetFilteredListFromControllerAction<SensorTemperatureValues>("GetLastSensorValue", 7, null);
               //tempSensors1 = tempSensors.Where(w => w.Date.Date == DateTime.Today && w.Date.Hour > 9).ToList();
               // AddSeries(tempSensors1,"CO");

            }
            catch (Exception)
            {

                throw;
            }
        }

        private void AddSeries<T>(List<T> tempSensors, string name)
        {
            var series1 = new LineSeries();
            series1.ItemsSource = tempSensors;
            //  series1.DataContext = Power;
            series1.DependentValuePath = "Value";
            series1.IndependentValuePath = "Date";
            series1.Title = name;

            //series1.Style = (Style)this.Resources["LineSeriesStyle1"];

            //set initial values:
            //LinearAxis independentaxis = new LinearAxis();
            //independentaxis.Orientation = AxisOrientation.X;
            //independentaxis.ShowGridLines = true;
            //independentaxis.Maximum = 60;
            //independentaxis.Minimum = 0;
            //independentaxis.Title = "Time";
            //independentaxis.ShowGridLines = true;

            //series1.IndependentAxis = independentaxis;

            //set initial values:
            //LinearAxis dependentaxis = new LinearAxis();
            //dependentaxis.Orientation = AxisOrientation.Y;
            //dependentaxis.ShowGridLines = true;
            //dependentaxis.Maximum = 70;
            //dependentaxis.Minimum = 20;
            //dependentaxis.Interval = 5;
            //dependentaxis.Title = "Stopnie";
            //dependentaxis.ShowGridLines = true;
            //dependentaxis.IsDoubleTapEnabled = true;
            //dependentaxis.IsEnabled = true;
            //series1.DependentRangeAxis = dependentaxis;

            lineChart.Series.Add(series1);


        }


    }
    public class SensorDht
    {
        public int Pin { get; set; }
        public float Temperature { get; set; }
        public float Humidity { get; set; }
        public DateTime Date { get; set; }
    }
    public class Records
    {
        public DateTime Date
        {
            get;
            set;
        }
        public double Value
        {
            get;
            set;
        }
    }

}
