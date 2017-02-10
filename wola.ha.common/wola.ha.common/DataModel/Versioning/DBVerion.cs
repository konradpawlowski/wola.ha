using SQLite.Net;
using SQLite.Net.Async;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wola.ha.common.Model;

namespace wola.ha.common.DataModel.Versioning
{
    public class DBVersionsContext : DataContext
    {
        #region SINGLETON
        private static DBVersionsContext _Instance;
        public static DBVersionsContext Instance
        {
            get
            {
                if (_Instance == null)
                    _Instance = new DBVersionsContext();

                return _Instance;
            }
        }
        #endregion

        public AsyncTableQuery<Version> TVersion { get { return connection.Table<Version>(); } }

        public override int Version
        {
            get { return 0; }
        }
        public override Task<int> LastVersion()
        {
            return Get(DatabaseVersionKey);
        }
        public override string DatabaseVersionKey
        {
            get { return DatabaseFile; }
        }
        public override string DatabaseFile
        {
            get { return "dbversions.db"; }
        }

        public DBVersionsContext()
        {

        }

        public override Task Reset()
        {
            throw new NotImplementedException();
        }
        protected override Task UpgradeTables(SQLiteAsyncConnection c)
        {
            throw new NotImplementedException();
        }
        protected override async Task CreateTables(SQLiteAsyncConnection c)
        {
            try
            {
                await c.CreateTableAsync<Version>();

                await this.Set(DatabaseVersionKey, Version);
            }
            catch (SQLiteException ex)
            {
                throw new SQLException("DBVersionsContext CreateTables failed.", ex.Message);
            }
        }

        public async Task<int> Get(string name)
        {
            int version = -1;

            try
            {
                var query = TVersion.Where(A => A.Name == name);
                var result = await query.ToListAsync();

                if (result.Count > 0)
                    version = result[0].Number;
            }
            catch (SQLiteException ex)
            {
                // No such table wird ignoriert da die Versionstabelle ebenfalls ihre
                // Version in sich selbst speichert. Beim ersten Start also durchaus vertretbar. 
                if (ex.HResult != -2146233088)
                    throw ex;
            }

            return version;
        }
        public async Task Set(string name, int version)
        {
            if (connection != null)
            {
                bool isInsert = await Get(name) == -1;

                if (isInsert)
                {
                    await connection.InsertAsync(new Version() { Name = name, Number = version });
                }
                else
                {
                    await connection.UpdateAsync(new Version() { Name = name, Number = version });
                }
            }
        }
        public async Task Del(string name)
        {
            if (connection != null)
            {
                await connection.DeleteAsync(new Version() { Name = name });
            }
        }

    }
}