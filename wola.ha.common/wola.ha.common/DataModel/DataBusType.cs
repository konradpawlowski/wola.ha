using SQLite.Net.Attributes;

namespace wola.ha.common.DataModel
{
    public class DataBusType
    {
        [PrimaryKey, NotNull, Indexed("idx_DataBusType_Id",1),Unique]
        public int Id { get; set; }
        [NotNull,MaxLength(10)]
        public string Type { get; set; }
        [MaxLength(50), NotNull]
        public string Name { get; set; }

        [MaxLength(100), NotNull]
        public string Descript { get; set; }

        [MaxLength(10)]
        public string Address { get; set; }


    }
}