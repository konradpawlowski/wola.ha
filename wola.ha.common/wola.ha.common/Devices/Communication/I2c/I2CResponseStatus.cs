namespace wola.ha.common.Devices.I2c
{
   public enum I2CResponseStatus
    {
        Brak = (short)0,
        Ok = (short)1,
        Error = (short)2,
        Warrning = (short)3,
        CrcError = (short)4
    };
}