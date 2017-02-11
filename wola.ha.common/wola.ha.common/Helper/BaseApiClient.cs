using System.Collections.Generic;
using System.Net.Http;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using System.Threading.Tasks;
using System.Security.Claims;
using Newtonsoft.Json.Linq;
using System;
using System.Text;
using wola.ha.common.Model;

namespace wola.ha.common.Helper
{
    public class BaseApiClient
    {
        protected string Endpoint; // = ConstantsAgeUrls.WebApiAddress;
        private ClaimsPrincipal _principal;
        private string _token = string.Empty;

        public BaseApiClient(string endpoint, ClaimsPrincipal principal)
        {
            Endpoint = endpoint;
            _principal = principal;
        }

        public BaseApiClient(string endpoint, string autorizationtoken)
        {
            Endpoint = endpoint;
            _token = autorizationtoken;
        }

        public BaseApiClient(string endpoint)
        {
            Endpoint = endpoint;
        }

        public BaseApiClient()
        {
            Endpoint += "test";
        }


        public async Task<bool> Test()
        {
            try
            {
                var testWsrv = await this.Get<bool>();

                if (!testWsrv)
                {
                   LoggerFactory.LogInfo("endpoint wsrv TEST FALSE: " + Endpoint);
                }
                return testWsrv;
            }
            catch (Exception e)
            {
                LoggerFactory.LogExceptionStack(e);//,    System.Reflection.MethodBase. .GetCurrentMethod().DeclaringType.FullName);
                LoggerFactory.LogException(e); ;//,       System.Reflection.MethodBase.GetCurrentMethod().DeclaringType.FullName);

                return false;
            }
        }


        public async Task<T> Get<T>(int top = 0, int skip = 0)
        {
            using (var httpClient = NewHttpClient())
            {
                try
                {
                    var endpoint = Endpoint + "?";

                    var parameters = new List<string>();

                    if (top > 0)
                        parameters.Add(string.Concat("$top=", top));

                    if (skip > 0)
                        parameters.Add(string.Concat("$skip=", skip));

                    endpoint += string.Join("&", parameters);

                    var response = await httpClient.GetAsync(endpoint); //.Result;
                    //if (response.IsSuccessStatusCode)
                    //{
                    //    return await response.Content.ReadAsAsync<T>();
                    //}

                    return JsonConvert.DeserializeObject<T>(response.Content.ReadAsStringAsync().Result);
                }
                catch (Exception e)
                {
                    if (e.InnerException != null)
                    {
                        throw e.InnerException;
                    }
                    else
                    {
                        throw e;
                    }
                }
            }
        }

        public async Task<T> Get<T>(int id)
        {
            using (var httpClient = NewHttpClient())
            {
                try
                {
                    var response = await httpClient.GetAsync(Endpoint + id); //.Result;

                    if (response.StatusCode != System.Net.HttpStatusCode.OK)
                    {

                        ThrowIfStatusNotOk(response.StatusCode, response.Content.ReadAsStringAsync().Result);
                        //, response.Content.ReadAsStringAsync().Result


                        //throwIfStatusNotOk(response.StatusCode);

                        //var errret = response.Content.ReadAsStringAsync().Result;
                        //if (errret != null)
                        //{
                        //    var errormsg = JsonConvert.DeserializeObject<JObject>(errret);
                        //    throw new System.Exception(errormsg["message"].ToString());
                        //}
                        //else
                        //{
                        //    throw new HttpResponseException(response.StatusCode);
                        //}
                    }

                    return JsonConvert.DeserializeObject<T>(response.Content.ReadAsStringAsync().Result);

                }
                catch (System.Exception e)
                {
                    if (e.InnerException != null)
                    {
                        throw e.InnerException;
                    }
                    else
                    {
                        throw e;
                    }
                }
            }
        }

