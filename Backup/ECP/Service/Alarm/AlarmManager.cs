using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using ECP.Service.Logger;

namespace ECP.Service.Alarm
{
    /// <summary>
    /// Alarm Manage Class
    /// Alarm의 등록, 삭제, 조회, 상태 변경에 대한 기능을 제공하는 Class 임.
    /// </summary>
    public class AlarmManager : Manager
    {
        /// <summary>
        ///  Class Name을 담고 이는 property
        /// </summary>
        private const string CLASS = "AlarmManager";

        /// <summary>
        /// 등록된 전체의 Alarm 정보를 가지고 있는 Dictionary 
        /// </summary>
        private Dictionary<long, EcpAlarm> alarms = new Dictionary<long, EcpAlarm>();

        public AlarmManager(string ownerId, string id)
            : base(ownerId, id)
        {
            this.loadAlarms();
        }

        /// <summary>
        /// 등록된 SubSystem들의 Alarm을 메모리로 Loading 함.
        /// </summary>
        public void loadAlarms()
        {
            this.alarms = SubSystemAlarm.LoadSubSystemAlarms();
        }

        /// <summary>
        /// Alarm의 ID로 등록된 Alarm의 내용(Alarm Text)를 알아 냄.
        /// </summary>
        /// <param name="alarmId">조회하고자하는 Alarm ID</param>
        /// <returns>알람 내용</returns>
        public string GetAlarmText(long alarmId)
        {
            return alarms[alarmId].ALARMTEXT;
        }


        /// <summary>
        /// 등록된 Alarm의 상태를 Set 상태와 Reset 상태로 변경 관리 함.
        /// </summary>
        /// <param name="alarmId">상태를 변경하고자 하는 Alarm ID</param>
        /// <param name="isAlarmSet">true : 상태를 Set 상태로 변경</param>
        public void AlarmSetReset( long alarmId, bool isAlarmSet)
        {
            EcpAlarm alarm = null;

            if (!alarms.ContainsKey(alarmId))
            {
                return;
            }
            alarm = alarms[alarmId];

            byte alarmSetReset;

            if (isAlarmSet == true)
            {
                alarmSetReset = (byte)(0x80 | alarm.ALARMCD);
            }
            else
            {
                alarmSetReset = (byte)(0x7F & alarm.ALARMCD);
            }

            if (alarmSetReset != alarm.ALARMCD)
            {
                alarm.ALARMCD = alarmSetReset;
            }

        }

        /// <summary>
        ///  등록된 특정 알람의 객체
        /// </summary>
        /// <param name="alarmId"></param>
        /// <returns></returns>
        public EcpAlarm GetAlarm(long alarmId)
        {
            if (!alarms.ContainsKey(alarmId))
            {
                return null;
            }

            return alarms[alarmId];
        }


        /// <summary>
        /// 총 등록된 Alarm의 수
        /// </summary>
        /// <returns>Alarm 개수</returns>
        public int GetTotalAlarmCount()
        {
            return alarms.Count;
        }

        /// <summary>
        /// Set 상태의 알람 리스트
        /// </summary>
        /// <returns>Set된 알람 List</returns>
        public Dictionary<long, EcpAlarm> GetSetedAlarmList()
        {
            Dictionary<long, EcpAlarm> setedAlarms = new Dictionary<long, EcpAlarm>();

            foreach (long key in alarms.Keys)
            {
                if (alarms[key].ALARMCD >= 0x80)
                {
                    setedAlarms.Add(key, alarms[key]);
                }

            }
            return  setedAlarms;
        }

        /// <summary>
        /// ReSet 상태의 알람 리스트
        /// </summary>
        /// <returns>ReSet 상태의 알람 리스트</returns>
        public Dictionary<long, EcpAlarm> GetResetedAlarmList()
        {
            Dictionary<long, EcpAlarm> ResetedAlarms = new Dictionary<long, EcpAlarm>();

            foreach (long key in alarms.Keys)
            {
                if (alarms[key].ALARMCD <= 0x7F)
                {
                    ResetedAlarms.Add(key, alarms[key]);
                }

            }
            return ResetedAlarms;
        }

        /// <summary>
        /// 모든 Alarm을 초기화 Reset 상태로 만듬
        /// </summary>
        public void ResetAllAlarm()
        {
            foreach (long key in alarms.Keys)
            {
                alarms[key].ALARMCD = (byte)0x7F;
            }

        }

        /// <summary>
        /// Alarm 사용 가불 정보 변경
        /// </summary>
        /// <param name="alarmId"></param>
        /// <param name="isEnable"></param>
        public void AlarmEnableStatusChange(long alarmId, bool isEnable)
        {
            //if (!alarms.ContainsKey(alarmId))
            //{
            //    return;
            //}
            //EcpAlarm alarm = alarms[alarmId];
            //if (alarm != null)
            //{
            //    return;
            //}
            //if (isEnable == true)
            //{
            //    alarm.ALED = (byte)0x80;
            //}
            //else
            //{
            //    alarm.ALED = (byte)0x00;
            //}
        }

        /// <summary>
        /// 전체 알람의 사용가불을 변경 함.
        /// </summary>
        /// <param name="isEnable"></param>
        public void AlarmAllEnableStausChange(bool isEnable)
        {
            //foreach (int key in alarms.Keys)
            //{
            //    if (isEnable == true)
            //    {
            //        alarms[key].ALED = (byte)0x80;
            //    }
            //    else
            //    {
            //        alarms[key].ALED = (byte)0x00;
            //    }
            //}
        }


        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }

        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }
 
    }
 
 
}
