using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;
using ECP.Global;
using System.Data.SqlClient;

namespace ECP.Db.Entity
{
    [Serializable]
    public class MovementSapIfData : ECPObject 
    {
        const string CLASS = "MovementSapIfData";

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

        public MovementSapIfData()
        {
        }

        public MovementSapIfData(MovementSapIfData data)
        {
            this.Iftime = data.Iftime;
            this.Reqno = data.Reqno;
            this.Crdat = data.Crdat;
            this.Crtim = data.Crtim;
            this.Matnr = data.Matnr;
            this.Charg = data.Charg;
            this.Pallet = data.Pallet;
            this.Menge = data.Menge;
            this.Frwerks = data.Frwerks;
            this.Frlgort = data.Frlgort;
            this.Towerks = data.Towerks;
            this.Tolgort = data.Tolgort;
            this.Rqnam = data.Rqnam;
            this.Sampleflag = data.Sampleflag;
            this.Transcompletetime = data.Transcompletetime;
        }

        public MovementSapIfData(string reqno      ,string crdat      ,
                                 string crtim      ,string matnr      ,
                                 string charg      ,string pallet     ,
                                 string menge      ,string frwerks    ,
                                 string frlgort    ,string towerks    ,
                                 string tolgort    ,string rqnam      ,
                                 string sampleflag)
        {
            this.reqno = reqno;
            this.crdat = crdat;
            this.crtim = crtim;
            this.matnr = matnr;
            this.charg = charg;
            this.pallet = pallet;
            this.menge = menge;
            this.frwerks = frwerks;
            this.frlgort = frlgort;
            this.towerks = towerks;
            this.tolgort = tolgort;
            this.rqnam = rqnam;
            this.sampleflag = sampleflag; 
        }

        //TB_EXTERNALIF의 IFVALUE의 여러개의 반송 정보를 건의 반송정보 List로 분리함.
        public void getMovementSapIfData(string ifValue)
        {
            string[] strValueRst = ifValue.Split(GlobalConstant.MOVEMENT_DELIMITER);
            
            this.Reqno = Helper.PreventNull(strValueRst[0]);
            this.Crdat = Helper.PreventNull(strValueRst[1]);
            this.Crtim = Helper.PreventNull(strValueRst[2]);
            this.Matnr = Helper.PreventNull(strValueRst[3]);
            this.Charg = Helper.PreventNull(strValueRst[4]);
            this.Pallet = Helper.PreventNull(strValueRst[5]);
            this.Menge = Helper.PreventNull(strValueRst[6]);
            this.Frwerks = Helper.PreventNull(strValueRst[7]);
            this.Frlgort = Helper.PreventNull(strValueRst[8]);
            this.Towerks = Helper.PreventNull(strValueRst[9]);
            this.Tolgort = Helper.PreventNull(strValueRst[10]);
            this.Rqnam = Helper.PreventNull(strValueRst[11]);
            this.Sampleflag = Helper.PreventNull(strValueRst[12]);
         }

