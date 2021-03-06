﻿using SQLite.Net.Attributes;
namespace wola.ha.common.DataModel
{
    public class DataBusType
    {
        [PrimaryKey, NotNull, Indexed("idx_DataBusType_Id", 1), Unique]
        public int Id { get; set; }

        [NotNull, MaxLength(20)] 
        public string Type { get; set; }
    }
}
