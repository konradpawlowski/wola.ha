using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using wola.ha.common.Model;

namespace wola.ha.common.Helper
{

    public static  class WolaClient 
    {
        //public WolaClient():base();
        //public WolaClient(string endpoint):base(endpoint);
        //public WolaClient(string endpoint, ClaimsPrincipal principal);
        //public WolaClient(string endpoint, string autorizationtoken);
        public static async Task<IEnumerable<T>> GetEnumerableFromController<T>(string controllerName, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName, user);
                var retList = await client.GetList<IEnumerable<T>>();

                return retList;
            }
            catch (Exception)
            {

                throw;
            }

        }
        public static async Task<List<T>> GetListFromController<T>(string controllerName, ClaimsPrincipal user)
        {
            var client = new BaseApiClient(controllerName, user);
            var retList = await client.GetList<List<T>>();

            return retList;
        }


        public static async Task<List<T>> GetListFromController<T>(string controllerName, string controlerAction, ClaimsPrincipal user)
        {
            var client = new BaseApiClient(controllerName + "/" + controlerAction + "/", user);
            var retList = await client.GetList<List<T>>();

            return retList;
        }
        public static async Task<IQueryable<T>> GetQuerableFromController<T>( string controllerName, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName, user);
                var retList = await client.GetList<IQueryable<T>>();

                return retList;
            }
            catch (Exception)
            {

                throw;
            }

        }

        public static async Task<T> GetItemFromController<T>(string controllerName, int id, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/", user);
                var ret = await client.Get<T>(id);

                return ret;
            }
            catch (Exception)
            {

                throw;
            }

        }



        public static async Task<T> PostItemToController<T>( string controllerName, T item, ClaimsPrincipal user = null)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/", user);
                var ret = await client.Post(item);

                return ret;
            }
            catch (Exception)
            {
                throw;
            }

        }
        public static async Task<String> PutItemToController<T>(string controllerName, int id, T item, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/", user);
                var ret = await client.Put(id, item);

                return ret;
            }
            catch (Exception)
            {
                throw;
            }
        }


      
        public static async Task<string> DeleteItemFromController<T>(string controllerName, int id, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/", user);
                var ret = await client.Delete(id);
                return ret;
            }
            catch (Exception)
            {

                throw;
            }


        }


        public static async Task<IEnumerable<T>> GetFilteredEnumerableFromControllerAction<T>( string controllerName, string controlerAction, int id, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/" + controlerAction + "/", user);
                var retList = await client.Get<IEnumerable<T>>(id);

                return retList;
            }
            catch (Exception)
            {
                throw;
            }

        }

        public static async Task<List<T>> GetFilteredListFromControllerAction<T>( string controllerName, string controlerAction, int id, int id2, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/" + controlerAction + "/" + id.ToString() + "/" + id2.ToString(), user);
                var retList = await client.GetList<List<T>>();

                return retList;
            }
            catch (Exception)
            {
                throw;
            }

        }


        public static async Task<List<T>> GetFilteredListFromControllerAction<T>( string controllerName, string controlerAction, int id, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/" + controlerAction + "/" + id.ToString(), user);
                var retList = await client.GetList<List<T>>();

                return retList;
            }
            catch (Exception)
            {
                throw;
            }

        }

        public static async Task<List<T>> GetFilteredListFromControllerAction<T>(string controllerName, int id, ClaimsPrincipal user)
        {
            try
            {
                var client = new BaseApiClient(controllerName + "/" + id.ToString(), user);
                var retList = await client.GetList<List<T>>();

                return retList;
            }
            catch (Exception)
            {
                throw;
            }

        }
    }
}
