using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace wola.ha.common.Model
{
    public class ResponseModelStatus<T>
    {
        private List<ResponseModelMessage> _message;
        public HttpStatusCode HtttpStatus { set; get; }
        public ResponseMainModelStatusEnum ResponseStatus { set; get; }

        public List<ResponseModelMessage> Message
        {
            get { return _message ?? (_message = new List<ResponseModelMessage>()); }
            set
            {
                _message = value;
            }
        }

        public T Model { set; get; }

        //public void CompletMessageFromModalState(ModelStateDictionary modelState)
        //{
        //    var modalStateList = modelState.Where(q => q.Value.Errors.Count > 0).ToList();

        //    if (modalStateList.Any())
        //    {
        //        ResponseStatus = ResponseMainModelStatusEnum.Error;
        //    }

        //    foreach (var item in modalStateList)
        //    {
        //        foreach (var erroritem in item.Value.Errors)
        //        {
        //            Message.Add(new ResponseModelMessage
        //            {
        //                Key = item.Key,
        //                Message = erroritem.ErrorMessage,
        //                ResponseStatus = ResponseModelStatusEnum.Error
        //            });
        //        }
        //    }
        //}
    }
    public class ResponseModelMessage
    {
        public ResponseModelStatusEnum ResponseStatus { set; get; }
        public string Key { set; get; }
        public string Message { set; get; }
    }
    public enum ResponseModelStatusEnum
    {
        Error,
        Warning,
        Info
    };
    public enum ResponseMainModelStatusEnum
    {
        Error,
        Warning,
        Ok,
        Onlyhttpstatus
    };
   
}
