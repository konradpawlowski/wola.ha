
using SQLite.Net.Attributes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace wola.ha.common.DataModel.Versioning
{
    [Table("Version")]
    public class Version
    {
        [PrimaryKey, MaxLength(512)]
        public string Name { get; set; }
        [NotNull]
        public int Number { get; set; }
    }
}