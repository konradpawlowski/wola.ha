using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace wola.ha.common.DataModel
{
    public class TableBase
    {
        /// <summary>
        /// Lock for Table data. 
        /// </summary>
        protected SemaphoreSlim Lock { get; private set; } = new SemaphoreSlim(1, 1);
    }
}