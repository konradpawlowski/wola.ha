using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class SensorKind
    {
        [PrimaryKey, NotNull, Indexed("idx_DataBusType_Id", 1), Unique]
        public int Id { get; set; }

        [NotNull, MaxLength(20), Unique]
        public string Kind { get; set; }
    }
}