        public async Task<T> Get<T>(string id)
        {
            using (var httpClient = NewHttpClient())
            {
                try
                {
                    var response = await httpClient.GetAsync(Endpoint + id); //.Result;

                    if (response.StatusCode != System.Net.HttpStatusCode.OK)
                    {

                        ThrowIfStatusNotOk(response.StatusCode, response.Content.ReadAsStringAsync().Result);
                        //, response.Content.ReadAsStringAsync().Result


                        //throwIfStatusNotOk(response.StatusCode);

                        //throw new HttpResponseException(response.StatusCode);
                    }

                    return JsonConvert.DeserializeObject<T>(response.Content.ReadAsStringAsync().Result);
                }
                catch (System.Exception e)
                {
                    if (e.InnerException != null)
                    {
                        throw e.InnerException;
                    }
                    else
                    {
                        throw e;
                    }
                }

            }
        }

        public async Task<T> GetList<T>()
        {
            using (var httpClient = NewHttpClient())
            {
                try
                {
                    var response = await httpClient.GetAsync(Endpoint); //.Result;

                    if (response.StatusCode != System.Net.HttpStatusCode.OK)
                    {

                        ThrowIfStatusNotOk(response.StatusCode, response.Content.ReadAsStringAsync().Result);
                        //, response.Content.ReadAsStringAsync().Result

                        //var errret = response.Content.ReadAsStringAsync().Result;
                        //if (!string.IsNullOrEmpty(errret))
                        //{
                        //    var errormsg = JsonConvert.DeserializeObject<JObject>(errret);

                        //    var exmsg = errormsg["exceptionMessage"];
                        //    if (exmsg != null)
                        //    {
                        //        throw new System.Exception(exmsg.ToString() + " :" + _endpoint);
                        //    }

                        //    throw new System.Exception(errormsg["message"].ToString() + " :" + _endpoint);
                        //}
                        //else
                        //{
                        //    throw new HttpResponseException(response.StatusCode);
                        //}
                    }

                    return JsonConvert.DeserializeObject<T>(response.Content.ReadAsStringAsync().Result);
                }
                catch (System.Exception e)
                {
                    if (e.InnerException != null)
                    {
                        throw e.InnerException;
                    }
                    else
                    {
                        throw e;
                    }
                }
            }
        }

        public async Task<T> GetTop<T>(int top = 0, int skip = 0)
        {
            using (var httpClient = new HttpClient())
            {
                var endpoint = Endpoint + "?";
                var parameters = new List<string>();

                if (top > 0)
                    parameters.Add(string.Concat("$top=", top));

                if (skip > 0)
                    parameters.Add(string.Concat("$skip=", skip));

                endpoint += string.Join("&", parameters);

                var response = await httpClient.GetAsync(endpoint); //.Result;
                //if (response.IsSuccessStatusCode)
                //{
                //    return await response.Content.ReadAsAsync<T>();
                //}

                return JsonConvert.DeserializeObject<T>(response.Content.ReadAsStringAsync().Result);
            }
        }


        public async Task<T> Post<T>(T data)
        {
            using (var httpClient = NewHttpClient())
            {
                HttpContent requestMessage = GetHttpRequestMessage<T>(data);
                HttpResponseMessage response = await httpClient.PostAsync(Endpoint, requestMessage);


                //HttpResponseMessage response = await httpClient.PostAsJsonAsync<T>(Endpoint, data);
                if (response.StatusCode != System.Net.HttpStatusCode.Created &&
                    response.StatusCode != System.Net.HttpStatusCode.OK)
                {
                    throw new HttpResponseException(response.StatusCode);
                }

                return JsonConvert.DeserializeObject<T>(response.Content.ReadAsStringAsync().Result);
            }
        }

        private StringContent GetHttpRequestMessage<T>(T data)
        {
            return new StringContent(JsonConvert.SerializeObject(data), Encoding.UTF8, "application/json");
        }

        public async Task<string> Put<T>(int id, T data)
        {
            using (var httpClient = NewHttpClient())
            {
                HttpContent requestMessage = GetHttpRequestMessage<T>(data);
                HttpResponseMessage response = await httpClient.PutAsync(Endpoint + id.ToString(), requestMessage);
                //return result.Content.ReadAsStringAsync().Result;

               
                if (response.StatusCode != System.Net.HttpStatusCode.NoContent)
                {
                    throw new HttpResponseException(response.StatusCode);
                }
                return response.Content.ToString();
            }
        }

