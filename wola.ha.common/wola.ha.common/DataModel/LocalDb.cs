using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Contacts;
using Windows.Storage;
using SQLite.Net;
using SQLite.Net.Platform.WinRT;

namespace wola.ha.common.DataModel
{
    public class LocalDb
    {
        private static readonly string DbName = "wola.ha.db.sqlite";
        private static readonly string DbPath = Path.Combine(ApplicationData.Current.LocalFolder.Path,DbName);
        private static SQLiteConnection DbConnection => new SQLiteConnection(new SQLitePlatformWinRT(),DbPath);

        public static void CreateDatabase()
        {


            try
            {
                using (SQLiteConnection conn = DbConnection)

                {
                    // Activate Tracing 
                    conn.TraceListener = new DebugTraceListener();

                    conn.CreateTable<SensorType>();
                    conn.CreateTable<DataBusType>();
                    conn.CreateTable<Sensors>();
                    conn.CreateTable<SensorTemperatureValues>();
                    conn.CreateTable<SensorHumidityValues>();
                    conn.CreateTable<SensorOnOffValue>();

                }
            }
            catch (Exception)
            {
                
                throw;
            }
        }

        private static void FillDb()
        {
            using (var db = DbConnection)
            {
                List<DataBusType> dataBusTypes = new List<DataBusType>();
                dataBusTypes.Add(new DataBusType {Id = 1, Type = "I2C", Name = "Arduino 1" , Descript = "Arduino w kotłowni", Address = "0x40"});
                dataBusTypes.Add(new DataBusType {Id = 2, Type = "I2C", Name = "Arduino 2" , Descript = "Arduino strych", Address = "0x40"});
                dataBusTypes.Add(new DataBusType {Id = 3, Type = "I2C", Name = "BMP180", Descript = "Czujnik ciśnienia", Address = "0x40"});
                dataBusTypes.Add(new DataBusType {Id = 4, Type = "I2C", Name = "Expander In", Descript = "Rozszerzenie wejść", Address = "0x40"});
                dataBusTypes.Add(new DataBusType {Id = 5, Type = "I2C", Name = "Expander OUT", Descript = "Rozszerzenie wyjść", Address = "0x40"});
                dataBusTypes.Add(new DataBusType {Id = 6, Type = "Raspberry", Name = "Raspberry", Descript = "Raspberry", Address = "0x40"});


               


                List<SensorType> sensorTypes = new List<SensorType>();
                sensorTypes.Add(new SensorType {Type = "DS18B20", Descript = "Temperatura" });
                sensorTypes.Add(new SensorType {Type = "DHT11", Descript = "Temperatura i wilgotność" });
                sensorTypes.Add(new SensorType {Type = "DHT22", Descript = "Temperatura i wilgotność" });
                sensorTypes.Add(new SensorType {Type = "BMP180", Descript = "Temperatura i ciśnienie" });
                sensorTypes.Add(new SensorType {Type = "ACS712", Descript = "Czujnik przepływu prądu" });
                sensorTypes.Add(new SensorType {Type = "OnOff", Descript = "Czujnik On / Off" });


                List<Sensors> sensors = new List<Sensors>();
                sensors.Add(new Sensors {Id = 1, DataBusTypeId = 1 , SensorType = "DHT22", Name = "Temperatura na zewnątrz", Descript = "Temperatura na zewnątrz", Pin = null});
                sensors.Add(new Sensors {Id = 2, DataBusTypeId = 1 , SensorType = "DHT22", Name = "Wilgotność na zewnątrz", Descript = "Wilgotność na zewnątrz", Pin = null});
                sensors.Add(new Sensors {Id = 3, DataBusTypeId = 6 , SensorType = "BMP180", Name = "Ciśnienie na zewnątrz", Descript = "Ciśnienie na zewnątrz", Pin = null});
                sensors.Add(new Sensors {Id = 4, DataBusTypeId = 4 , SensorType = "OnOff", Name = "Czujnik zmierzchowy", Descript = "Czujnik zmierzchowy", Pin = "GPA7"});
                sensors.Add(new Sensors {Id = 5, DataBusTypeId = 4 , SensorType = "OnOff", Name = "Brama garażu", Descript = "Brama garażu", Pin = "GPA6"});
                sensors.Add(new Sensors {Id = 6, DataBusTypeId = 1 , SensorType = "DS18B20", Name = "CUW", Descript = "Woda CUW", Address = "28FC572800008035" });
                sensors.Add(new Sensors {Id = 7, DataBusTypeId = 1 , SensorType = "DS18B20", Name = "CO", Descript = "Temperatura na piecu", Address = "287C35280000804D" });
                sensors.Add(new Sensors {Id = 8, DataBusTypeId = 1 , SensorType = "DS18B20", Name = "Spaliny", Descript = "Temperatura spalin", Address = "280B2328000080B0" });

                sensors.Add(new Sensors {Id = 9, DataBusTypeId = 2 , SensorType = "DS18B20", Name = "Sypialnia balkon", Descript = "Sypialnia balkon", Address = "28FF1BFB54160463" });
                sensors.Add(new Sensors {Id = 10, DataBusTypeId = 2 , SensorType = "DS18B20", Name = "Sypialnia TV", Descript = "Sypialnia TV", Address = "28FF18FF54160423" });
                sensors.Add(new Sensors {Id = 11, DataBusTypeId = 2 , SensorType = "DS18B20", Name = "Strych", Descript = "Strych", Address = "28FFCB3257160473" });
                sensors.Add(new Sensors {Id = 12, DataBusTypeId = 2 , SensorType = "DS18B20", Name = "Patryk", Descript = "Patryk", Address = "28FF64DB5616045E" });
                sensors.Add(new Sensors {Id = 13, DataBusTypeId = 2 , SensorType = "DS18B20", Name = "Julia", Descript = "Julia", Address = "28FF6B4157160460" });

                
                db.InsertOrReplaceAll(sensorTypes);
                db.InsertOrReplaceAll(dataBusTypes);
                db.InsertOrReplaceAll(sensors);

            }
        }
    }
}
