using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace wola.ha.common.Helper
{
    public static class SerialHelper
    {

        //public static MessageDs18b20 GetObject(byte[] bData)
        //{
        //    Int16 sizeData = bData[0];
        //    int size = sizeData + 4;

        //    byte[] Data = new byte[sizeData];
        //    Buffer.BlockCopy(bData, 2, Data, 0, sizeData);

        //    return (MessageDs18b20)GetObjectFromBytes(Data, typeof(MessageDs18b20));
        //}
        public static bool VerifyCrcData(byte[] bData)
        {
            try
            {


                Int16 sizeData = bData[0];
                int size = sizeData + 4;
                byte lo = bData[size - 2];
                byte hi = bData[size - 1];

                byte[] crcData = new byte[size - 2];
                Buffer.BlockCopy(bData, 0, crcData, 0, size - 2);
                Crc16Ccitt crc = new Crc16Ccitt(InitialCrcValue.NonZero1);
                Debug.WriteLine("CRC: " + crc.ComputeChecksum(crcData));
                var retByte = crc.ComputeChecksumBytes(crcData);

                return (retByte[0] == lo && retByte[1] == hi);


            }
            catch (Exception ex)
            {
                string val = String.Join(",", bData);
                LoggerFactory.LogException(ex, "VerifyIncomingData", new { val });
                return false;


            }

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
