using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class UserHistData : ECPObject 
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }

        private string userid = "";

        public string Userid
        {
            get { return userid; }
            set { userid = value; }
        }
        private string password = "";

        public string Password
        {
            get { return password; }
            set { password = value; }
        }
        private string username = "";

        public string Username
        {
            get { return username; }
            set { username = value; }
        }
        private string passwordchangedtime = "";

        public string Passwordchangedtime
        {
            get { return passwordchangedtime; }
            set { passwordchangedtime = value; }
        }
        private string passwordexpiredtime = "";

        public string Passwordexpiredtime
        {
            get { return passwordexpiredtime; }
            set { passwordexpiredtime = value; }
        }
        private string createtime = "";

        public string Createtime
        {
            get { return createtime; }
            set { createtime = value; }
        }
        private string createman = "";

        public string Createman
        {
            get { return createman; }
            set { createman = value; }
        }
        private string disabled = "";

        public string Disabled
        {
            get { return disabled; }
            set { disabled = value; }
        }

        private string dbConnectionString = "";
        public UserHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertUserHistData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter para = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                strSql = getInsertSQL();

                theDB.Start();
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                para.SetVarChar("USERID", Helper.PreventNull(this.Userid));
                para.SetVarChar("PASSWORD", Helper.PreventNull(this.Password));
                para.SetVarChar("USERNAME", Helper.PreventNull(this.Username));
                para.SetVarChar("PASSWORDCHANGEDTIME", Helper.PreventNull(this.Passwordchangedtime));
                para.SetVarChar("PASSWORDEXPIREDTIME", Helper.PreventNull(this.Passwordexpiredtime));
                para.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                para.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                para.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
           
                theDB.ExecuteUpdate(strSql, para);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void insertUserHistData(DBConnection theDB)
        {
            PreparedSqlParameter para = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                para.SetVarChar("USERID", Helper.PreventNull(this.Userid));
                para.SetVarChar("PASSWORD", Helper.PreventNull(this.Password));
                para.SetVarChar("USERNAME", Helper.PreventNull(this.Username));
                para.SetVarChar("PASSWORDCHANGEDTIME", Helper.PreventNull(this.Passwordchangedtime));
                para.SetVarChar("PASSWORDEXPIREDTIME", Helper.PreventNull(this.Passwordexpiredtime));
                para.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                para.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                para.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                theDB.ExecuteUpdate(strSql, para);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setUserHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Userid = Helper.PreventNull(list[0]);
            this.Password = Helper.PreventNull(list[1]);
            this.Username = Helper.PreventNull(list[2]);
            this.Passwordchangedtime = Helper.PreventNull(list[3]);
            this.Passwordexpiredtime = Helper.PreventNull(list[4]);
            this.Createtime = Helper.PreventNull(list[5]);
            this.Createman = Helper.PreventNull(list[6]);
            this.Disabled = Helper.PreventNull(list[7]);
        }

        public UserData setUserData(string[] list)
        {
            UserData data = new UserData();

            data.Userid = Helper.PreventNull(list[0]);
            data.Password = Helper.PreventNull(list[1]);
            data.Username = Helper.PreventNull(list[2]);
            data.Passwordchangedtime = Helper.PreventNull(list[3]);
            data.Passwordexpiredtime = Helper.PreventNull(list[4]);
            data.Createtime = Helper.PreventNull(list[5]);
            data.Createman = Helper.PreventNull(list[6]);
            data.Disabled = Helper.PreventNull(list[7]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql = "" +
                    "  INSERT INTO tb_userhist  " +
                    "            ( backuptime             " +
                    "             ,userid                 " +
                    "             ,password               " +
                    "             ,username               " +
                    "             ,passwordchangedtime    " +
                    "             ,passwordexpiredtime    " +
                    "             ,createtime             " +
                    "             ,createman              " +
                    "             ,disabled               " +
                    "            )                        " +
                    "       VALUES                        " +
                    "            ( @BACKUPTIME            " +
                    "             ,@USERID                " +
                    "             ,@PASSWORD              " +
                    "             ,@USERNAME              " +
                    "             ,@PASSWORDCHANGEDTIME   " +
                    "             ,@PASSWORDEXPIREDTIME   " +
                    "             ,@CREATETIME            " +
                    "             ,@CREATEMAN             " +
                    "             ,@DISABLED              " +
                    "            )                        ";

            return strSql;
        }
    }
}
