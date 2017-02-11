using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace wola.ha.common.Model
{
    public class HttpResponseException : Exception
    {
        public HttpContent Content { get; set; }
        public HttpStatusCode StatusCode { get; set; }
        public HttpRequestMessage RequestMessage { get; set; }

        public HttpResponseException(HttpStatusCode statusCode)
        {
            StatusCode = statusCode;
        }
        //    private void ThrowResponseException(HttpStatusCode statusCode, string message)
        //    {
        //        throw new HttpResponseException() { StatusCode = statusCode, RequestMessage = message };

        //    }
    }
}
