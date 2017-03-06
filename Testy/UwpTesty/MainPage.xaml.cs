﻿using System;
using System.Diagnostics;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Porrey.Uwp.IoT.Devices;
using Porrey.Uwp.Ntp;
using System.Threading.Tasks;
using Newtonsoft.Json;


//Szablon elementu Pusta strona jest udokumentowany na stronie https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x415

namespace UwpTesty
{
    /// <summary>
    /// Pusta strona, która może być używana samodzielnie lub do której można nawigować wewnątrz ramki.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void Button_ClickAsync(object sender, RoutedEventArgs e)
        {

            Ds1307 ds = new Ds1307();
            NtpClient ntp = new NtpClient();
            try
            {
                //SensorDht dht = new SensorDht();
                //dht.Humidity = 38;
                //dht.Temperature = 25;
                //dht.Pin = 5;
                //dht.Date = DateTime.Now;

               
                //string str = JsonConvert.SerializeObject(dht);
                //Debug.WriteLine(str);


                //"2017-02-27T14:17:41.9489929+01:00"
                //string data = "2017-02-27T14:17:41.9489929+01:00";
                //DateTime dd = new DateTime();
                //if (DateTime.TryParse(data, out dd))
                //    Debug.WriteLine("Parse OK 1");
                //else
                //    Debug.WriteLine("Parse NOT OK 1");
                //data = "2017-02-27T14:17:41";
                //if (DateTime.TryParse(data, out dd))
                //    Debug.WriteLine("Parse OK 2");
                //else
                //    Debug.WriteLine("Parse NOT OK 2");

                //string ddd = "{ \"Pin\":5,\"Temperature\":25.0,\"Humidity\":38.0,\"Date\":\"2016-03-27T14:25:02\"} "   ;
                //SensorDht da = JsonConvert.DeserializeObject<SensorDht>(ddd);
                //Debug.WriteLine(da.Date.ToString());
                //// await Test();

                Debug.WriteLine(ntp.ToString());
                await ds.InitializeAsync();
                Debug.Write(ds.DeviceAddress);
                ////     DateTime? dt = await ntp.GetAsync(@"europe.pool.ntp.org");
                if (ds.IsInitialized)
                    Debug.WriteLine("zains");

                    var dto = new DateTimeOffset(DateTime.Now);
                 await ds.SetAsync(dto);

                var dtoa = await ds.GetAsync();
                Debug.WriteLine(dtoa.DateTime.ToLocalTime());


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
    }
    public class SensorDht
    {
        public int Pin { get; set; }
        public float Temperature { get; set; }
        public float Humidity { get; set; }
        public DateTime Date { get; set; }
    }

}
