using SQLite.Net;
using SQLite.Net.Async;
using SQLite.Net.Platform.WinRT;
using System;
using System.Threading.Tasks;
using Windows.Storage;

namespace wola.ha.common.DataModel
{
    public abstract class DataContext : IDisposable
    {
        public StorageFolder Folder { get; private set; }
        public abstract int Version { get; }
        public abstract Task<int> LastVersion();

        public abstract string DatabaseVersionKey { get; }
        public abstract string DatabaseFile { get; }
        private string _DatabaseFullPathFile = "";


        protected bool IsDisposed { get; private set; }

        protected SQLiteAsyncConnection connection = null;
        public SQLiteAsyncConnection Connection { get { return connection; } }
        protected SQLiteConnectionWithLock connectionWithLock = null;

        protected SQLiteConnectionWithLock GetSQLiteConnectionWithLock()
        {
            if (connectionWithLock == null)
                connectionWithLock = new SQLiteConnectionWithLock(new SQLitePlatformWinRT(), new SQLiteConnectionString(_DatabaseFullPathFile, storeDateTimeAsTicks: false));

            return connectionWithLock;
        }

        public async Task Init(StorageFolder folder)
        {
            this.Folder = folder;
            this.IsDisposed = false;

            _DatabaseFullPathFile = await GetDatabaseFile(DatabaseFile);
            await GetConnection();
        }

        protected async Task<string> GetDatabaseFile(string file)
        {
            if (string.IsNullOrEmpty(DatabaseFile))
                throw new InvalidOperationException("DatabaseFile can not be null.");
            if (Folder == null)
                throw new InvalidOperationException("Folder can not be null.");

            StorageFile storageFile = await Folder.CreateFileAsync(DatabaseFile, CreationCollisionOption.OpenIfExists);
            return storageFile.Path;
        }

        protected async Task<SQLiteAsyncConnection> GetConnection()
        {
            if (connection == null && !IsDisposed)
            {
                Log.w("DB OPEN Connection {0}", System.IO.Path.GetFileName(DatabaseFile));

                connection = new SQLiteAsyncConnection(GetSQLiteConnectionWithLock);

                int lastVersion = await LastVersion(); // cause a dbselect

                if (lastVersion == -1)
                {
                    // CreateTabels
                    await CreateTables(connection);
                }
                else
                {
                    if (lastVersion < Version)
                    {
                        await UpgradeTables(connection);
                    }
                }
            }

            return connection;
        }

        public abstract Task Reset();

        protected abstract Task UpgradeTables(SQLiteAsyncConnection c);

        protected abstract Task CreateTables(SQLiteAsyncConnection c);

        public virtual void Dispose()
        {
            IsDisposed = true;
            if (connection != null)
            {
                connection = null;
            }
            if (connectionWithLock != null)
            {
                connectionWithLock.Dispose();
                connectionWithLock = null;
            }
        }
    }
}