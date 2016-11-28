using System;
using System.ComponentModel.DataAnnotations.Schema;
using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class SensorTemperatureValues
    {
        [
            PrimaryKey,
            NotNull,
            Default(true, "CURRENT_TIMESTAMP"), 
            Indexed("idx_SensorTemperatureValue_Date", 1),
            Unique
        ]
        public DateTime Date { get; set; }
        [NotNull,
         ForeignKey("FK_SensorTemperatureValue2Sensors")]
        public int SensorId { get; set; }

        [Default(true,0)]
        public decimal Value { get; set; }
    }
}