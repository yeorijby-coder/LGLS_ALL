using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global.Device;

namespace ECP.Service.Alarm
{
    /// <summary>
    /// Alarm의 기본형 개체로써 ID, Text, Category등의 정보를 담음
    /// </summary>
     public class EcpAlarm
    {
   
        #region Attribute

        /// <summary>
        /// Equipment Type
        /// </summary>
        private EquipmentType equipmentType;
         public EquipmentType EQUIPMENTTYPE
        {
            get { return equipmentType; }
            set { equipmentType = value; }
        }

        /// <summary>
        /// Alarm ID
        /// </summary>
         private long alarmId;
         public long ALARMID
        {
            get { return alarmId; }
            set { alarmId = value; }
        }

        /// <summary>
        /// Alarm Category
        /// </summary>
         private byte alarmCd;
         public byte ALARMCD
        {
            get { return alarmCd; }
            set { alarmCd = value; }
        }

        /// <summary>
        /// Alarm Text
        /// </summary>
        private string alarmText;
        public string ALARMTEXT
        {
            get { return alarmText; }
            set { alarmText = value; }
        }

        /// <summary>
        /// ALARM LEVEL : HEAVY,RIGHT
        /// </summary>
        private string level;
        public string LEVEL
        {
            get { return level; }
            set { level = value; }
        }

        ///// <summary>
        ///// Alarm ID
        ///// </summary>
        //private long alid;
        //public long ALID
        //{
        //    get { return alid; }
        //    set { alid = value; }
        //}

        ///// <summary>
        ///// Alarm Category
        ///// </summary>
        //private byte alcd;
        //public byte ALCD
        //{
        //    get { return alcd; }
        //    set { alcd = value; }
        //}

        ///// <summary>
        ///// ALARM TYPE : HEAVY,RIGHT
        ///// </summary>
        //private string type;
        //public string Type
        //{
        //    get { return type; }
        //    set { type = value; }
        //}


        ///// <summary>
        ///// Alarm Text
        ///// </summary>
        //private string altx;
        //public string ALTX
        //{
        //    get { return altx; }
        //    set { altx = value; }
        //}

        ///// <summary>
        ///// Alarm Couse
        ///// </summary>
        //private string alcs;
        //public string ALCS
        //{
        //    get { return alcs; }
        //    set { alcs = value; }
        //}

        ///// <summary>
        ///// Alarm Enable
        ///// </summary>
        //private byte aled;
        //public byte ALED
        //{
        //    get { return aled; }
        //    set { aled = value; }
        //}
        ///// <summary>
        ///// Host에 보고할 대상의 알람인지
        ///// </summary>
        ///// 
        //private bool isHostReport;        
        //public bool IsHostReport
        //{
        //    get { return isHostReport; }
        //    set { isHostReport = value; }
        //}

        ///// <summary>
        ///// Alarm Subsystem
        ///// </summary>
        //private SubSystemType subSystemType;

        //public SubSystemType SubSystemType
        //{
        //    get { return subSystemType; }
        //    set { subSystemType = value; }
        //}

        #endregion Attribute


    }
}
