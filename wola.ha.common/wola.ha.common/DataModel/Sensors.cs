using System.ComponentModel.DataAnnotations.Schema;
using SQLite.Net.Attributes;
using SQLite.Net;
using wola.ha.common.Model;
using System.Threading.Tasks;

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

        public async Task GetEx()
        {
            try
            {
              
                    DataBusEx =  await Context.Instance.Connection.Table<DataBus>().Where(w => w.Id == DataBus).FirstOrDefaultAsync();
                    SensorKindEx = await Context.Instance.Connection.Table<SensorKind>().Where(w => w.Id == SensorKind).FirstOrDefaultAsync();
                    SensorTypeEx = await Context.Instance.Connection.Table<SensorType>().Where(w => w.Id == SensorType).FirstOrDefaultAsync();
              
            }
            catch (System.Exception)
            {

                throw;
            }


        }


    }

}