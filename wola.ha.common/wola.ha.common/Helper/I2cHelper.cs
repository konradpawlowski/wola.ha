using System;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.I2c;
using wola.ha.common.Helper;

namespace wola.ha.common.Devices.I2c
{
    public class I2CHelper
    {
        private static string _aqs;
        private static DeviceInformationCollection _dis;

        public static async Task<byte[]> WriteRead(int slaveAddress, byte[] byteToBeSend)
        {
            byte[] receivedData = new byte[32];


            /* Arduino Nano's I2C SLAVE address */

            try
            {
                // Initialize I2C
                var settings = new I2cConnectionSettings(slaveAddress) { BusSpeed = I2cBusSpeed.StandardMode };

                if (_aqs == null || _dis == null)
                {

                    _aqs = I2cDevice.GetDeviceSelector("I2C1");
                    _dis = await DeviceInformation.FindAllAsync(_aqs);
                }


                using (I2cDevice device = await I2cDevice.FromIdAsync(_dis[0].Id, settings))
                {
                    var send = device.WritePartial((byteToBeSend));
                    if (send.Status == I2cTransferStatus.SlaveAddressNotAcknowledged) throw new Exception(String.Format("Urz¹dzenie i2c o adresie {0} nie jest dostêpne!",settings.SlaveAddress));
                        await Task.Delay(1000);
                    var a = device.ReadPartial(receivedData);
                    if (a.Status != I2cTransferStatus.FullTransfer)
                        throw new Exception("Nie ostrzymano komunikatu zwrotnego od urz¹dzenia: " + settings.SlaveAddress.ToString());
                    //  Debug.WriteLine(a.Status);

                }
            }
            catch (Exception ex)
            {
                // SUPPRESS ANY ERROR
                string val = String.Join(",", byteToBeSend);
                LoggerFactory.LogException(ex, "WriteRead", new { val });
            }

            /* Return received data or ZERO on error */
            return receivedData;
        }
        public static ArduinoI2CResponse VerifyIncomingData(byte[] bData)
        {
            try
            {


                Int16 sizeData = bData[0];
                int size = sizeData + 4;
                if (bData[1] != 2) throw new Exception("To nie jest typ Response");
                byte lo = bData[size - 2];
                byte hi = bData[size - 1];

                byte[] crcData = new byte[size - 2];
                Buffer.BlockCopy(bData, 0, crcData, 0, size - 2);
                Crc16Ccitt crc = new Crc16Ccitt(InitialCrcValue.NonZero1);
                Debug.WriteLine("CRC: " + crc.ComputeChecksum(crcData));
                var retByte = crc.ComputeChecksumBytes(crcData);

                if (!((retByte[0] == lo) && retByte[1] == hi)) throw new Exception("B³ad przesy³u danych! B³ad CRC");

                byte[] Data = new byte[size - 4];
                Buffer.BlockCopy(bData, 2, Data, 0, sizeData);

                return (ArduinoI2CResponse)GetObjectFromBytes(Data, typeof(ArduinoI2CResponse));
            }
            catch (Exception ex)
            {
                string val = String.Join(",", bData);
                LoggerFactory.LogException(ex, "VerifyIncomingData", new {val});
                return new ArduinoI2CResponse { Status = (int)I2CResponseStatus.Error };
                
                
            }

        }

        public static byte[] PrepareMessageToSend(I2CMessageFrame message)
        {
            byte[] mahByteArray = message.ToArray();
            byte[] crcFrame = new byte[Buffer.ByteLength(mahByteArray) + 2];

            byte[] bFrame = new byte[Buffer.ByteLength(mahByteArray) + 4];
            try
            {

               


                crcFrame[0] = (byte)Buffer.ByteLength(mahByteArray);
                crcFrame[1] = (byte)1;
                Buffer.BlockCopy(mahByteArray, 0, crcFrame, 2, Buffer.ByteLength(mahByteArray));

                Crc16Ccitt crc = new Crc16Ccitt(InitialCrcValue.NonZero1);
                var retByte = crc.ComputeChecksumBytes(crcFrame);
                Buffer.BlockCopy(crcFrame, 0, bFrame, 0, crcFrame.Count());
                Buffer.SetByte(bFrame, Buffer.ByteLength(bFrame) - 1, retByte[1]);
                Buffer.SetByte(bFrame, Buffer.ByteLength(bFrame) - 2, retByte[0]);

                
            }
            catch (Exception ex)
            {
                LoggerFactory.LogException(ex, "PrepareMessageToSend", new { message });
                
               
            }
            return bFrame;

        }

        public static object GetObjectFromBytes(byte[] buffer, Type objType)
        {
            object obj = null;
            if ((buffer != null) && (buffer.Length > 0))
            {
                IntPtr ptrObj = IntPtr.Zero;
                try
                {
                    int objSize = Marshal.SizeOf(objType);
                    if (objSize > 0)
                    {
                        if (buffer.Length < objSize)
                            throw new Exception(
                                String.Format("Buffer smaller than needed for creation of object of type {0}", objType));
                        ptrObj = Marshal.AllocHGlobal(objSize);
                        if (ptrObj != IntPtr.Zero)
                        {
                            Marshal.Copy(buffer, 0, ptrObj, objSize);
                            obj = Marshal.PtrToStructure(ptrObj, objType);
                        }
                        else
                            throw new Exception(String.Format("Couldn't allocate memory to create object of type {0}",
                                objType));
                    }
                }
                finally
                {
                    if (ptrObj != IntPtr.Zero)
                        Marshal.FreeHGlobal(ptrObj);
                }
            }
            return obj;
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