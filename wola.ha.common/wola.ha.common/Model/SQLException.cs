using System;

namespace wola.ha.common.Model
{
    internal class SQLException : Exception
    {
        private string v;
        private string message;

        public SQLException()
        {
        }

        public SQLException(string message) : base(message)
        {
        }

        public SQLException(string v, string message)
        {
            this.v = v;
            this.message = message;
        }

        public SQLException(string message, Exception innerException) : base(message, innerException)
        {
        }
    }
}