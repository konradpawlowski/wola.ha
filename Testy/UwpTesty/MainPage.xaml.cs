using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using wola.ha.common.Helper;
using wola.ha.common.Devices.I2c;
using wola.ha.common.Enums;
using Porrey.Uwp.IoT.Devices;
using Porrey.Uwp.Ntp;
using System.Threading.Tasks;


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

                await Test();

                //Debug.WriteLine(ntp.ToString());
                //await ds.InitializeAsync();
                //Debug.Write(ds.DeviceAddress);
                ////     DateTime? dt = await ntp.GetAsync(@"europe.pool.ntp.org");
                // await ds.SetAsync(new DateTimeOffset(new DateTime(2017, 2, 16, 8, 39, 45)));

                //var dto = await ds.GetAsync();
                //Debug.WriteLine(dto.DateTime.ToLocalTime());


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

}
