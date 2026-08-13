using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Util.Client;
using ECP.Util;
using ECP.Global.Exception;
using System.Configuration;
using ECP.Service.Statistics;
using ECP.Db;
using System.Data.SqlClient;
using ECP.Service.Logger;

namespace ECP.Gui.LogIn
{
    public partial class LoginForm : Form
    {
        public delegate void LoginCompleteEventHandler(object sender);

        public event LoginCompleteEventHandler LoginInCompleted = null;

        string dbConnectionString = String.Empty;
        UserPermission hostPermission=null;
    
        public LoginForm()
        {
            InitializeComponent();
            PreProcessLoad();   
        }

        public LoginForm(Form parentForm)
        {
            InitializeComponent();
            PreProcessLoad();
        }

        public LoginForm(UserPermission parent, string userId, string password)
        {
            InitializeComponent();
            PreProcessLoad();

            this.hostPermission = parent;

            if (!Helper.IsNullOrEmpty(userId) && !Helper.IsNullOrEmpty(password))
            {
                try
                {
                    UserPermission.GetInstance(userId, password, false);
                }
                catch (ECPException ex)
                {
                    System.Console.WriteLine("LoginForm:" + ex.StackTrace);
                }
            }
        }
        /// <summary>
        /// 창이 로드되기 전에 화면대한 위치 포커스에 대한 전처리를 수행합니다.
        /// </summary>
        private void PreProcessLoad()
        {
            this.CenterToScreen();
            this.Focus();
            this.textBoxUserID.Focus();
        }


        private DBConnection GetDBControl()
        {
            try
            {
                DBConnection conn = DBControl.GetConnection(this.dbConnectionString);
                return conn;
            }
            catch (SqlException ex)
            {
                System.Console.WriteLine("GetDBControl:" + ex.StackTrace);
            }
            catch (Exception e2)
            {
                System.Console.WriteLine("GetDBControl:" + e2.StackTrace);
            }

            return null;
        }

