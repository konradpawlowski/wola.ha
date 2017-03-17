using System;

namespace RestUpServerController.Logger
{
    public interface ILogFactory : IDisposable
    {
        ILogger GetLogger<T>();
        ILogger GetLogger(string name);
    }
}
