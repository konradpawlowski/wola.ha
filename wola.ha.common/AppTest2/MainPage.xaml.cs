using AdafruitClassLibrary;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Devices.Gpio;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace AppTest2
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        Mcp23017 mcp;
        //private const int LED_PIN = 6;
        private const int BUTTON_PIN = 4;
        //private GpioPin ledPin;
        private GpioPin buttonPin;
        public MainPage()
        {
            this.InitializeComponent();
            if (mcp == null) Task.Run(() => Init()).Wait();
            InitGPIO();
        }

        private async System.Threading.Tasks.Task Init()
        {
            mcp = new Mcp23017(0x20);
            await mcp.InitMCP23017Async();
            mcp.pinMode(0, Mcp23017.Direction.INPUT);
            mcp.pullUp(0, Mcp23017.Level.HIGH);
            mcp.pinMode(1, Mcp23017.Direction.INPUT);
            mcp.pullUp(1, Mcp23017.Level.HIGH);
            mcp.pinMode(8, Mcp23017.Direction.OUTPUT);
            mcp.pinMode(9, Mcp23017.Direction.OUTPUT);
            mcp.pinMode(10, Mcp23017.Direction.OUTPUT);
        }



        private void Button_Click(object sender, RoutedEventArgs e)
        {


            // Will block until the task is completed...


            Debug.WriteLine("Pin 0: " + mcp.digitalRead(0));
            Debug.WriteLine("Pin 1: " + mcp.digitalRead(1));


            mcp.digitalWrite(8, Mcp23017.Level.HIGH);
            //await Task.Delay(1000);
            Task.Run(() => Task.Delay(1000)).Wait();
            mcp.digitalWrite(8, Mcp23017.Level.LOW);


        }
        private void InitGPIO()
        {
            var gpio = GpioController.GetDefault();

            // Show an error if there is no GPIO controller
            if (gpio == null)
            {
                Debug.WriteLine("There is no GPIO controller on this device.");
                return;
            }
            buttonPin = gpio.OpenPin(BUTTON_PIN);
            // Check if input pull-up resistors are supported
            if (buttonPin.IsDriveModeSupported(GpioPinDriveMode.InputPullUp))
                buttonPin.SetDriveMode(GpioPinDriveMode.InputPullUp);
            else
                buttonPin.SetDriveMode(GpioPinDriveMode.Input);


            // Set a debounce timeout to filter out switch bounce noise from a button press
            buttonPin.DebounceTimeout = TimeSpan.FromMilliseconds(50);

            // Register for the ValueChanged event so our buttonPin_ValueChanged 
            // function is called when the button is pressed
            buttonPin.ValueChanged += buttonPin_ValueChanged;
        }

        private void buttonPin_ValueChanged(GpioPin sender, GpioPinValueChangedEventArgs args)
        {
            Debug.WriteLine("Pin 0: " + mcp.digitalRead(0));
            Debug.WriteLine("Pin 1: " + mcp.digitalRead(1));
        }
    }
}
