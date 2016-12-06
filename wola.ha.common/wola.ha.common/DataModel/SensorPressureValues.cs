using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class SensorPressureValues
    {
        [
           PrimaryKey,
           NotNull,
           Default(true, "CURRENT_TIMESTAMP"),
           Indexed("idx_SensorPressureValues_Date", 1),
           Unique
       ]
        public DateTime Date { get; set; }
        [NotNull,
         ForeignKey("FK_SensorPressureValues2Sensors")]
        public int SensorId { get; set; }

        [Default(true, 0)]
        public decimal Value { get; set; }
    }
}
