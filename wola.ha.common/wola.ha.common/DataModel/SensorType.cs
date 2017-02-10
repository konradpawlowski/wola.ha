using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class SensorType
    {
        [PrimaryKey]
        public int Id { get; set; }

        [MaxLength(10), NotNull, Indexed]
        public string Type { get; set; }

        [MaxLength(100)]
        public string Descript { get; set; }

    }
}