        public async Task<string> Put<T>(string id, T data)
        {
            using (var httpClient = NewHttpClient())
            {
                HttpContent requestMessage = GetHttpRequestMessage<T>(data);
                HttpResponseMessage response = await httpClient.PutAsync(Endpoint + id, requestMessage);
                //return result.Content.ReadAsStringAsync().Result;

               // HttpResponseMessage response = await httpClient.PutAsJsonAsync<T>(Endpoint + id, data);
                if (response.StatusCode != System.Net.HttpStatusCode.NoContent)
                {
                    throw new HttpResponseException(response.StatusCode);
                }
                return response.Content.ToString();

            }
        }


        public async Task<string> PutStringContent<T>(int id, T data)
        {
            using (var httpClient = NewHttpClient())
            {
                string preservereferenacesall = Newtonsoft.Json.JsonConvert.SerializeObject(data, Newtonsoft.Json.Formatting.Indented,
                       new Newtonsoft.Json.JsonSerializerSettings
                       {
                           ReferenceLoopHandling = Newtonsoft.Json.ReferenceLoopHandling.Ignore,
                           PreserveReferencesHandling = Newtonsoft.Json.PreserveReferencesHandling.None
                       }
                    );

                var stringContent = new StringContent(preservereferenacesall, Encoding.UTF8, "application/json");
                HttpResponseMessage response = await httpClient.PutAsync(Endpoint + id, stringContent);

                if (response.StatusCode != System.Net.HttpStatusCode.NoContent)
                {
                    throw new HttpResponseException(response.StatusCode);
                }
                return response.Content.ToString();
            }
        }

        //public async Task<ResponseModelStatus<T>> PostRespModel<T>(RequestModelStatus<T> data)
        //{
        //    using (var httpClient = NewHttpClient())
        //    {
        //        try
        //        {
        //            //var requestMessage = GetHttpRequestMessage<T>(data);
        //            //var result = httpClient.PostAsync(_endpoint, requestMessage.Content).Result;
        //            //return result.Content.ToString();

        //            var response = await httpClient.PostAsJsonAsync<RequestModelStatus<T>>(Endpoint, data);
        //            if (response.StatusCode != System.Net.HttpStatusCode.Created &&
        //                response.StatusCode != System.Net.HttpStatusCode.OK)
        //            {
        //                //throw new HttpResponseException(response.StatusCode);

        //                ThrowIfStatusNotOk(response.StatusCode, response.Content.ReadAsStringAsync().Result);
        //                //, response.Content.ReadAsStringAsync().Result
        //            }

        //            return
        //                JsonConvert.DeserializeObject<ResponseModelStatus<T>>(
        //                    response.Content.ReadAsStringAsync().Result);

        //        }
        //        catch (System.Exception e)
        //        {
        //            if (e.InnerException != null)
        //            {
        //                throw e.InnerException;
        //            }
        //            else
        //            {
        //                throw e;
        //            }
        //        }
        //    }
        //}

        //public async Task<ResponseModelStatus<T>> PutRespModel<T>(int id, RequestModelStatus<T> data)
        //{
        //    using (var httpClient = NewHttpClient())
        //    {
        //        try
        //        {
        //            //var requestMessage = GetHttpRequestMessage<T>(data);
        //            //var result = httpClient.PutAsync(_endpoint + id, requestMessage.Content).Result;
        //            //return result.Content.ReadAsStringAsync().Result;

        //            var response = await httpClient.PutAsJsonAsync<RequestModelStatus<T>>(Endpoint + id, data);

        //            if (response.StatusCode != System.Net.HttpStatusCode.OK)
        //            {
        //                ThrowIfStatusNotOk(response.StatusCode, response.Content.ReadAsStringAsync().Result);
        //            }

