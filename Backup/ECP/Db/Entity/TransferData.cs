using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Db;
using ECP.Global;
using System.Data;
using ECP.BizRule;

namespace ECP.Db.Entity
{
    [Serializable]
    public class TransferData : ECPObject 
    {
        const string CLASS = "TransferData";

        private bool isBoost = false;

        public bool IsBoost
        {
            get
            {
                return isBoost;
            }
            set
            {
                isBoost = value;
            }
        } 
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }

        private string commandid = "";

        public string Commandid
        {
            get { return commandid; }
            set { commandid = value; }
        }
        private string commandtype = "";

        public string Commandtype
        {
            get { return commandtype; }
            set { commandtype = value; }
        }

        private string movementtype = "";

        public string Movementtype
        {
            get { return movementtype; }
            set { movementtype = value; }
        }

        private string priority = "";

        public string Priority
        {
            get { return priority; }
            set { priority = value; }
        }
        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
        }
        private string batchno = "";

        public string Batchno
        {
            get { return batchno; }
            set { batchno = value; }
        }
        private string materialno = "";

        public string Materialno
        {
            get { return materialno; }
            set { materialno = value; }
        }
        private string qty = "";

        public string Qty
        {
            get { return qty; }
            set { qty = value; }
        }
        private string unit = "";

        public string Unit
        {
            get { return unit; }
            set { unit = value; }
        }
        private string fromdeviceid = "";

        public string Fromdeviceid
        {
            get { return fromdeviceid; }
            set { fromdeviceid = value; }
        }
        private string fromlocationid = "";

        public string Fromlocationid
        {
            get { return fromlocationid; }
            set { fromlocationid = value; }
        }
        private string fromsublocationid = "";

        public string Fromsublocationid
        {
            get { return fromsublocationid; }
            set { fromsublocationid = value; }
        }
        private string todeviceid = "";

        public string Todeviceid
        {
            get { return todeviceid; }
            set { todeviceid = value; }
        }
        private string tolocationid = "";

        public string Tolocationid
        {
            get { return tolocationid; }
            set { tolocationid = value; }
        }
        private string tosublocationid = "";

        public string Tosublocationid
        {
            get { return tosublocationid; }
            set { tosublocationid = value; }
        }
        private string currentcommandseq = "";

        public string Currentcommandseq
        {
            get { return currentcommandseq; }
            set { currentcommandseq = value; }
        }

        private string transferstatus = "";

        public string Transferstatus
        {
            get { return transferstatus; }
            set { transferstatus = value; }
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
        private string resultcode = "";

        public string Resultcode
        {
            get { return resultcode; }
            set { resultcode = value; }
        }
        private string executetime = "";

        public string Executetime
        {
            get { return executetime; }
            set { executetime = value; }
        }
        private string completetime = "";

        public string Completetime
        {
            get { return completetime; }
            set { completetime = value; }
        }
        private string executecount = "";

        public string Executecount
        {
            get { return executecount; }
            set { executecount = value; }
        }

        private string fromposition = "";

        public string Fromposition
        {
            get { return fromposition; }
            set { fromposition = value; }
        }
        private string toposition = "";

        public string Toposition
        {
            get { return toposition; }
            set { toposition = value; }
        }

        private string sampleflag = "";

        public string Sampleflag
        {
            get { return sampleflag; }
            set { sampleflag = value; }
        }

        private string transferdetailflag = "";

        public string Transferdetailflag
        {
            get { return transferdetailflag; }
            set { transferdetailflag = value; }
        }

        private string fromplant = "";

        public string Fromplant
        {
            get { return fromplant; }
            set { fromplant = value; }
        }

        //반송 조정된 우선 순위값을 위해 추가함.
        private string newpriority = "";

        public string Newpriority
        {
            get { return newpriority; }
            set { newpriority = value; }
        }

        //LGLS 작업 ID 및 PALLET ID 구분 위해 추가 
        private string palletidname = "";

        public string Palletidname
        {
            get { return palletidname; }
            set { palletidname = value; }
        }

        private string dbConnectionString = "";

        public TransferData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public TransferData(TransferData data) 
        {
                //this.Plantcode          = data.Plantcode         ;        
                this.Commandid          = data.Commandid         ;
                this.Commandtype        = data.Commandtype       ;
                this.Movementtype       = data.Movementtype      ;
                this.Priority           = data.Priority          ;
                this.Palletid           = data.Palletid          ;
                //this.Batchno            = data.Batchno           ;
                this.Materialno         = data.Materialno        ;
                this.Qty                = data.Qty               ;
                this.Unit               = data.Unit              ;
                this.Fromdeviceid       = data.Fromdeviceid      ;
                this.Fromlocationid     = data.Fromlocationid    ;
                this.Fromsublocationid  = data.Fromsublocationid ;
                this.Todeviceid         = data.Todeviceid        ;
                this.Tolocationid       = data.Tolocationid      ;
                this.Tosublocationid    = data.Tosublocationid   ;
                this.Currentcommandseq = data.Currentcommandseq;
                this.Transferstatus     = data.Transferstatus    ;
                this.Createtime         = data.Createtime        ;
                this.Createman          = data.Createman         ;
                //this.Resultcode         = data.Resultcode        ;
                this.Executetime        = data.Executetime       ;
                this.Completetime       = data.Completetime      ;
                //this.Executecount       = data.Executecount      ;
                this.Fromposition       = data.Fromposition      ;
                this.Toposition         = data.Toposition        ;
                //this.Sampleflag         = data.Sampleflag        ;
                this.Transferdetailflag = data.Transferdetailflag;
                //this.Fromplant          = data.Fromplant         ;
                this.Newpriority = data.Newpriority;
                this.Palletidname = data.Palletidname;
            }

        //SAP I/F 정보를 통해서 입고되는 transferData setting
        public void setTransferIngoData(MovementSapIfData movementSapIfData, string movementtype)
        {
            try
            {
                //this.Plantcode = GlobalConstant.PLANT_CODE;
                this.Commandid = movementSapIfData.Reqno;

                this.Fromposition = movementSapIfData.Frlgort;
                this.Toposition = movementSapIfData.Tolgort;

                this.Commandtype = EntityEnumData.COMMANDTYPE.INGO.ToString();  //입고
                this.Movementtype = movementtype;

                this.Priority = GlobalConstant.TRANSFER_PRIORITY_DEFAULT;
                this.Palletid = movementSapIfData.Pallet;
                //this.Batchno = movementSapIfData.Charg;
                this.Materialno = movementSapIfData.Matnr;
                this.Qty = movementSapIfData.Menge;

                MaterialData materialData = new MaterialData();
                materialData.getMaterialData(this.materialno);

                if((materialData == null) || (string.IsNullOrEmpty(materialData.Materialno))) 
                {
                    Exception mex = new Exception("(자재 번호 : " + this.materialno + ") 가 ECS에 등록되지 않았습니다.");
                    throw mex;
                }

                this.Unit = materialData.Unit;
                //this.Sampleflag = movementSapIfData.Sampleflag;

                FindLocationData findLocationData = new FindLocationData();
                findLocationData.getFindLocationData(this);

                this.Fromdeviceid = findLocationData.Fromdeviceid;
                this.Fromlocationid = findLocationData.Fromlocationid;
                this.Fromsublocationid = findLocationData.Fromsublocationid;

                this.Todeviceid = findLocationData.Todeviceid;
                this.Tolocationid = findLocationData.Tolocationid;
                this.Tosublocationid = findLocationData.Tosublocationid;

                this.Transferstatus = EntityEnumData.TRANSFERSTATUS.Wait.ToString();
                this.Createtime = movementSapIfData.Crdat + movementSapIfData.Crtim + "001";
                this.Createman = movementSapIfData.Rqnam;
                //this.Resultcode = "";
                this.Executetime = "";
                this.Completetime = "";
                //this.Executecount = "";

                this.Transferdetailflag = "N";
                //this.Fromplant = movementSapIfData.Frwerks;
                this.Newpriority = "";
            }
            catch (Exception se)
            {
                throw se;
            }
        }

        //SAP I/F 정보를 통해서 출고되는 transferData setting
        public TransferDivisionData setTransferOutgoData(FindPalletData findPalletData, Boolean newFalg, Boolean orderFlag, string movementtype)
        {
            TransferDivisionData transferDivisionData = new TransferDivisionData();

            this.Plantcode = findPalletData.Plantcode;

            try
            {
                if (newFalg)
                {
                    transferDivisionData.Plantcode = findPalletData.Plantcode;

                    transferDivisionData.Sapcommandid = findPalletData.Commandid;
                    this.Commandid = getMaxCommandId(movementtype);
                    transferDivisionData.Ecscommandid = this.Commandid;

                    transferDivisionData.Palletid = findPalletData.Palletid;
                    transferDivisionData.Batchno = findPalletData.Batchno;
                    transferDivisionData.Materialno = findPalletData.Materialno;

                    transferDivisionData.Sapqty = findPalletData.Sapqty;
                    transferDivisionData.Qty = findPalletData.Qty;

                    if (orderFlag)
                    {
                        transferDivisionData.Ordertype = GlobalConstant.ORDER_DATAMOVE;
                    }
                    else
                    {
                        transferDivisionData.Ordertype = GlobalConstant.ORDER_CONFIRMATION;
                    }

                    transferDivisionData.Createtime = findPalletData.Createtime;
                    transferDivisionData.Createman = findPalletData.Createman;
                }
                else
                {
                    this.Commandid = findPalletData.Commandid;
                }

                this.Fromposition = findPalletData.Fromposition;
                this.Toposition = findPalletData.Toposition;

                this.Commandtype = EntityEnumData.COMMANDTYPE.OUTGO.ToString();  //출고
                this.Movementtype = movementtype;

                this.Priority = GlobalConstant.TRANSFER_PRIORITY_DEFAULT;
                this.Palletid = findPalletData.Palletid;
                //this.Batchno = findPalletData.Batchno;
                this.Materialno = findPalletData.Materialno;


                this.Qty = findPalletData.Qty;

                MaterialData materialData = new MaterialData();
                materialData.getMaterialData(this.materialno);

                this.Unit = materialData.Unit;
                //this.Sampleflag = "N";

                this.Fromdeviceid = findPalletData.Fromdeviceid;
                this.Fromlocationid = findPalletData.Fromlocationid;
                this.Fromsublocationid = findPalletData.Fromsublocationid;

                this.Todeviceid = findPalletData.Todeviceid;
                this.Tolocationid = findPalletData.Tolocationid;
                this.Tosublocationid = findPalletData.Tosublocationid;

                this.Transferstatus = EntityEnumData.TRANSFERSTATUS.Wait.ToString();
                this.Createtime = findPalletData.Createtime;
                this.Createman = findPalletData.Createman;

                //this.Resultcode = "";
                this.Executetime = "";
                this.Completetime = "";
                //this.Executecount = "";

                this.Transferdetailflag = "N";
                this.Newpriority = "";
            }
            catch (SqlException se)
            {
                throw se;
            }
           
            return transferDivisionData;
        }

        //SAP I/F 정보를 통해서 이동되는 transferData setting
        public void setTransferSapMoveData(MovementSapIfData movementSapIfData, string movementtype)
        {
            try
            {
                //this.Plantcode = GlobalConstant.PLANT_CODE;
                this.Commandid = movementSapIfData.Reqno;

                this.Fromposition = movementSapIfData.Frlgort;
                this.Toposition = movementSapIfData.Tolgort;

                this.Commandtype = EntityEnumData.COMMANDTYPE.MOVE.ToString();  //이동
                this.Movementtype = movementtype;

                this.Priority = GlobalConstant.TRANSFER_PRIORITY_DEFAULT;
                this.Palletid = movementSapIfData.Pallet;
                //this.Batchno = movementSapIfData.Charg;
                this.Materialno = movementSapIfData.Matnr;
                this.Qty = movementSapIfData.Menge;

                MaterialData materialData = new MaterialData();
                materialData.getMaterialData(this.materialno);

                this.Unit = materialData.Unit;
                //this.Sampleflag = movementSapIfData.Sampleflag;

                FindLocationData findLocationData = new FindLocationData();
                findLocationData.getSapMoveFindLocationData(this);

                if((string.IsNullOrEmpty(findLocationData.Fromdeviceid)) ||
                    (string.IsNullOrEmpty(findLocationData.Todeviceid))
                  )
                {
                    throw new Exception("Not Find Location For Transferring");
                }

                this.Fromdeviceid = findLocationData.Fromdeviceid;
                this.Fromlocationid = findLocationData.Fromlocationid;
                this.Fromsublocationid = findLocationData.Fromsublocationid;

                this.Todeviceid = findLocationData.Todeviceid;
                this.Tolocationid = findLocationData.Tolocationid;
                this.Tosublocationid = findLocationData.Tosublocationid;

                this.Transferstatus = EntityEnumData.TRANSFERSTATUS.Wait.ToString();
                this.Createtime = movementSapIfData.Crdat + movementSapIfData.Crtim + "001";
                this.Createman = movementSapIfData.Rqnam;
                //this.Resultcode = "";
                this.Executetime = "";
                this.Completetime = "";
                //this.Executecount = "";

                this.Transferdetailflag = "N";
                //this.Fromplant = movementSapIfData.Frwerks;
                this.Newpriority = "";
            }
            catch (SqlException se)
            {
                throw se;
            }catch (Exception ex)
            {
                throw ex;
            }
        }

        //SAP I/F 정보를 통해서 이동되는 transferData setting
        public void setTransferExternalIfData(ExternalIfData externalIfData)
        {
            try
            {
                CodeMasterData codeData = new CodeMasterData();
                List<CodeMasterData> codeList = new List<CodeMasterData>();

                codeList = codeData.getPositionInfoList();

                string strData = externalIfData.Ifvalue;
                string nowDay = DateTime.Now.ToString("yyyyMMdd");

                this.Commandid = nowDay + strData.Substring(18, 4);

                string strCommandType = strData.Substring(17, 1);

                if (EntityEnumData.COMMANDTYPE.INGO.GetHashCode().ToString() == strCommandType)
                {
                    this.Commandtype = EntityEnumData.COMMANDTYPE.INGO.ToString();
                }
                else if (EntityEnumData.COMMANDTYPE.OUTGO.GetHashCode().ToString() == strCommandType)
                {
                    this.Commandtype = EntityEnumData.COMMANDTYPE.OUTGO.ToString();
                }
                else if (EntityEnumData.COMMANDTYPE.MOVE.GetHashCode().ToString() == strCommandType)
                {
                    this.Commandtype = EntityEnumData.COMMANDTYPE.MOVE.ToString();
                }

                this.Movementtype = EntityEnumData.MOVEMENTTYPE.WMS.ToString();
                this.Priority = int.Parse(strData.Substring(56, 3).Trim()).ToString();

                this.Palletid = strData.Substring(18, 4);
                
                this.Materialno = strData.Substring(60, 20).Trim();
                this.Qty = "1";
                this.Unit = "EA";

                string strFromStation = strData.Substring(33, 3);
                string strFromLoc = strData.Substring(36, 2) + strData.Substring(39, 2) + strData.Substring(41, 2);

                //From Position 및 장비 가져오기
                foreach (CodeMasterData cdTemp in codeList)
                {
                    if (cdTemp.Code == strFromStation)
                    {
                        //피킹존 입고 위치 찾기
                        if (this.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString() ||
                            this.Commandtype == EntityEnumData.COMMANDTYPE.MOVE.ToString())
                        {
                            //피킹 입고 Code 찾기
                            if (!string.IsNullOrEmpty(cdTemp.Attribute5))
                            {
                                foreach (CodeMasterData cdTemp1 in codeList)
                                {
                                    if (cdTemp1.Code == cdTemp.Attribute5)
                                    {
                                        this.Fromposition = cdTemp1.Attribute1;
                                        this.Fromdeviceid = cdTemp1.Attribute2;
                                        this.Fromlocationid = cdTemp1.Attribute3;
                                        this.Fromsublocationid = cdTemp1.Attribute4;
                                    }
                                }

                                break;
                            }
                        }

                        //Hirack 찾기
                        if (strFromLoc != "000000")
                        {
                            this.Fromposition = cdTemp.Attribute1;
                            this.Fromdeviceid = cdTemp.Attribute2;
                            this.Fromlocationid = GlobalConstant.SUBSYSTEM_RACK + int.Parse(strFromLoc.Substring(0, 2)).ToString();
                            this.Fromsublocationid = strFromLoc;
                        }
                        else
                        {
                            this.Fromposition = cdTemp.Attribute1;
                            this.Fromdeviceid = cdTemp.Attribute2;
                            this.Fromlocationid = cdTemp.Attribute3;
                            this.Fromsublocationid = cdTemp.Attribute4;
                        }

                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

                string strToStation = strData.Substring(46, 3);
                string strToLoc = strData.Substring(49, 2) + strData.Substring(52, 2) + strData.Substring(54, 2);

                //To Position 및 장비 가져오기
                foreach (CodeMasterData cdTemp in codeList)
                {
                    if (cdTemp.Code == strToStation)
                    {
                        //Hirack 찾기
                        if (strToLoc != "000000")
                        {
                            this.Toposition = cdTemp.Attribute1;
                            this.Todeviceid = cdTemp.Attribute2;
                            this.Tolocationid = GlobalConstant.SUBSYSTEM_RACK + int.Parse(strToLoc.Substring(0, 2)).ToString();
                            this.Tosublocationid = strToLoc;
                        }
                        else
                        {
                            this.Toposition = cdTemp.Attribute1;
                            this.Todeviceid = cdTemp.Attribute2;
                            this.Tolocationid = cdTemp.Attribute3;
                            this.Tosublocationid = cdTemp.Attribute4;
                        }

                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

                this.Currentcommandseq = "";
                this.Transferstatus = EntityEnumData.TRANSFERSTATUS.Wait.ToString();
                this.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                this.Createman = "SYSTEM";
                this.Executetime = "";
                this.Completetime = "";
                this.Transferdetailflag = "N";
                this.Newpriority = "";
                this.Palletidname = strData.Substring(26, 7).Trim();
            }
            catch (SqlException se)
            {
                throw se;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void insertTransferData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();
                
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(this.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(this.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(this.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(this.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(this.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(this.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(this.Palletidname));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertTransferDataBackup(theDB, "Insert");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        TransferHistData thData = new TransferHistData();
                        thData.insertTransferHistData(this);
                    }
                    catch (Exception exe)
                    {
                        //History Backup Error
                    }
                }
                
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertTransferData(TransferDivisionData tfdData, Boolean bFlag)
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

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(this.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(this.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(this.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(this.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(this.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(this.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(this.Palletidname));

                theDB.ExecuteUpdate(strSql, pss);

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertTransferDataBackup(theDB, "Insert");

                //Hirack 상태 및 반송 상태 reserved
                HirackData hData = new HirackData();
                hData.updateHirackOutgoStatus(theDB, this);

                //Pallet 상태 및 반송 reserved
                PalletData pData = new PalletData();
                pData.updatePalletStatus(theDB, this);

                //Pallet 상태 및 반송 reserved
                if (bFlag)
                {
                    tfdData.insertTransferDivisionData(theDB, true);
                }

                theDB.Commit();

                //DataBackup
                try
                {
                    TransferHistData thData = new TransferHistData();
                    thData.insertTransferHistData(this);
                }
                catch (Exception exe)
                {
                    //History Backup Error
                }
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

        public void insertMovementTransferData()
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

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(this.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(this.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(this.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(this.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(this.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(this.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(this.Palletidname));

                theDB.ExecuteUpdate(strSql, pss);

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertTransferDataBackup(theDB, "Insert");

                if (this.Commandtype == EntityEnumData.MOVEMENTTYPE.MOVE.ToString())
                {
                    //Hirack 상태 및 반송 상태 reserved
                    HirackData hData = new HirackData();

                    if (this.Fromposition == GlobalConstant.STORAGE_LOCATION_CODE)
                    {
                        hData.updateHirackOutgoStatus(theDB, this);
                    }

                    if (this.Toposition == GlobalConstant.STORAGE_LOCATION_CODE)
                    {
                        hData.updateHirackIngoStatus(theDB, this);
                    }
                }

                //Pallet 상태 및 반송 reserved
                PalletData pData = new PalletData();
                pData.updatePalletStatus(theDB, this);

                theDB.Commit();

                //DataBackup
                try
                {
                    TransferHistData thData = new TransferHistData();
                    thData.insertTransferHistData(this);
                }
                catch (Exception exe)
                {
                    //History Backup Error
                }
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

        public int updateTransferData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";
            int iResult = 0;

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getUpdateSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(this.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(this.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(this.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(this.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(this.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(this.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(this.Palletidname));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID1", Helper.PreventNull(this.Commandid));

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertTransferDataBackup(theDB, "Update");

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();

                //DataBackup
                try
                {
                    TransferHistData thData = new TransferHistData();
                    thData.insertTransferHistData(this);
                }
                catch (Exception exe)
                {
                    //History Backup Error
                }
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                iResult = -1;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            return iResult;
        }

        public void updateTransferData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";
            
            try
            {
                strSql = getUpdateSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(this.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(this.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(this.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(this.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(this.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(this.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(this.Palletidname));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID1", Helper.PreventNull(this.Commandid));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertTransferDataBackup(theDB, "Update");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        TransferHistData thData = new TransferHistData();
                        thData.insertTransferHistData(this);
                    }
                    catch (Exception exe)
                    {
                        //History Backup Error
                    }
                }
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public int deleteTransferData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";
            int iResult = 0;

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getDeleteSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertTransferDataBackup(theDB, "Delete");

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();

                //DataBackup
                try
                {
                    TransferHistData thData = new TransferHistData();
                    thData.insertTransferHistData(this);
                }
                catch (Exception exe)
                {
                    //History Backup Error
                }
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                iResult = -1;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            return iResult;
        }

        public void deleteTransferData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";
            
            try
            {
                strSql = getDeleteSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertTransferDataBackup(theDB, "Delete");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        TransferHistData thData = new TransferHistData();
                        thData.insertTransferHistData(this);
                    }
                    catch (Exception exe)
                    {
                        //History Backup Error
                    }
                }
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        //전체 반송 정보를 가져옴.
        public Dictionary<string, TransferData> getTransferDataList()
        {
            Dictionary<string, TransferData> list = new Dictionary<string, TransferData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();

            //CreateTime, material, batch, commandid 순으로 정렬
            strSql += " order by createtime, materialno, commandid ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferData transferData = new TransferData();

                    transferData.setTransferData(reader);

                    //From Postition 가져오기
                    PositionData pData = new PositionData();
                    //pData.Plantcode = transferData.Plantcode;
                    pData.Deviceid = transferData.Fromdeviceid;
                    pData.Locationid = transferData.Fromlocationid;
                    pData.Sublocationid = transferData.Fromsublocationid;
                    pData.Position = GlobalConstant.FROM_TEXT;

                    pData.getPositionData(pData);
                    transferData.Fromposition = pData.Sapposition;
                    transferData.Sampleflag = pData.Sampleflag;

                    //TO Position 가져오기
                    pData = new PositionData();
                    //pData.Plantcode = transferData.Plantcode;
                    pData.Deviceid = transferData.Todeviceid;
                    pData.Locationid = transferData.Tolocationid;
                    pData.Sublocationid = transferData.Tosublocationid;
                    pData.Position = GlobalConstant.TO_TEXT;

                    pData.getPositionData(pData);
                    transferData.Toposition = pData.Sapposition;

                    //TransferDetail 쪼개짐 여부
                    TransferDetailData tfData = new TransferDetailData();
                    int iCnt = tfData.getTransferDetailDataCnt(transferData);

                    if (iCnt != 0)
                    {
                        transferData.Transferdetailflag = "Y";
                    }
                    else
                    {
                        transferData.Transferdetailflag = "N";
                    }

                    list.Add(transferData.Commandid, transferData);
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

        public int getTransferDataCnt(MovementSapIfData msData)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = " SELECT  COUNT(*)             CNT                          " +
                            "   FROM tb_transfer                                        " +
                            "  WHERE commandid = '" + msData.Reqno              + "'    ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    iCnt = (int)reader["CNT"];
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

        public int getTransferDataCnt()
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = " SELECT  COUNT(*)             CNT                          " +
                            "   FROM tb_transfer                                        " +
                            "  WHERE commandid = '" + this.Commandid + "'               ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    iCnt = (int)reader["CNT"];
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

        public int getTransferDataWaitCnt()
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = " SELECT  COUNT(*)             CNT                          " +
                            "   FROM tb_transfer                                        " +
                            "  WHERE commandid = '" + this.Commandid + "'               " +
                            "    AND transferstatus = 'Wait'                            ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    iCnt = (int)reader["CNT"];
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

        public string getMaxCommandId(string movementtype)
        {
            string commandid = "";
            string strType = "";
            string commanddate = DateTime.Now.ToString("yyyyMMdd");

            string strSql = "PR_GETMAXCOMMANDID";

            DBConnection theDB = null;
            PreparedSqlParameter pss = null;

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                pss = theDB.GetPreparedSqlParameter();

                if (movementtype == EntityEnumData.MOVEMENTTYPE.ECS.ToString())
                {
                    strType = GlobalConstant.ECS_PREFIX;
                }
                else if (movementtype == EntityEnumData.MOVEMENTTYPE.MOVE.ToString())
                {
                    strType = GlobalConstant.MOVE_PREFIX;
                }
                else
                {
                    strType = GlobalConstant.SAP_PREFIX;
                }

                pss.SetProcedureParameter("RetVal", SqlDbType.Int, 0,
                                           ParameterDirection.ReturnValue, "");
                pss.SetProcedureParameter("commandtype", SqlDbType.VarChar, 1,
                                           ParameterDirection.Input, strType);
                pss.SetProcedureParameter("commanddate", SqlDbType.VarChar, 8,
                                           ParameterDirection.Input, commanddate);
                pss.SetProcedureParameter("commandid", SqlDbType.VarChar, 20,
                                           ParameterDirection.Output, "");

                theDB.ExecuteUpdate(strSql, pss, CommandType.StoredProcedure);

                List<SqlParameter> sqlParameters = pss.GetParameters();

                int returnValue = (int)sqlParameters[0].Value; //커맨트 객체에서 가져오실 땐, 저 위에서 정의한 'RetVal'을 지정하면 됩니다.

                if (returnValue == 0) //정상이면...
                {
                    commandid = sqlParameters[3].Value.ToString(); //Output 변수인 @p2를 사용할 수 있죠.
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            return commandid;
        }

        //Command id를 통해 반송 명령 가져오기
        public void getTransferData(string commandid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += " WHERE commandid = '" + commandid + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    this.setTransferData(reader);
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
        
        //자신보다 우선 진행될 반송 정보를 가져옴.
        public List<TransferData> getTransferQueList()
        {
            List<TransferData> list = new List<TransferData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferData transferData = new TransferData();

                    transferData.setTransferData(reader);

                    list.Add(transferData);
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

        public void setTransferData(SqlDataReader reader)
        {
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Commandid = Helper.PreventNull((string)reader["COMMANDID"]);
            this.Commandtype = Helper.PreventNull((string)reader["COMMANDTYPE"]);
            this.Movementtype = Helper.PreventNull((string)reader["MOVEMENTTYPE"]);
            this.Priority = Helper.PreventNull((string)reader["PRIORITY"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            //this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            //this.Fromplant = Helper.PreventNull((string)reader["FROMPLANT"]);
            this.Fromdeviceid = Helper.PreventNull((string)reader["FROMDEVICEID"]);
            this.Fromlocationid = Helper.PreventNull((string)reader["FROMLOCATIONID"]);
            this.Fromsublocationid = Helper.PreventNull((string)reader["FROMSUBLOCATIONID"]);
            this.Todeviceid = Helper.PreventNull((string)reader["TODEVICEID"]);
            this.Tolocationid = Helper.PreventNull((string)reader["TOLOCATIONID"]);
            this.Tosublocationid = Helper.PreventNull((string)reader["TOSUBLOCATIONID"]);
            this.Currentcommandseq = Helper.PreventNull((string)reader["CURRENTCOMMANDSEQ"]); 
            this.Transferstatus = Helper.PreventNull((string)reader["TRANSFERSTATUS"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            //this.Resultcode = Helper.PreventNull((string)reader["RESULTCODE"]);
            this.Executetime = Helper.PreventNull((string)reader["EXECUTETIME"]);
            this.Completetime = Helper.PreventNull((string)reader["COMPLETETIME"]);
            //this.Executecount = Helper.PreventNull((string)reader["EXECUTECOUNT"]);
            this.Palletidname = Helper.PreventNull((string)reader["PALLETIDNAME"]);
            
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql =" " +
                    " SELECT  commandid                                           " +
                    "        ,ISNULL(commandtype       ,'')    commandtype        " +
                    "        ,ISNULL(movementtype      ,'')   movementtype        " +
                    "        ,ISNULL(priority          ,'')    priority           " +
                    "        ,ISNULL(palletid          ,'')    palletid           " +
                    "        ,ISNULL(materialno        ,'')    materialno         " +
                    "        ,ISNULL(qty               ,'')    qty                " +
                    "        ,ISNULL(unit              ,'')    unit               " +
                    "        ,ISNULL(fromdeviceid      ,'')    fromdeviceid       " +
                    "        ,ISNULL(fromlocationid    ,'')    fromlocationid     " +
                    "        ,ISNULL(fromsublocationid ,'')    fromsublocationid  " +
                    "        ,ISNULL(todeviceid        ,'')    todeviceid         " +
                    "        ,ISNULL(tolocationid      ,'')    tolocationid       " +
                    "        ,ISNULL(tosublocationid   ,'')    tosublocationid    " +
                    "        ,ISNULL(currentcommandseq ,'')    currentcommandseq  " +
                    "        ,ISNULL(transferstatus    ,'')    transferstatus     " +
                    "        ,ISNULL(createtime        ,'')    createtime         " +
                    "        ,ISNULL(createman         ,'')    createman          " +
                    "        ,ISNULL(executetime       ,'')    executetime        " +
                    "        ,ISNULL(completetime      ,'')    completetime       " +
                    "        ,ISNULL(palletidname      ,'')    palletidname       " +
                    "   FROM tb_transfer                                          ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql =" " + 
                    "INSERT INTO tb_transfer        " +
                    "           (commandid          " +
                    "           ,commandtype        " +
                    "           ,movementtype       " +
                    "           ,priority           " +
                    "           ,palletid           " +
                    "           ,materialno         " +
                    "           ,qty                " +
                    "           ,unit               " +
                    "           ,fromdeviceid       " +
                    "           ,fromlocationid     " +
                    "           ,fromsublocationid  " +
                    "           ,todeviceid         " +
                    "           ,tolocationid       " +
                    "           ,tosublocationid    " +
                    "           ,currentcommandseq  " +
                    "           ,transferstatus     " +
                    "           ,createtime         " +
                    "           ,createman          " +
                    "           ,executetime        " +
                    "           ,completetime       " +
                    "           ,palletidname)      " +
                    "     VALUES                    " +
                    "           (@COMMANDID         " +
                    "           ,@COMMANDTYPE       " +
                    "           ,@MOVEMENTTYPE      " +
                    "           ,@PRIORITY          " +
                    "           ,@PALLETID          " +
                    "           ,@MATERIALNO        " +
                    "           ,@QTY               " +
                    "           ,@UNIT              " +
                    "           ,@FROMDEVICEID      " +
                    "           ,@FROMLOCATIONID    " +
                    "           ,@FROMSUBLOCATIONID " +
                    "           ,@TODEVICEID        " +
                    "           ,@TOLOCATIONID      " +
                    "           ,@TOSUBLOCATIONID   " +
                    "           ,@CURRENTCOMMANDSEQ " +
                    "           ,@TRANSFERSTATUS    " +
                    "           ,@CREATETIME        " +
                    "           ,@CREATEMAN         " +
                    "           ,@EXECUTETIME       " +
                    "           ,@COMPLETETIME      " +
                    "           ,@PALLETIDNAME)     ";
                            
            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";

            strSql = " " +
                    " UPDATE tb_transfer                              " +
                    "    SET  commandid          = @COMMANDID         " +
                    "        ,commandtype        = @COMMANDTYPE       " +
                    "        ,movementtype       = @MOVEMENTTYPE      " +
                    "        ,priority           = @PRIORITY          " +
                    "        ,palletid           = @PALLETID          " +
                    "        ,materialno         = @MATERIALNO        " +
                    "        ,qty                = @QTY               " +
                    "        ,unit               = @UNIT              " +
                    "        ,fromdeviceid       = @FROMDEVICEID      " +
                    "        ,fromlocationid     = @FROMLOCATIONID    " +
                    "        ,fromsublocationid  = @FROMSUBLOCATIONID " +
                    "        ,todeviceid         = @TODEVICEID        " +
                    "        ,tolocationid       = @TOLOCATIONID      " +
                    "        ,tosublocationid    = @TOSUBLOCATIONID   " +
                    "        ,currentcommandseq  = @CURRENTCOMMANDSEQ " +
                    "        ,transferstatus     = @TRANSFERSTATUS    " +
                    "        ,createtime         = @CREATETIME        " +
                    "        ,createman          = @CREATEMAN         " +
                    "        ,executetime        = @EXECUTETIME       " +
                    "        ,completetime       = @COMPLETETIME      " +
                    "        ,palletidname       = @PALLETIDNAME      " +
                    " WHERE  commandid = @COMMANDID1                  ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";

            strSql = " " +
                    " DELETE tb_transfer                " +
                    " WHERE  commandid = @COMMANDID     ";

            return strSql;
        }

        public void insertTransferDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                TransferData data = new TransferData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE commandid  = '" + this.Commandid            + "' ";

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
                    data.setTransferData(reader);
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
            ifValue +=  this.Commandid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Commandtype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Movementtype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Priority + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Unit + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Fromdeviceid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Fromlocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Fromsublocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Todeviceid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tolocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tosublocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Currentcommandseq + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Transferstatus + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Executetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Completetime;

            return ifValue;
        }
    }
}
