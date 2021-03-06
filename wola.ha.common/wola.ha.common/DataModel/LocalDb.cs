﻿using System;
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
    public static class LocalDb
    {
        
        private const string DbName = "wola.ha.db.sqlite";
        private static readonly string DbPath = Path.Combine(ApplicationData.Current.LocalFolder.Path , DbName);
      //  private static readonly string DbPath = Path.Combine(@"C:\temp\", DbName);
        public static SQLiteConnection DbConnection => new SQLiteConnection(new SQLitePlatformWinRT(), DbPath);

        public async static void CreateDatabase()
        {

          //  if(!CheckDbAsync())
            try
            {
                using (SQLiteConnection conn = DbConnection)

                {
                    // Activate Tracing 
                    conn.TraceListener = new DebugTraceListener();

                    conn.CreateTable<SensorKind>();
                    conn.CreateTable<DataBusType>();
                    conn.CreateTable<DataBus>();
                    conn.CreateTable<SensorType>();
                    conn.CreateTable<Sensors>();
                    conn.CreateTable<SensorTemperatureValues>();
                    conn.CreateTable<SensorHumidityValues>();
                    conn.CreateTable<SensorOnOffValue>();
                    conn.CreateTable<SensorPressureValues>();

                    FillDb();
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
                List<DataBusType> dataBusType = new List<DataBusType>
                {
                    new DataBusType
                    {
                        Id = 0,
                        Type = "pi"

                    },
                    new DataBusType
                    {
                        Id = 1,
                        Type = "i2c"
                    },
                     new DataBusType
                    {
                        Id = 2,
                        Type = "spi"
                    },
                      new DataBusType
                    {
                        Id = 3,
                        Type = "restup"
                    }
                     
                };
                List<SensorKind> dataSensorKind = new List<SensorKind>
                {
                    new SensorKind{Id = 1, Kind = "Temperature"},
                    new SensorKind{Id = 2, Kind = "Humidity"},
                    new SensorKind{Id = 3, Kind = "Presure"},
                    new SensorKind{Id = 4, Kind = "OnOff"},
                    new SensorKind{Id = 5, Kind = "Electricity"}
                };
                List<DataBus> dataBus = new List<DataBus>
                {
                     new DataBus
                    {
                        Id = 0,
                        Type = 0,
                        Name = "Internal GPIO",
                        Descript = "Gpio rapberry pi"
                    },
                    new DataBus
                    {
                        Id = 1,
                        Type = 1,
                        Name = "Arduino 1",
                        Descript = "Arduino kotłownia",
                        Address = "0x40"
                    },
                    new DataBus
                    {
                        Id = 2,
                        Type = 1,
                        Name = "Arduino 2",
                        Descript = "Arduino Strych",
                        Address = "0x41"
                    }   ,
                    new DataBus
                    {
                        Id = 3,
                        Type = 1,
                        Name = "BMP180",
                        Descript = "Czujnik ciśnienia",
                        Address = "0x77"
                    },
                     new DataBus
                    {
                        Id = 4,
                        Type = 1,
                        Name = "PCF8591",
                        Descript = "Przetwornik A/C",
                        Address = "0x48"
                    },
                    new DataBus
                    {
                        Id = 5,
                        Type = 1,
                        Name = "MCP16_IN",
                        Descript = "Expander wejść",
                        Address = "0x20"
                    },
                    new DataBus
                    {
                        Id = 6,
                        Type = 1,
                        Name = "MCP16_OUT",
                        Descript = "Expander wyjść",
                        Address = "0x27"
                    }
                   
                };
                List<SensorType> sensorTypes = new List<SensorType>
                {
                    new SensorType {Id = 1, Type = "DS18B20", Descript = "Temperatura"},
                    new SensorType {Id = 2,Type = "DHT11", Descript = "Temperatura i wilgotność"},
                    new SensorType {Id = 3,Type = "DHT22", Descript = "Temperatura i wilgotność"},
                    new SensorType {Id = 4,Type = "BMP180", Descript = "Temperatura i ciśnienie"},
                    new SensorType {Id = 5,Type = "ACS712", Descript = "Czujnik przepływu prądu"},
                    new SensorType {Id = 6,Type = "OnOff", Descript = "Czujnik On / Off"}
                };
                List<Sensors> sensors = new List<Sensors>
                {
                    new Sensors{Id = 1, DataBus = 1,  SensorType = 3, SensorKind = 1,  Name = "Temperatura na zewnątrz",    Descript = "Temperatura na zewnątrz",   Pin = "4" },
                    new Sensors{Id = 2, DataBus = 1,  SensorType = 3, SensorKind = 2,  Name = "Wilgotność na zewnątrz",     Descript = "Wilgotność na zewnątrz",    Pin = "4"},
                    new Sensors{Id = 3, DataBus = 0,  SensorType = 4, SensorKind = 3,  Name = "Ciśnienie na zewnątrz",      Descript = "Ciśnienie na zewnątrz"},
                    new Sensors{Id = 4, DataBus = 5,  SensorType = 6, SensorKind = 4,  Name = "Czujnik zmierzchowy",        Descript = "Czujnik zmierzchowy",       Pin = "GPA7"},
                    new Sensors{Id = 5, DataBus = 6,  SensorType = 6, SensorKind = 4,  Name = "Brama garażu",               Descript = "Brama garażu",              Pin = "GPA6"},
                    new Sensors{Id = 6, DataBus = 1,  SensorType = 1, SensorKind = 1,  Name = "CUW",                        Descript = "Woda CUW",                  Address = "28FC572800008035"},
                    new Sensors{Id = 7, DataBus = 1,  SensorType = 1, SensorKind = 1,  Name = "CO",                         Descript = "Temperatura na piecu",      Address = "287C35280000804D"},
                    new Sensors{Id = 8, DataBus = 1,  SensorType = 1, SensorKind = 1,  Name = "Spaliny",                    Descript = "Temperatura spalin",        Address = "280B2328000080B0"},
                    new Sensors{Id = 9, DataBus = 2,  SensorType = 1, SensorKind = 1,  Name = "Sypialnia balkon",           Descript = "Sypialnia balkon",          Address = "28FF1BFB54160463"},
                    new Sensors{Id = 10, DataBus = 2, SensorType = 1, SensorKind = 1,  Name = "Sypialnia TV",               Descript = "Sypialnia TV",              Address = "28FF18FF54160423"},
                    new Sensors{Id = 11, DataBus = 2, SensorType = 1, SensorKind = 1,  Name = "Strych",                     Descript = "Strych",                    Address = "28FFCB3257160473"},
                    new Sensors{Id = 12, DataBus = 2, SensorType = 1, SensorKind = 1,  Name = "Patryk",                     Descript = "Patryk",                    Address = "28FF64DB5616045E"},
                    new Sensors{Id = 13, DataBus = 2, SensorType = 1, SensorKind = 1,  Name = "Julia",                      Descript = "Julia",                     Address = "28FF6B4157160460"}

                };


                db.InsertOrReplaceAll(dataBusType);
                db.InsertOrReplaceAll(dataSensorKind);
                db.InsertOrReplaceAll(dataBus);
                db.InsertOrReplaceAll(sensorTypes);
                db.InsertOrReplaceAll(sensors);

            }
        }

        private static async Task<bool> CheckDbAsync()
        {
            bool dbExist = true;
            try
            {
                StorageFile sf = await ApplicationData.Current.LocalFolder.GetFileAsync(DbName);
            }
            catch (Exception)
            {

                dbExist = false;
            }
            return dbExist;
        }
    }
}
