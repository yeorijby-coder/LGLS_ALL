using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;
using ECP.Global;
using System.Data.SqlClient;

namespace ECP.Db.Entity
{
    [Serializable]
    public class MovementSapIfHistData : ECPObject 
    {
        const string CLASS = "MovementSapIfHistData";

        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
        private string iftime = "";

        public string Iftime
        {
            get { return iftime; }
            set { iftime = value; }
        }
        private string reqno = "";

        public string Reqno
        {
            get { return reqno; }
            set { reqno = value; }
        }

        private string crdat = "";

        public string Crdat
        {
            get { return crdat; }
            set { crdat = value; }
        }
        private string crtim = "";

        public string Crtim
        {
            get { return crtim; }
            set { crtim = value; }
        }

        private string matnr = "";

        public string Matnr
        {
            get { return matnr; }
            set { matnr = value; }
        }

        private string charg = "";

        public string Charg
        {
            get { return charg; }
            set { charg = value; }
        }
        private string pallet = "";

        public string Pallet
        {
            get { return pallet; }
            set { pallet = value; }
        }
        private string menge = "";

        public string Menge
        {
            get { return menge; }
            set { menge = value; }
        }
        private string frwerks = "";

        public string Frwerks
        {
            get { return frwerks; }
            set { frwerks = value; }
        }
        private string frlgort = "";

        public string Frlgort
        {
            get { return frlgort; }
            set { frlgort = value; }
        }
        private string towerks = "";

        public string Towerks
        {
            get { return towerks; }
            set { towerks = value; }
        }
        private string tolgort = "";

        public string Tolgort
        {
            get { return tolgort; }
            set { tolgort = value; }
        }

        private string rqnam = "";

        public string Rqnam
        {
            get { return rqnam; }
            set { rqnam = value; }
        }

        private string sampleflag = "";

        public string Sampleflag
        {
            get { return sampleflag; }
            set { sampleflag = value; }
        }
        private string transcompletetime = "";

        public string Transcompletetime
        {
            get { return transcompletetime; }
            set { transcompletetime = value; }
        }

        private string dbConnectionString = "";
        public MovementSapIfHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertMovementSapIfHistData(DBConnection theDB)
        {
             PreparedSqlParameter pss = null;

             try
             {
                 string strSql = getInsertSQL();

                 theDB.Start();
                 pss = theDB.GetPreparedSqlParameter();

                 pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime)); 
                 pss.SetVarChar("IFTIME", Helper.PreventNull(this.Iftime));
                 pss.SetVarChar("REQNO", Helper.PreventNull(this.Reqno));
                 pss.SetVarChar("CRDAT", Helper.PreventNull(this.Crdat));
                 pss.SetVarChar("CRTIM", Helper.PreventNull(this.Crtim));
                 pss.SetVarChar("MATNR", Helper.PreventNull(this.Matnr));
                 pss.SetVarChar("CHARG", Helper.PreventNull(this.Charg));
                 pss.SetVarChar("PALLET", Helper.PreventNull(this.Pallet));
                 pss.SetVarChar("MENGE", Helper.PreventNull(this.Menge));
                 pss.SetVarChar("FRWERKS", Helper.PreventNull(this.Frwerks));
                 pss.SetVarChar("FRLGORT", Helper.PreventNull(this.Frlgort));
                 pss.SetVarChar("TOWERKS", Helper.PreventNull(this.Towerks));
                 pss.SetVarChar("TOLGORT", Helper.PreventNull(this.Tolgort));
                 pss.SetVarChar("RQNAM", Helper.PreventNull(this.Rqnam));
                 pss.SetVarChar("SAMPLEFLAG", Helper.PreventNull(this.Sampleflag));
                 pss.SetVarChar("TRANSCOMPLETETIME", Helper.PreventNull(this.Transcompletetime));

                 theDB.ExecuteUpdate(strSql, pss);
                 theDB.Commit();
             }
             catch (SqlException se)
             {
                 if (theDB != null) theDB.Rollback();
                 throw se;
             }
         }

        public void setMovementSapIfHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Iftime = Helper.PreventNull(list[0]);
            this.Reqno = Helper.PreventNull(list[1]);
            this.Crdat = Helper.PreventNull(list[2]);
            this.Crtim = Helper.PreventNull(list[3]);
            this.Matnr = Helper.PreventNull(list[4]);
            this.Charg = Helper.PreventNull(list[5]);
            this.Pallet = Helper.PreventNull(list[6]);
            this.Menge = Helper.PreventNull(list[7]);
            this.Frwerks = Helper.PreventNull(list[8]);
            this.Frlgort = Helper.PreventNull(list[9]);
            this.Towerks = Helper.PreventNull(list[10]);
            this.Tolgort = Helper.PreventNull(list[11]);
            this.Rqnam = Helper.PreventNull(list[12]);
            this.Sampleflag = Helper.PreventNull(list[13]);
            this.Transcompletetime = Helper.PreventNull(list[14]);                 

        }

        public MovementSapIfData setMovementSapIfData(string[] list)
        {
            MovementSapIfData data = new MovementSapIfData();

            data.Iftime = Helper.PreventNull(list[0]);
            data.Reqno = Helper.PreventNull(list[1]);
            data.Crdat = Helper.PreventNull(list[2]);
            data.Crtim = Helper.PreventNull(list[3]);
            data.Matnr = Helper.PreventNull(list[4]);
            data.Charg = Helper.PreventNull(list[5]);
            data.Pallet = Helper.PreventNull(list[6]);
            data.Menge = Helper.PreventNull(list[7]);
            data.Frwerks = Helper.PreventNull(list[8]);
            data.Frlgort = Helper.PreventNull(list[9]);
            data.Towerks = Helper.PreventNull(list[10]);
            data.Tolgort = Helper.PreventNull(list[11]);
            data.Rqnam = Helper.PreventNull(list[12]);
            data.Sampleflag = Helper.PreventNull(list[13]);
            data.Transcompletetime = Helper.PreventNull(list[14]);         

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql = "" +
                    " INSERT INTO tb_movementsapifhist  " +
                    "            (backuptime            " +
                    "            ,iftime                " +
                    "            ,reqno                 " +
                    "            ,crdat                 " +
                    "            ,crtim                 " +
                    "            ,matnr                 " +
                    "            ,charg                 " +
                    "            ,pallet                " +
                    "            ,menge                 " +
                    "            ,frwerks               " +
                    "            ,frlgort               " +
                    "            ,towerks               " +
                    "            ,tolgort               " +
                    "            ,rqnam                 " +
                    "            ,sampleflag            " +
                    "            ,transcompletetime)    " +
                    "      VALUES                       " +
                    "            (@BACKUPTIME           " +
                    "            ,@IFTIME               " +
                    "            ,@REQNO                " +
                    "            ,@CRDAT                " +
                    "            ,@CRTIM                " +
                    "            ,@MATNR                " +
                    "            ,@CHARG                " +
                    "            ,@PALLET               " +
                    "            ,@MENGE                " +
                    "            ,@FRWERKS              " +
                    "            ,@FRLGORT              " +
                    "            ,@TOWERKS              " +
                    "            ,@TOLGORT              " +
                    "            ,@RQNAM                " +
                    "            ,@SAMPLEFLAG           " +
                    "            ,@TRANSCOMPLETETIME)   ";

            return strSql;
        }
    }
}
