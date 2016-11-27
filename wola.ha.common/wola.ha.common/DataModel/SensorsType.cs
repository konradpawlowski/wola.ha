using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class SensorType
    {
        [PrimaryKey, MaxLength(10), NotNull, Indexed ]
        public string Type { get; set; }
        [MaxLength(100)]
        public string Descript { get; set; }


        public void FillData()
        {
            
        }
    }

    public class Sensors
    {
        [PrimaryKey]
        public int Id { get; set; }
        [ForeignKey("FK_SensorType2Sensors")]
        public int SensorType { get; set; }


    }
}
