using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;
using ECP.Db.Entity;
using ECP.Global;

namespace ECP.Util
{
    [Serializable]
    public class DBConnectionValue : ECPObject 
    {
        public DBConnectionValue()
        {
        }

        public string getGlobalDBConnectionString()
        {
            ConnectionStringSettings setting = null;

            setting = ConfigurationManager.ConnectionStrings["GlobalDataBaseConnectionString"];

            return setting.ConnectionString;
        }
    }
}
