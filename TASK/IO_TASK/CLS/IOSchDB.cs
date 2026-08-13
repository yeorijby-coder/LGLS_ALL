using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Data;
using System.Data.OleDb;
using NpgsqlTypes;
using Samoh_Lib;

namespace TSK_COMM_IOSCH
{
    public class IOSchDB : MainClass
	{
#if ORACLE
        //RFID수신 후 작업처리.
        public bool B_SP_CHK_DIV1_JOB_CREATE(ref CUserDb argBdb, string strAREA_NO, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_DIV1_JOB_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_DIV2_JOB_CREATE(ref CUserDb argBdb, string strAREA_NO, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_DIV2_JOB_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_DIV3_JOB_CREATE(ref CUserDb argBdb, string strAREA_NO, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_DIV3_JOB_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_DIV4_JOB_CREATE(ref CUserDb argBdb, string strAREA_NO, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_DIV4_JOB_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        //화물 도착 후 이동처리(각지점)
        public bool ECS_SP_CHK_CV_MOVE_CREATE(ref CUserDb argBdb, string strWH_TYP, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_CV_MOVE_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        public bool ECS_SP_CHK_SC_JOB_CALL(ref CUserDb argBdb, string strWH_TYP, string strSC_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_SC_JOB_CALL";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;
            argBdb.comMain.Parameters.Add("IN_vSC_NO", DbLang.VARCHAR, 255).Value = strSC_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool ECS_SP_CHK_SC_INTER_LOCK(ref CUserDb argBdb, string strWH_TYP, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_SC_INTER_LOCK";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_RECHECK_JOB(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_RECHECK_JOB";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_RFID2_JOB_CRT(ref CUserDb argBdb, string strAREA_NO, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_RFID2_JOB_CRT";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_RFID3_JOB_CRT(ref CUserDb argBdb, string strAREA_NO, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_RFID3_JOB_CRT";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_RFID4_JOB_CRT(ref CUserDb argBdb, string strAREA_NO, string strSITENO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_RFID4_JOB_CRT";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        //입고 작업생성.
        public bool ECS_SP_CHK_COMP_ARR_CV(ref CUserDb argBdb, string strWH_TYP, string strSITENO1, string strSITENO2, string strSITENO3, string strSITENO4, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_COMP_ARR_CV";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;
            argBdb.comMain.Parameters.Add("IN_vSITENO1", DbLang.VARCHAR, 255).Value = strSITENO1;
            argBdb.comMain.Parameters.Add("IN_vSITENO2", DbLang.VARCHAR, 255).Value = strSITENO2;
            argBdb.comMain.Parameters.Add("IN_vSITENO3", DbLang.VARCHAR, 255).Value = strSITENO3;
            argBdb.comMain.Parameters.Add("IN_vSITENO4", DbLang.VARCHAR, 255).Value = strSITENO4;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_IN_31JOB_CREATE(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, string strSITENO, string strRP_NO1, string strRP_NO2, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_IN_31JOB_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;
            argBdb.comMain.Parameters.Add("IN_vSITENO", DbLang.VARCHAR, 255).Value = strSITENO;
            argBdb.comMain.Parameters.Add("IN_vRP_NO1", DbLang.VARCHAR, 255).Value = strRP_NO1;
            argBdb.comMain.Parameters.Add("IN_vRP_NO2", DbLang.VARCHAR, 255).Value = strRP_NO2;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool ECS_SP_CHK_COMP_ARR_RTV(ref CUserDb argBdb, string strWH_TYP, string strRTV_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_COMP_ARR_RTV";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;
            argBdb.comMain.Parameters.Add("IN_vRTV_NO", DbLang.VARCHAR, 255).Value = strRTV_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_MERGE(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, string strRP_NO1, string strRP_NO2, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_MERGE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;
            argBdb.comMain.Parameters.Add("IN_vRP_NO1", DbLang.VARCHAR, 255).Value = strRP_NO1;
            argBdb.comMain.Parameters.Add("IN_vRP_NO2", DbLang.VARCHAR, 255).Value = strRP_NO2;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_COMP_RET(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_COMP_RET";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_EQUALITY(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, string strRP_NO1, string strRP_NO2, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_EQUALITY";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;
            argBdb.comMain.Parameters.Add("IN_vRP_NO1", DbLang.VARCHAR, 255).Value = strRP_NO1;
            argBdb.comMain.Parameters.Add("IN_vRP_NO2", DbLang.VARCHAR, 255).Value = strRP_NO2;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool B_SP_CHK_EMG_TO_NOEMG(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, string strRP_NO1, string strRP_NO2, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_EMG_TO_NOEMG";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;
            argBdb.comMain.Parameters.Add("IN_vRP_NO1", DbLang.VARCHAR, 255).Value = strRP_NO1;
            argBdb.comMain.Parameters.Add("IN_vRP_NO2", DbLang.VARCHAR, 255).Value = strRP_NO2;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool ECS_SP_CHK_RTV_JOB_CALL(ref CUserDb argBdb, string strWH_TYP, string strRTV_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_RTV_JOB_CALL";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;
            argBdb.comMain.Parameters.Add("IN_vRTV_NO", DbLang.VARCHAR, 255).Value = strRTV_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool ECS_SP_CHK_RTV_INTER_LOCK(ref CUserDb argBdb, string strWH_TYP, string strRTV_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_RTV_INTER_LOCK";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;
            argBdb.comMain.Parameters.Add("IN_vRTV_NO", DbLang.VARCHAR, 255).Value = strRTV_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool ECS_SP_CHK_COMP_ARR_SC(ref CUserDb argBdb, string strWH_TYP, string strSC_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_CHK_COMP_ARR_SC";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_TYP", DbLang.VARCHAR, 255).Value = strWH_TYP;
            argBdb.comMain.Parameters.Add("IN_vSC_NO", DbLang.VARCHAR, 255).Value = strSC_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool ECS_SP_DEL_HISTORY(ref CUserDb argBdb, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "ECS_SP_DEL_HISTORY";
            argBdb.comMain.Parameters.Clear();

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        
        //RTV 작업 LOAD 작업대에서 작업 위치값 UPDATE 하는 프로시저
        public bool SP_CHK_ASRS_RTV_PICK_POINT(ref CUserDb argBdb, 
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                              ref string strRtnMsg, 
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_RTV_PICK_POINT";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        //asrs 창고 RTV JOB CALL 프로시저 
        public bool SP_CHK_ASRS_RTV_JOB_CALL(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strFLOOR,
                                                  string strRTV_NO,
                                                  string strRTV_NO2,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_RTV_JOB_CALL";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vFLOOR", DbLang.VARCHAR, 255).Value = strFLOOR;
            argBdb.comMain.Parameters.Add("IN_vRTV_NO", DbLang.VARCHAR, 255).Value = strRTV_NO;
            argBdb.comMain.Parameters.Add("IN_vRTV_NO2", DbLang.VARCHAR, 255).Value = strRTV_NO2;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_ASRS_3F_CV_MV_DIV(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_3F_CV_MV_DIV";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        public bool SP_CHK_ASRS_3F_CV_MV_251(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_3F_CV_MV_251";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }


        public bool SP_CHK_ASRS_CV_MV_PADIV(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_CV_MV_PADIV";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_ALFT_CV_MOVE(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ALFT_CV_MOVE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        public bool SP_CHK_ASRS_3F_MG_CV_MOVE(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_3F_MG_CV_MOVE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_ASRS_CV_MV_MZ(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_CV_MV_MZ";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_ASRS_CV_MV_PA_PK(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_CV_MV_PA_PK";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_SC_RET_CV_MOVE(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SC_RET_CV_MOVE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_RTV_TO_CV_MOVE(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_RTV_TO_CV_MOVE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_SIDE_SC_JOB_CALL(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SIDE_SC_JOB_CALL";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_ASRS_RTV_MG_CV_MOVE(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_RTV_MG_CV_MOVE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_SIDE_RTV_MG_CV_MOVE(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                                  string strGBN,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SIDE_RTV_MG_CV_MOVE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            argBdb.comMain.Parameters.Add("IN_vGBN", DbLang.VARCHAR, 255).Value = strGBN;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_ASRS_SC_JOB_CALL(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_SC_JOB_CALL";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_SC_ERR_JOB_DOING(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SC_ERR_JOB_DOING";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_SC_WECS_ERROR(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SC_WECS_ERROR";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        public bool SP_CHK_SC_COMP(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SC_COMP";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }


        public bool SP_CHK_RTV_INTER_LOCK(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_RTV_INTER_LOCK";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_RTV_COMP(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_RTV_COMP";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_ASRS_COMP_CV(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_COMP_CV";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_MG_JOB_CREATE(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_MG_JOB_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        public bool SP_CHK_SIDE_RTV_JOB_CALL(ref CUserDb argBdb,
                                                  string strGRP_TYP,
                                                  string strSTRG_TYP,
                                                  string strEQMT_TYP,
                                                  string strGRP_NO,
                                                  string strMC_NO,
                                              ref string strRtnMsg,
                                                 ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SIDE_RTV_JOB_CALL";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            argBdb.comMain.Parameters.Add("IN_vEQMT_TYP", DbLang.VARCHAR, 255).Value = strEQMT_TYP;
            argBdb.comMain.Parameters.Add("IN_vGRP_NO", DbLang.VARCHAR, 255).Value = strGRP_NO;
            argBdb.comMain.Parameters.Add("IN_vMC_NO", DbLang.VARCHAR, 255).Value = strMC_NO;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_SIDE_IN_HS_REQUEST(ref CUserDb argBdb, string strGRP_TYP, string strSTRG_TYP, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_SIDE_IN_HS_REQUEST";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        //입고 HS 도착시 작업 요청 프로시저 
        public bool SP_CHK_ASRS_IN_HS_REQUEST(ref CUserDb argBdb, string strGRP_TYP,string strSTRG_TYP, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_ASRS_IN_HS_REQUEST";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            argBdb.comMain.Parameters.Add("IN_vSTRG_TYP", DbLang.VARCHAR, 255).Value = strSTRG_TYP;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        //WCS -> WMS 작업 삭제 및 작업 완료 IF
        public bool SP_CHK_WMS_CMP_DEL_JOB_REPORT(ref CUserDb argBdb,string strGRP_TYP, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_WMS_CMP_DEL_JOB_REPORT";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        //WCS -> WMS 입고 작업 관련 IF
        public bool SP_CHK_WMS_IN_JOB_REPORT(ref CUserDb argBdb, string strGRP_TYP, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_WMS_IN_JOB_REPORT";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_IF_WCS_OTR_JOB_INFO(ref CUserDb argBdb, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_IF_WCS_OTR_JOB_INFO";
            argBdb.comMain.Parameters.Clear();

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }


        public bool SP_CHK_DEL_HISTORY(ref CUserDb argBdb, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_DEL_HISTORY";
            argBdb.comMain.Parameters.Clear();

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        public bool SP_CHK_IF_WCS_OUT_JOB_INFO(ref CUserDb argBdb, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_IF_WCS_OUT_JOB_INFO";
            argBdb.comMain.Parameters.Clear();

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        public bool SP_CHK_IF_WCS_IN_JOB_INFO(ref CUserDb argBdb, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_IF_WCS_IN_JOB_INFO";
            argBdb.comMain.Parameters.Clear();

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        
            //WCS -> WMS SC 작업 관련 IF
        public bool SP_CHK_INS_EQM_R_WCS_EQM_ERR(ref CUserDb argBdb, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_INS_EQM_R_WCS_EQM_ERR";
            argBdb.comMain.Parameters.Clear();

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        public bool SP_CHK_INS_IF_EQM_R_WCS_STAT(ref CUserDb argBdb, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_INS_IF_EQM_R_WCS_STAT";
            argBdb.comMain.Parameters.Clear();

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        //WCS -> WMS SC 작업 관련 IF
        public bool SP_CHK_WMS_SC_JOB_REPORT(ref CUserDb argBdb, string strGRP_TYP, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_WMS_SC_JOB_REPORT";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vGRP_TYP", DbLang.VARCHAR, 255).Value = strGRP_TYP;
            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
        //작업 완료 CALL.
        public bool B_SP_CHK_COMPLETE(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_COMPLETE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        //입고대 홈 이동.
        public bool B_SP_CHK_IN_HOME_CREATE(ref CUserDb argBdb, string strWH_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "B_SP_CHK_IN_HOME_CREATE";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }

        //입고대 홈 이동.
        public bool SP_CHK_RP_TEST(ref CUserDb argBdb, string strAREA_NO, string strWH_NO, string strRP_NO, ref string strRtnMsg, ref int nRetCd)
        {
            argBdb.comMain.CommandText = "SP_CHK_RP_TEST";
            argBdb.comMain.Parameters.Clear();

            argBdb.comMain.Parameters.Add("IN_vAREA_NO", DbLang.VARCHAR, 255).Value = strAREA_NO;
            argBdb.comMain.Parameters.Add("IN_vWH_NO", DbLang.VARCHAR, 255).Value = strWH_NO;
            argBdb.comMain.Parameters.Add("IN_vRP_NO", DbLang.VARCHAR, 255).Value = strRP_NO;

            //OUT
            argBdb.comMain.Parameters.Add("OT_nRETCD", DbLang.INTEGER).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vMSG", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;
            argBdb.comMain.Parameters.Add("OT_vDBERR", DbLang.VARCHAR, 255).Direction = ParameterDirection.Output;

            argBdb.comMain.CommandType = CommandType.StoredProcedure;
            argBdb.comMain.ExecuteScalar();

            if (System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value) != 0)
            {
                nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
                strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value) + " DB ERR MESSAGE [" + System.Convert.ToString(argBdb.comMain.Parameters["OT_vDBERR"].Value) + "]";
                return false;
            }

            nRetCd = System.Convert.ToInt32(argBdb.comMain.Parameters["OT_nRETCD"].Value);
            strRtnMsg = System.Convert.ToString(argBdb.comMain.Parameters["OT_vMSG"].Value);
            return true;
        }
#elif POSTGRESQL || SQL
#endif


    }
}