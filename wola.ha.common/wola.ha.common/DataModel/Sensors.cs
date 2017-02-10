using System.ComponentModel.DataAnnotations.Schema;
using SQLite.Net.Attributes;
using SQLite.Net;

namespace wola.ha.common.DataModel
{
    public partial class Sensors
    {
        [PrimaryKey, NotNull]
        public int Id { get; set; }

        public int DataBus { get; set; } //  REFERENCES DataBus (Id)

        [NotNull]
        public int SensorType { get; set; } //  REFERENCES SensorType (Id)

        [NotNull]
        public int SensorKind { get; set; } //  REFERENCES SensorKind (Id)

        [MaxLength(100), NotNull]
        public string Name { get; set; }

        [MaxLength(100)]
        public string Descript { get; set; }

        [MaxLength(10)]
        public string Pin { get; set; }

        [MaxLength(16)]
        public string Address { get; set; }


        [Ignore ]
        public DataBus DataBusEx { get; set; }
        [Ignore]
        public SensorType SensorTypeEx { get; set; }
        [Ignore]
        public SensorKind SensorKindEx { get; set; }

        public void GetEx()
        {
            try
            {
                using (SQLiteConnection db = LocalDb.DbConnection)
                {
                    DataBusEx = db.Table<DataBus>().Where(w => w.Id == this.DataBus).FirstOrDefault();
                    SensorKindEx = db.Table<SensorKind>().Where(w => w.Id == SensorKind).FirstOrDefault();
                    SensorTypeEx = db.Table<SensorType>().Where(w => w.Id == SensorType).FirstOrDefault();
                }
            }
            catch (System.Exception)
            {

                throw;
            }


        }


    }

}