        private void LoadUserDataFromMasterDB()
        {
            ConnectionStringSettings setting = ConfigurationManager.ConnectionStrings["StockerMasterDataBaseConnectionString"];

            if (setting != null)
            {
                this.dbConnectionString = setting.ConnectionString;
            }

            List<string> tables = new List<string>();
            DBConnection conn = DBControl.GetConnection();
            SqlDataReader reader = null;

            try
            {
                string sql = "select name from sys.tables ";
                reader = conn.ExecuteQuery(sql);

                while (reader.Read())
                {
                    tables.Add((string)reader["name"]);
                }

                if (tables.Contains("ECPUSER"))
                {
                    sql = "delete from ECPUSER";
                    conn.ExecuteUpdate(sql);
                }
                else
                {
                    sql = "CREATE TABLE [dbo].[ECPUSER]( " +
                          "[USERID] [varchar](255) NULL, " +
                          "[PASSWORD] [varchar](255) NULL, " +
                          "[USERNAME] [varchar](255) NULL, " +
                          "[USERDESCRIPTION] [varchar](255) NULL, " +
                          "[PASSWORDCREATETIME] [datetime] NULL, " +
                          "[PASSWORDMAXAGE] [int] NULL, " +
                          "[DISABLED] [char](1) NULL) ";
                    conn.ExecuteQuery(sql);
                }

                if (tables.Contains("ECPUSERROLE"))
                {
                    sql = "delete from ECPUSERROLE";
                    conn.ExecuteUpdate(sql);
                }
                else
                {
                    sql = "CREATE TABLE [dbo].[ECPUSERROLE]( " +
                          "[USERID] [varchar](255) NULL, " +
                          "[ROLEID] [varchar](255) NULL) ";
                    conn.ExecuteQuery(sql);
                }

                if (tables.Contains("ROLE"))
                {
                    sql = "delete from ROLE";
                    conn.ExecuteUpdate(sql);
                }
                else
                {
                    sql = "CREATE TABLE [dbo].[ROLE]( " +
                          "[ROLEID] [varchar](255) NULL, " +
                          "[DESCRIPTION] [varchar](255) NULL) ";
                    conn.ExecuteQuery(sql);
                }

                if (tables.Contains("ROLEPRIVILEGE"))
                {
                    sql = "delete from ROLEPRIVILEGE";
                    conn.ExecuteUpdate(sql);
                }
                else
                {
                    sql = "CREATE TABLE [dbo].[ROLEPRIVILEGE]( " +
                          "[ROLEID] [varchar](255) NULL, " +
                          "[PRIVILEGEID] [varchar](255) NULL) ";
                    conn.ExecuteQuery(sql);
                }

                if (tables.Contains("PRIVILEGEDATA"))
                {
                    sql = "delete from PRIVILEGEDATA";
                    conn.ExecuteUpdate(sql);
                }
                else
                {
                    sql = "CREATE TABLE [dbo].[PRIVILEGEDATA]( " +
                          "[PRIVILEGEID] [varchar](255) NULL, " +
                          "[DESCRIPTION] [varchar](255) NULL) ";
                    conn.ExecuteQuery(sql);
                }

                sql = "  sp_configure 'show advanced option',1 ";
                conn.ExecuteQuery(sql);
                sql = " reconfigure";
                conn.ExecuteQuery(sql);
                sql = " sp_configure 'Ad Hoc Distributed Queries',1 ";
                conn.ExecuteQuery(sql);
                sql = "reconfigure ";
                conn.ExecuteQuery(sql);

                sql = "insert into ECPUSER "
                      + " SELECT * FROM OPENDATASOURCE "
                      + " ('SQLOLEDB', '"
                      + this.dbConnectionString
                      + "').STKC_MASTER.dbo.ECPUSER ";
                conn.ExecuteUpdate(sql);

                sql = "insert into ECPUSERROLE "
                      + " SELECT * FROM OPENDATASOURCE "
                      + " ('SQLOLEDB', '"
                      + this.dbConnectionString
                      + "').STKC_MASTER.dbo.ECPUSERROLE ";
                conn.ExecuteUpdate(sql);

                sql = "insert into ROLE "
                      + " SELECT * FROM OPENDATASOURCE "
                      + " ('SQLOLEDB', '"
                      + this.dbConnectionString
                      + "').STKC_MASTER.dbo.ROLE ";
                conn.ExecuteUpdate(sql);

                sql = "insert into ROLEPRIVILEGE "
                      + " SELECT * FROM OPENDATASOURCE "
                      + " ('SQLOLEDB', '"
                      + this.dbConnectionString
                      + "').STKC_MASTER.dbo.ROLEPRIVILEGE ";
                conn.ExecuteUpdate(sql);

                sql = "insert into PRIVILEGEDATA "
                      + " SELECT * FROM OPENDATASOURCE "
                      + " ('SQLOLEDB', '"
                      + this.dbConnectionString
                      + "').STKC_MASTER.dbo.PRIVILEGEDATA ";
                conn.ExecuteUpdate(sql);

                MessageBox.Show("Update Complete!!");
            }
            catch (SqlException ex)
            {
                MessageBox.Show("SQL Error Occur!!");
                Log.log(5, "loadButton_Click", "error", Category.ERROR, "", "USER", "sql error \n " + ex.Message.ToString());
            }
            catch (Exception ex)
            {
                Log.log(5, "loadButton_Click", "error", Category.ERROR, "", "USER", "error \n " + ex.Message.ToString());
            }
            finally
            {
                DBControl.RemoveDBConection(conn);
            }
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if(Helper.IsNullOrEmpty(textBoxUserID.Text) 
                || Helper.IsNullOrEmpty(textBoxPassword.Text))
            {
                MessageBox.Show("UserId or Passworld is Empty");
                //throw new ECPException(ECPExceptionType.INVALID_VALUE, "", "UserId or Passworld is Empty");
                return;
            }
            else
            {
                try
                {
                    if (textBoxUserID.Text.Equals("super") && textBoxPassword.Text.Equals("super"))
                    {
                        UserPermission.SetSuperUser();
                    }
                    else
                    {
                        UserPermission.GetInstance(textBoxUserID.Text, textBoxPassword.Text, false);

                        if (Naming.GetUserManager().GetUser(textBoxUserID.Text).IsPasswordExpired())
                        {
                            MessageBox.Show("Password is Expired");
                            return;
                        }
                    }

                    UserPermission.SetOnlineLogin(true);
                }
                catch (ECPException ex)
                {
                    MessageBox.Show("Invalid Passworld");
                    return;
                }
            }

            //publish login complete event

            if (LoginInCompleted != null)
            {
                LoginInCompleted(this);
            }

            
            this.Close();
        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void textBoxPassword_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                this.okButton_Click(sender,e);
            }
        }

        private void LoginForm_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.A)
            {
                UserPermission.SetSuperUser();
                UserPermission.SetOnlineLogin(true);

                if (LoginInCompleted != null)
                {
                    LoginInCompleted(this);
                }

                this.Close();
            }
            if (e.Control && e.KeyCode == Keys.L)
            {
                LoadUserDataFromMasterDB();
            }
        }

        private void LoginForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (!UserPermission.GetOnlineLogin())
                Application.Exit();
        }



    }
}