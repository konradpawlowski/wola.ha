using System.Diagnostics;
using SQLite.Net;

namespace wola.ha.common.DataModel
{
    /// <summary> 
    /// Writes SQLite.NET trace to the Debug window. 
    /// </summary> 
    public class DebugTraceListener : ITraceListener
    {
        public void Receive(string message)
        {
            Debug.WriteLine(message);
        }
    }
}