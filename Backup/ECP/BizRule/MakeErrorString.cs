using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db.Entity;
using System.Data.SqlClient;
using ECP.Global;
using ECP.Db;
using ECP.Util;

namespace ECP.BizRule
{
    [Serializable]
    public class MakeErrorString : ECPObject
    {
        private string elementid = "";

        public string Elementid
        {
            get { return elementid; }
            set { elementid = value; }
        }
        private string errorcode = "";

        public string Errorcode
        {
            get { return errorcode; }
            set { errorcode = value; }
        }
        private string deviceclass = "";

        public string Deviceclass
        {
            get { return deviceclass; }
            set { deviceclass = value; }
        }
        private string deviceNo = "";

        public string DeviceNo
        {
            get { return deviceNo; }
            set { deviceNo = value; }
        }

        public MakeErrorString(string elementid, string errorcode)
        {
            string errorCode = "";
            if (elementid == "VEHICLE:1")
            {
                errorCode = GlobalConstant.RGV_ERR_PREFIX + errorcode;
            }
            else
            {
                errorCode = GlobalConstant.SC_ERR_PREFIX + errorcode;
            }
            this.Elementid = elementid;
            this.Errorcode = errorCode;
        }

        public string createErrorString()
        {
            CodeMasterData codeData = new CodeMasterData();
            codeData.getErrorInfo(this.Errorcode);

            if (!string.IsNullOrEmpty(codeData.Code))
            {
                switch (this.Elementid)
                {
                    case "VEHICLE:1":
                        this.Deviceclass = "4";
                        this.DeviceNo = "000";
                        break;
                    case "VEHICLE:11":
                    case "VEHICLE:12":
                    case "VEHICLE:13":
                    case "VEHICLE:14":
                    case "VEHICLE:15":
                        this.Deviceclass = "1";
                        this.DeviceNo = "00" + this.Elementid.Substring(9, 1);
                        break;
                    case "CONVEYOR:11":
                        this.Deviceclass = "2";
                        this.DeviceNo = "101";
                        break;
                    case "CONVEYOR:14":
                    case "CONVEYOR:15":
                        this.Deviceclass = "2";
                        this.DeviceNo = "102";
                        break;
                    case "CONVEYOR:12":
                        this.Deviceclass = "2";
                        this.DeviceNo = "103";
                        break;
                    case "CONVEYOR:13":
                        this.Deviceclass = "2";
                        this.DeviceNo = "104";
                        break;
                }

                if (string.IsNullOrEmpty(this.Deviceclass) && string.IsNullOrEmpty(this.DeviceNo))
                {
                    return "";
                }
                else
                {
                    string strTotal = this.Deviceclass + 
                                      this.DeviceNo + 
                                      codeData.Attribute2 + 
                                      this.Errorcode +
                                      "00000000000";
                    return strTotal;
                }
            }
            else
            {
                return "";
            }
        }
    }
}
