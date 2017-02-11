
using Serilog;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace wola.ha.common.Helper
{
    public static class LoggerFactory
    {

        public static void InitialLogger()
        {
            Serilog.Log.Logger = new LoggerConfiguration()
                .WriteTo.RollingFile(@"logi\log-{Date}.txt")
                .CreateLogger();
        }

        public static void InitialLoggerByServer(string path)
        {
            Serilog.Log.Logger = new LoggerConfiguration()
                .WriteTo.RollingFile(path + @"\logi\log-{Date}.txt")
                .CreateLogger();
        }

        public static void InitialLoggerByServer()
        {
            Serilog.Log.Logger = new LoggerConfiguration()
                .WriteTo.RollingFile(@"logi\log-{Date}.txt")
                .CreateLogger();
        }


        public static string LogException(Exception ex, string extrainfo = "", dynamic paraminfo = null)
        {
            var msgExp = new List<string>();
            var prepareStringException = string.Empty;

            var info = "Processed {@SensorInput} ";


            if (paraminfo != null)
            {
                var sb = new StringBuilder();
                foreach (PropertyInfo pi in paraminfo.GetType().GetProperties())
                {
                    sb.AppendFormat("{0}: {1} ", pi.Name, pi.GetValue(paraminfo, null));
                }

                info += sb.ToString();
            }

            Serilog.Log.Information(info, extrainfo);

            if (ex.InnerException != null)
            {
                var exception = ex.InnerException;

                while (exception != null)
                {
                    msgExp.Add(exception.Message);
                    exception = exception.InnerException;
                }

                if (msgExp.Count > 0)
                {
                    foreach (var item in msgExp)
                    {
                        Serilog.Log.Error("inerException: " + item);

                        if (prepareStringException.Length > 0)
                        {
                            prepareStringException += Environment.NewLine;
                        }
                        prepareStringException += item;
                    }
                }
            }
            else
            {
                prepareStringException = ex.Message;
                Serilog.Log.Error(ex.Message);
            }

            return prepareStringException;
        }

        public static string LogExceptionStack(Exception ex, string extrainfo = "", dynamic paraminfo = null)
        {
            var msgExp = new List<string>();
            var prepareStringException = string.Empty;

            var info = "Processed {@SensorInput} ";


            if (paraminfo != null)
            {
                var sb = new StringBuilder();
                foreach (PropertyInfo pi in paraminfo.GetType().GetProperties())
                {
                    sb.AppendFormat("{0}: {1} ", pi.Name, pi.GetValue(paraminfo, null));
                }

                info += sb.ToString();
            }

            Serilog.Log.Information(info, extrainfo);

            if (ex.InnerException != null)
            {
                var exception = ex.InnerException;

                while (exception != null)
                {
                    msgExp.Add(exception.Message);
                    exception = exception.InnerException;
                }

                if (msgExp.Count > 0)
                {
                    foreach (var item in msgExp)
                    {
                        if (prepareStringException.Length > 0)
                        {
                            prepareStringException += Environment.NewLine;
                        }
                        prepareStringException += item;
                    }
                }
            }
            else
            {
                prepareStringException = ex.Message;
            }

            Serilog.Log.Error(ex, ex.Message);
            return prepareStringException;
        }

        public static void LogInfo(string info, string extrainfo = "", dynamic paraminfo = null)
        {
            Serilog.Log.Information(info, extrainfo);
        }


    }

}