        //            return
        //                JsonConvert.DeserializeObject<ResponseModelStatus<T>>(
        //                    response.Content.ReadAsStringAsync().Result);
        //        }
        //        catch (System.Exception e)
        //        {
        //            if (e.InnerException != null)
        //            {
        //                throw e.InnerException;
        //            }
        //            else
        //            {
        //                throw e;
        //            }
        //        }
        //    }
        //}



        public async Task<string> Delete(int id)
        {
            using (var httpClient = NewHttpClient())
            {
                try
                {
                    var response = await httpClient.DeleteAsync(Endpoint + id); //.Result;

                    if (response.StatusCode != System.Net.HttpStatusCode.OK)
                    {
                        var errret = response.Content.ReadAsStringAsync().Result;
                        if (errret != null)
                        {
                            var errormsg = JsonConvert.DeserializeObject<JObject>(errret);
                            throw new System.Exception(errormsg["message"].ToString());
                        }
                        else
                        {
                            throw new HttpResponseException(response.StatusCode);
                        }
                    }

                    var result = await response.Content.ReadAsStringAsync();

                    return result;
                }
                catch (System.Exception e)
                {
                    if (e.InnerException != null)
                    {
                        throw e.InnerException;
                    }
                    else
                    {
                        throw e;
                    }
                }
            }
        }

        //protected HttpRequestMessage GetHttpRequestMessage<T>(T data)
        //{
        //    var mediaType = new MediaTypeHeaderValue("application/json");
        //    var jsonSerializerSettings = new JsonSerializerSettings();
        //    jsonSerializerSettings.Converters.Add(new IsoDateTimeConverter());

        //    var jsonFormatter = new JsonNetFormatter(jsonSerializerSettings);

        //    var requestMessage = new HttpRequestMessage<T>(data, mediaType, new MediaTypeFormatter[] { jsonFormatter });

        //    return requestMessage;
        //}

        protected HttpClient NewHttpClient()
        {
            var htc = new HttpClient();
            htc.BaseAddress = new Uri("http://192.168.200.10:8800/api/");
            htc.DefaultRequestHeaders.Accept.Clear();
            htc.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
            htc.Timeout = TimeSpan.FromMinutes(3);
            AuthenticationHeaderValue header;

            if (_principal != null)
            {
                var accestoken = _principal.FindFirst("access_token");

                header = accestoken != null ?
                        new AuthenticationHeaderValue("Bearer", accestoken.Value) :
                        new AuthenticationHeaderValue("Bearer", _token);
            }
            else
            {
                header = new AuthenticationHeaderValue("Bearer", _token);
            }

            htc.DefaultRequestHeaders.Authorization = header;

            return htc;
        }


        private void ThrowIfStatusNotOk(System.Net.HttpStatusCode statusCode)
        {
            switch (statusCode)
            {
                case System.Net.HttpStatusCode.Forbidden:
                    throw new System.Exception("Dostęp Zabroniony! : " + Endpoint);
                case System.Net.HttpStatusCode.Unauthorized:
                    throw new System.Exception("Wygasła sesja! Zaloguj się ponownie aby ją odświerzyć.");
            }
        }


        private void ThrowIfStatusNotOk(System.Net.HttpStatusCode statusCode, string responseResult)
        {
            ThrowIfStatusNotOk(statusCode);

            if (string.IsNullOrEmpty(responseResult))
            {
                throw new HttpResponseException(statusCode);
            }

            var errormsg = JsonConvert.DeserializeObject<JObject>(responseResult);

            var exmsg = errormsg["exceptionMessage"];
            if (exmsg != null)
            {
                JToken outtt;
                if (errormsg.TryGetValue("innerException", out outtt))
                {
                    var inererror = outtt["exceptionMessage"];
                    if (inererror != null)
                    {
                        throw new Exception(inererror + " :");
                    }
                }

                throw new Exception(exmsg + " :");
            }

            throw new Exception(errormsg["message"] + " :");
        }


    }
    
}
