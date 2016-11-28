using System.ComponentModel.DataAnnotations.Schema;
using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class Sensors
    {
        [PrimaryKey]
        public int Id { get; set; }
        

        [ForeignKey("FK_DataBusType2Sensors"), NotNull]
        public int DataBusTypeId { get; set; }
        [ForeignKey("FK_SensorType2Sensors"),MaxLength(10), NotNull]
        public string SensorType  { get; set; }
        [MaxLength(100), NotNull]
        public string Name { get; set; }
        [MaxLength(100)]
        public string Descript { get; set; }

        [MaxLength(10)]
        public string Pin { get; set; }
        [MaxLength(16)]
        public string Address { get; set; }
    }
}