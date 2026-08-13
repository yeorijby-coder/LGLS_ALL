using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class UserData : ECPObject 
    {
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

        public UserData()
        {
        }

        //User List 조회
        public Dictionary<string, UserData> getUserDataList()
        {
            Dictionary<string, UserData> list = new Dictionary<string, UserData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY userid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    UserData uData = new UserData();
                    uData.setUserData(reader);

                    list.Add(uData.Userid, uData);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
            
            return list;
        }

        public void insertUserData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_User 정보  update
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));
                pss.SetVarChar("PASSWORD", Helper.PreventNull(this.Password));
                pss.SetVarChar("USERNAME", Helper.PreventNull(this.Username));
                pss.SetVarChar("PASSWORDCHANGEDTIME", Helper.PreventNull(this.Passwordchangedtime));
                pss.SetVarChar("PASSWORDEXPIREDTIME", Helper.PreventNull(this.Passwordexpiredtime));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertUserDataBackup(theDB, "Insert");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void updateUserData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_User 정보  update
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));
                pss.SetVarChar("PASSWORD", Helper.PreventNull(this.Password));
                pss.SetVarChar("USERNAME", Helper.PreventNull(this.Username));
                pss.SetVarChar("PASSWORDCHANGEDTIME", Helper.PreventNull(this.Passwordchangedtime));
                pss.SetVarChar("PASSWORDEXPIREDTIME", Helper.PreventNull(this.Passwordexpiredtime));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("USERID1", Helper.PreventNull(this.Userid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertUserDataBackup(theDB, "Update");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void deleteUserData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_User 정보  update
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertUserDataBackup(theDB, "Delete");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertUserData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_User 정보  update
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));
                pss.SetVarChar("PASSWORD", Helper.PreventNull(this.Password));
                pss.SetVarChar("USERNAME", Helper.PreventNull(this.Username));
                pss.SetVarChar("PASSWORDCHANGEDTIME", Helper.PreventNull(this.Passwordchangedtime));
                pss.SetVarChar("PASSWORDEXPIREDTIME", Helper.PreventNull(this.Passwordexpiredtime));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                //DataBackup
                this.insertUserDataBackup(theDB, "Insert");

                theDB.ExecuteUpdate(strSql, pss);

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

        public void updateUserData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_User 정보  update
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));                   
                pss.SetVarChar("PASSWORD", Helper.PreventNull(this.Password));                 
                pss.SetVarChar("USERNAME", Helper.PreventNull(this.Username));                 
                pss.SetVarChar("PASSWORDCHANGEDTIME", Helper.PreventNull(this.Passwordchangedtime));       
                pss.SetVarChar("PASSWORDEXPIREDTIME", Helper.PreventNull(this.Passwordexpiredtime));     
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));               
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));                
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("USERID1", Helper.PreventNull(this.Userid));                   
                
                //DataBackup
                this.insertUserDataBackup(theDB, "Update");

                theDB.ExecuteUpdate(strSql, pss);

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

        public void deleteUserData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_User 정보  update
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));

                //DataBackup
                this.insertUserDataBackup(theDB, "Delete");

                theDB.ExecuteUpdate(strSql, pss);

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

        public void setUserData(SqlDataReader reader)
        {
            this.Userid = Helper.PreventNull((string)reader["USERID"]);
            this.Password = Helper.PreventNull((string)reader["PASSWORD"]);
            this.Username = Helper.PreventNull((string)reader["USERNAME"]);
            this.Passwordchangedtime = Helper.PreventNull((string)reader["PASSWORDCHANGEDTIME"]);
            this.Passwordexpiredtime = Helper.PreventNull((string)reader["PASSWORDEXPIREDTIME"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Disabled = Helper.PreventNull((string)reader["DISABLED"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT userid                            userid              " +
                    "       ,ISNULL(password            ,'')   password            " +
                    "       ,ISNULL(username            ,'')   username            " +
                    "       ,ISNULL(passwordchangedtime  ,'')  passwordchangedtime " +
                    "       ,ISNULL(passwordexpiredtime,'')    passwordexpiredtime " +
                    "       ,ISNULL(createtime          ,'')   createtime          " +
                    "       ,ISNULL(createman           ,'')   createman           " +
                    "       ,ISNULL(disabled            ,'')   disabled            " +
                    "   FROM tb_user                                               ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_user                " +
                    "            (userid                 " +
                    "            ,password               " +
                    "            ,username               " +
                    "            ,passwordchangedtime    " +
                    "            ,passwordexpiredtime    " +
                    "            ,createtime             " +
                    "            ,createman              " +
                    "            ,disabled)              " +
                    "      VALUES                        " +
                    "            (@USERID                " +
                    "            ,@PASSWORD              " +
                    "            ,@USERNAME              " +
                    "            ,@PASSWORDCHANGEDTIME   " +
                    "            ,@PASSWORDEXPIREDTIME   " +
                    "            ,@CREATETIME            " +
                    "            ,@CREATEMAN             " +
                    "            ,@DISABLED)             ";

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_user                                       " +
                    "    SET  userid               = @USERID                " +
                    "        ,password             = @PASSWORD              " +
                    "        ,username             = @USERNAME              " +
                    "        ,passwordchangedtime  = @PASSWORDCHANGEDTIME   " +
                    "        ,passwordexpiredtime  = @PASSWORDEXPIREDTIME   " +
                    "        ,createtime           = @CREATETIME            " +
                    "        ,createman            = @CREATEMAN             " +
                    "        ,disabled             = @DISABLED              " +
                    "  WHERE userid                = @USERID1               "; 

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_user     " +
                    "  WHERE userid = @USERID "; 

            return strSql;
        }

        public void insertUserDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                UserData data = new UserData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE userid  = '" + this.Userid + "' ";

                try
                {
                    reader = theDB.ExecuteQuery(strSql);
                }
                catch (SqlException se)
                {
                    if (reader != null) reader.Close();
                    throw se;
                }

                if (reader.Read())
                {
                    data.setUserData(reader);
                }
                else
                {
                    if (reader != null) reader.Close();
                    return;
                }

                if (reader != null) reader.Close();

                internalIfData.Internaliftype = data.GetType().Name;
                internalIfData.Transactiontype = transactiontype;
                internalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                internalIfData.Ifvalue = data.makeInternalIfValue();
                internalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;
            }
            else
            {
                internalIfData.Internaliftype = this.GetType().Name;
                internalIfData.Transactiontype = transactiontype;
                internalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                internalIfData.Ifvalue = this.makeInternalIfValue();
                internalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;
            }

            try
            {
                internalIfData.insertInternalIfData(theDB);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
        }

        public string makeInternalIfValue()
        {
            string ifValue = "";
            ifValue += this.Userid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Password + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Username + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Passwordchangedtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Passwordexpiredtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Disabled;                                            

            return ifValue;
        }
    }
}
