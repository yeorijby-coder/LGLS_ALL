using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db.Entity;
using ECP.Global;
using ECP.Com.Tcp;

namespace ECP.BizRule
{
    [Serializable]
    public class MakeWmsIfString : ECPObject 
    {

        public MakeWmsIfString()
        {
        }

        public void insertStatus(string strStatus)
        {
            try
            {
                WMSCommand sCommand = new WMSCommand(WMSCommand.MESSAGE_TYPE.S);

                //Head
                sCommand.MessageBox = "ECS_MBX";
                sCommand.MessageSize = "43";

                //Body
                sCommand.MessageType = WMSCommand.MESSAGE_TYPE.S;

                sCommand.S_SC01                  = strStatus.Substring(0, 2);
                sCommand.S_SC01Status            = strStatus.Substring(2, 1);
                sCommand.S_LuggageNo01           = strStatus.Substring(3, 4);
                sCommand.S_SC02                  = strStatus.Substring(7, 2);
                sCommand.S_SC02Status            = strStatus.Substring(9, 1);
                sCommand.S_LuggageNo02           = strStatus.Substring(10, 4);
                sCommand.S_SC03                  = strStatus.Substring(14, 2);
                sCommand.S_SC03Status            = strStatus.Substring(16, 1);
                sCommand.S_LuggageNo03           = strStatus.Substring(17, 4);
                sCommand.S_SC04                  = strStatus.Substring(21, 2);
                sCommand.S_SC04Status            = strStatus.Substring(23, 1);
                sCommand.S_LuggageNo04           = strStatus.Substring(24, 4);
                sCommand.S_SC05                  = strStatus.Substring(28, 2);
                sCommand.S_SC05Status            = strStatus.Substring(30, 1);
                sCommand.S_LuggageNo05           = strStatus.Substring(31, 4);
                sCommand.S_ExtInputOnly          = strStatus.Substring(35, 1);
                sCommand.S_PickingWorkConveyor   = strStatus.Substring(36, 1);
                sCommand.S_MaterialInputConveyor = strStatus.Substring(37, 1);
                sCommand.S_InputConveyorMode     = strStatus.Substring(38, 1);
                sCommand.S_SC01ConveyorMode      = strStatus.Substring(39, 1);


                ExternalIfData exData = new ExternalIfData();
                exData.Externaliftype = WMSCommand.MESSAGE_TYPE.S.ToString();
                exData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                exData.Ifvalue = sCommand.BinaryString;

                exData.Ifvalue = exData.Ifvalue.Replace((char)2, GlobalConstant.START_CHAR);
                exData.Ifvalue = exData.Ifvalue.Replace((char)3, GlobalConstant.END_CHAR);

                exData.Transstarttime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                exData.Transcompletetime = GlobalConstant.SET_MAXTIME;

                exData.insertExternalIfData();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void insertError(string strError)
        {
            try
            {
                WMSCommand eCommand = new WMSCommand(WMSCommand.MESSAGE_TYPE.E);

                //Head
                eCommand.MessageBox = "ECS_MBX";
                eCommand.MessageSize = "23";

                //Body
                eCommand.MessageType = WMSCommand.MESSAGE_TYPE.E;

                eCommand.E_DeviceClass = strError.Substring(0,1);
                eCommand.E_DeviceNo = strError.Substring(1, 3);
                eCommand.E_ErrorKind = strError.Substring(4, 1);
                eCommand.E_ErrorCode = strError.Substring(5, 4);
                eCommand.E_LuggageNo = strError.Substring(9, 4);
                eCommand.E_ErrorBank = strError.Substring(13, 2);
                eCommand.E_ErrorBay = strError.Substring(15, 3);
                eCommand.E_ErrorLevel = strError.Substring(18, 2);
                
                ExternalIfData exData = new ExternalIfData();
                exData.Externaliftype = WMSCommand.MESSAGE_TYPE.E.ToString();
                exData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                exData.Ifvalue = eCommand.BinaryString;

                exData.Ifvalue = exData.Ifvalue.Replace((char)2, GlobalConstant.START_CHAR);
                exData.Ifvalue = exData.Ifvalue.Replace((char)3, GlobalConstant.END_CHAR);

                exData.Transstarttime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                exData.Transcompletetime = GlobalConstant.SET_MAXTIME;

                exData.insertExternalIfData();
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void insertTransferDetailComplete(TransferDetailData tfdData)
        {
            string strFrom = "";
            
            try
            {
                ItineraryData itData = new ItineraryData();
                itData.getItineraryData(tfdData.Pathid);

                TransferData tfData = new TransferData();
                tfData.getTransferData(tfdData.Commandid);


                strFrom = itData.Fromlocationid;

                if (strFrom.Contains(GlobalConstant.SUBSYSTEM_RACK))
                {
                    WMSCommand fCommand = new WMSCommand(WMSCommand.MESSAGE_TYPE.F);

                    //Head
                    fCommand.MessageBox = "ECS_MBX";
                    fCommand.MessageSize = "10";

                    //Body
                    fCommand.MessageType = WMSCommand.MESSAGE_TYPE.F;

                    fCommand.F_JobDefine = "2";              //Unit 출고
                    fCommand.F_LuggageNo = tfData.Palletid;
                    fCommand.F_CompleteClass = "2";         //출고 완료
                    fCommand.F_StepCount = "1";            //Hirack 출고

                    ExternalIfData exData = new ExternalIfData();
                    exData.Externaliftype = WMSCommand.MESSAGE_TYPE.F.ToString();
                    exData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    exData.Ifvalue = fCommand.BinaryString;

                    exData.Ifvalue = exData.Ifvalue.Replace((char)2, GlobalConstant.START_CHAR);
                    exData.Ifvalue = exData.Ifvalue.Replace((char)3, GlobalConstant.END_CHAR);

                    exData.Transstarttime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    exData.Transcompletetime = GlobalConstant.SET_MAXTIME;

                    exData.insertExternalIfData();
                }
                else
                {
                    return;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void insertTransferComplete(TransferData tfData)
        {
            try
            {
                WMSCommand fCommand = new WMSCommand(WMSCommand.MESSAGE_TYPE.F);

                //Head
                fCommand.MessageBox = "ECS_MBX";
                fCommand.MessageSize = "10";

                //Body
                fCommand.MessageType = WMSCommand.MESSAGE_TYPE.F;

                if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
                {
                    fCommand.F_JobDefine = EntityEnumData.COMMANDTYPE.INGO.GetHashCode().ToString(); //Unit 입고
                }
                else if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                {
                    fCommand.F_JobDefine = EntityEnumData.COMMANDTYPE.OUTGO.GetHashCode().ToString(); //Unit 출고
                }
                else
                {
                    fCommand.F_JobDefine = EntityEnumData.COMMANDTYPE.MOVE.GetHashCode().ToString(); //Unit 이동
                }

                fCommand.F_LuggageNo = tfData.Palletid;

                if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
                {
                    fCommand.F_CompleteClass = EntityEnumData.COMMANDTYPE.INGO.GetHashCode().ToString(); //Unit 입고
                }
                else if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                {
                    fCommand.F_CompleteClass = EntityEnumData.COMMANDTYPE.OUTGO.GetHashCode().ToString(); //Unit 출고 완료
                }
                else
                {
                    fCommand.F_CompleteClass = EntityEnumData.COMMANDTYPE.MOVE.GetHashCode().ToString(); //Unit 이동 완료
                }

                if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                {
                    fCommand.F_StepCount = "2";            //전체 출고 완료
                }
                else
                {
                    fCommand.F_StepCount = "1";            //일반 완료
                }

                ExternalIfData exData = new ExternalIfData();
                exData.Externaliftype = WMSCommand.MESSAGE_TYPE.F.ToString();
                exData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                exData.Ifvalue = fCommand.BinaryString;

                exData.Ifvalue = exData.Ifvalue.Replace((char)2, GlobalConstant.START_CHAR);
                exData.Ifvalue = exData.Ifvalue.Replace((char)3, GlobalConstant.END_CHAR);

                exData.Transstarttime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                exData.Transcompletetime = GlobalConstant.SET_MAXTIME;

                exData.insertExternalIfData();
            }
            catch (Exception ex)
            {
                throw ex;
            }

        }
    }
}
