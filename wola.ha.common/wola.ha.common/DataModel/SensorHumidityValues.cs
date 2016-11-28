using System;
using System.ComponentModel.DataAnnotations.Schema;
using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class SensorHumidityValues
    {
        [
            PrimaryKey,
            NotNull,
            Default(true, "CURRENT_TIMESTAMP"),
            Indexed("idx_SensorHumidityValues_Date", 1),
            Unique
        ]
        public DateTime Date { get; set; }
        [NotNull,
         ForeignKey("FK_SensorHumidityValues2Sensors")]
        public int SensorId { get; set; }

        [Default(true, 0)]
        public decimal Value { get; set; }
    }

    class SensorHumidityValuesImpl : SensorHumidityValues
    {
    }
}