         //반송지시 정보 List 조회
         public int getMovementSapIfDataCount()
         {
             DBConnection theDB = null;
             SqlDataReader reader = null;
             int iCnt = 0;

             string strSql = getSelectSQL();
             strSql += "  WHERE reqno = '" + this.Reqno + "' ";

             try
             {
                 theDB = DBControl.GetConnection();
                 reader = theDB.ExecuteQuery(strSql);

                 while (reader.Read())
                 {
                     iCnt++;
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

             return iCnt;
         }

         //반송지시 정보 List 조회
         public void getMovementSapIfDataWithIftimeReqno(string iftime, string reqno)
         {
             DBConnection theDB = null;
             SqlDataReader reader = null;

             string strSql = getSelectSQL();
             strSql += " WHERE iftime = '" + iftime + "' " +
                       "   AND reqno  = '" + reqno + "' ";

             try
             {
                 theDB = DBControl.GetConnection();
                 reader = theDB.ExecuteQuery(strSql);

                 if (reader.Read())
                 {
                     this.setMovementSapIfData(reader);
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
         }

         //반송지시 정보 List 조회
         public List<MovementSapIfData> getMovementSapIfDataList()
         {
             List<MovementSapIfData> list = new List<MovementSapIfData>();

             DBConnection theDB = null;
             SqlDataReader reader = null;

             string strSql = getSelectSQL();

             try
             {
                 theDB = DBControl.GetConnection();
                 reader = theDB.ExecuteQuery(strSql);

                 while (reader.Read())
                 {
                     MovementSapIfData movementSapIfData = new MovementSapIfData();

                     movementSapIfData.setMovementSapIfData(reader);

                     list.Add(movementSapIfData);
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

         //반송지시 정보 List 조회
         public List<MovementSapIfData> getMovementSapIfDataListWithFromTo(string from, string to)
         {
             List<MovementSapIfData> list = new List<MovementSapIfData>();

             DBConnection theDB = null;
             SqlDataReader reader = null;

             string strSql = getSelectSQL();

             strSql += " WHERE iftime >= '" + from + "' " +
                       "   AND iftime <= '" + to + "' " +
                       " ORDER BY reqno                 ";

             try
             {
                 theDB = DBControl.GetConnection();
                 reader = theDB.ExecuteQuery(strSql);

                 while (reader.Read())
                 {
                     MovementSapIfData movementSapIfData = new MovementSapIfData();

                     movementSapIfData.setMovementSapIfData(reader);

                     list.Add(movementSapIfData);
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

         //반송지시 정보 List 조회
         public List<MovementSapIfData> getNotCompleteMovementSapIfDataList()
         {
             List<MovementSapIfData> list = new List<MovementSapIfData>();

             DBConnection theDB = null;
             SqlDataReader reader = null;

             string strSql = getSelectSQL();
             strSql += "  WHERE transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' ";

             try
             {
                 theDB = DBControl.GetConnection();
                 reader = theDB.ExecuteQuery(strSql);

                 while (reader.Read())
                 {
                     MovementSapIfData movementSapIfData = new MovementSapIfData();

                     movementSapIfData.setMovementSapIfData(reader);

                     list.Add(movementSapIfData);
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

         //TB_MOVEMENTSAPIF에 Data insert
         public void insertMovementSapIfData(DBConnection theDB, Boolean bkFlag)
         {
             PreparedSqlParameter pss = null;

             try
             {
                 string strSql = getInsertSQL();

                 theDB.Start();
                 pss = theDB.GetPreparedSqlParameter();

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

                 if (bkFlag)
                 {
                     //DataBackup
                     this.insertMovementSapIfDataBackup(theDB, "Insert");
                 }

                 theDB.ExecuteUpdate(strSql, pss);
                 theDB.Commit();
             }
             catch (SqlException se)
             {
                 if (theDB != null) theDB.Rollback();
                 throw se;
             }
         }

         //TB_MOVEMENTSAPIF에 TB_TRANSFER로 등록한 정보를 I/F 완료 처리함.
         public void updateMovementSapIfData(DBConnection theDB, Boolean bkFlag)
         {
             PreparedSqlParameter pss = null;

             try
             {
                 string strSql = getUpdateSQL();

                 theDB.Start();
                 pss = theDB.GetPreparedSqlParameter();

                 //TB_MOVEMENTSAPIF 정보  update                                                     
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
                 pss.SetVarChar("IFTIME", Helper.PreventNull(this.Iftime));
                 pss.SetVarChar("REQNO", Helper.PreventNull(this.Reqno));

                 if (bkFlag)
                 {
                     //DataBackup
                     this.insertMovementSapIfDataBackup(theDB, "Update");
                 }

                 theDB.ExecuteUpdate(strSql, pss);
                 theDB.Commit();
             }
             catch (SqlException se)
             {
                 if (theDB != null) theDB.Rollback();
                 throw se;
             }
         }

        public void setMovementSapIfData(SqlDataReader reader)
        {
            this.Iftime = Helper.PreventNull((string)reader["IFTIME"]);
            this.Reqno = Helper.PreventNull((string)reader["REQNO"]);
            this.Crdat = Helper.PreventNull((string)reader["CRDAT"]);
            this.Crtim = Helper.PreventNull((string)reader["CRTIM"]);
            this.Matnr = Helper.PreventNull((string)reader["MATNR"]);
            this.Charg = Helper.PreventNull((string)reader["CHARG"]);
            this.Pallet = Helper.PreventNull((string)reader["PALLET"]);
            this.Menge = Helper.PreventNull((string)reader["MENGE"]);
            this.Frwerks = Helper.PreventNull((string)reader["FRWERKS"]);
            this.Frlgort = Helper.PreventNull((string)reader["FRLGORT"]);
            this.Towerks = Helper.PreventNull((string)reader["TOWERKS"]);
            this.Tolgort = Helper.PreventNull((string)reader["TOLGORT"]);
            this.Rqnam = Helper.PreventNull((string)reader["RQNAM"]);
            this.Sampleflag = Helper.PreventNull((string)reader["SAMPLEFLAG"]);
            this.Transcompletetime = Helper.PreventNull((string)reader["TRANSCOMPLETETIME"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT iftime                        iftime             " +
                    "       ,ISNULL(reqno            ,'')  reqno              " +
                    "       ,ISNULL(crdat            ,'')  crdat              " +
                    "       ,ISNULL(crtim            ,'')  crtim              " +
                    "       ,ISNULL(matnr            ,'')  matnr              " +
                    "       ,ISNULL(charg            ,'')  charg              " +
                    "       ,ISNULL(pallet           ,'')  pallet             " +
                    "       ,ISNULL(menge            ,'')  menge              " +
                    "       ,ISNULL(frwerks          ,'')  frwerks            " +
                    "       ,ISNULL(frlgort          ,'')  frlgort            " +
                    "       ,ISNULL(towerks          ,'')  towerks            " +
                    "       ,ISNULL(tolgort          ,'')  tolgort            " +
                    "       ,ISNULL(rqnam            ,'')  rqnam              " +
                    "       ,ISNULL(sampleflag       ,'')  sampleflag         " +
                    "       ,ISNULL(transcompletetime,'')  transcompletetime  " +
                    "   FROM tb_movementsapif                                 ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql = "" +
                    " INSERT INTO tb_movementsapif      " +
                    "            (iftime                " +
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
                    "            (@IFTIME               " +
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

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE tb_movementsapif                          " +
                    "    SET  crdat              = @CRDAT              " +
                    "        ,crtim              = @CRTIM              " +
                    "        ,matnr              = @MATNR              " +
                    "        ,charg              = @CHARG              " +
                    "        ,pallet             = @PALLET             " +
                    "        ,menge              = @MENGE              " +
                    "        ,frwerks            = @FRWERKS            " +
                    "        ,frlgort            = @FRLGORT            " +
                    "        ,towerks            = @TOWERKS            " +
                    "        ,tolgort            = @TOLGORT            " +
                    "        ,rqnam              = @RQNAM              " +
                    "        ,sampleflag         = @SAMPLEFLAG         " +
                    "        ,transcompletetime  = @TRANSCOMPLETETIME  " +
                    "  WHERE  iftime             = @IFTIME             " +
                    "    AND  reqno              = @REQNO              ";

            return strSql;
        }

        public void insertMovementSapIfDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                MovementSapIfData data = new MovementSapIfData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE iftime  = '" + this.Iftime + "' " +
                          "   AND reqno   = '" + this.Reqno  + "' ";

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
                    data.setMovementSapIfData(reader);
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
            ifValue += this.Iftime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Reqno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Crdat + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Crtim + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Matnr + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Charg + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Pallet + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Menge + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Frwerks + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Frlgort + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Towerks + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tolgort + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Rqnam + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Sampleflag + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Transcompletetime; 

            return ifValue;
        }
    }
}
