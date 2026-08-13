using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;

namespace ECP.Service.Statistics
{
    [Serializable]
    public abstract class StatisticsCollectorData
    {
        /// <summary>
        /// 통계성 Data의 구분자
        /// </summary>
        protected const string  DELIM ="/";        

        protected string reportedData = "";

        /// <summary>
        /// 통계성 Data로 남겨질 Data의 Format은 이 abstract class을 구현한 하위 Class에서
        /// 결정하게 됨, 남기고자 하는 통계성 Data의 Format을 결정하여 string으로 Return함
        /// </summary>
        /// <example>
        /// public override string GetReportedData()
        /// {
        ///    string sdata1 = "Data a";
        ///    string sdata2 = "Data b";
        ///    string sdata3 = "Data c";
        ///    string sdata4 = "Data d";
        ///    return DELIM +sdata1 +
        ///        DELIM + sdata2 +
        ///        DELIM + sdata3 +
        ///        DELIM + sdata4 ;
        /// } 
        /// </example>
        /// <returns>통계성 Data로 남겨질 Data의 Format형태의 String</returns>
        public abstract string GetReportedData();

        /// <summary>
        /// Low Data의 의미를 알수 있도록 Data 의미를 Log에 남길 수 있도록 하는 함수
        /// 예)   True라는 값을 => True [Down] 등으로 남길 수 있음.
        /// </summary>
        /// <param name="data">Object형 Low Data </param>
        /// <param name="datainfomation">Low Data를 String 형태로 변환한 값을 매칭하는 정보 False=Down,True=Run,,,</param>
        /// <param name="valueDataType">형변환 필요한 Object를 판별 하기 위함</param>
        /// <returns>최종 Log에 남길 Data</returns>
        protected virtual string GetValueInfo(Object data, string datainfomation, string valueDataType)
        {
            string returnValue = "";
            try
            {
                switch (valueDataType)
                {
                    case "Bool Array":
                        {
                            if (data == null)
                                return returnValue = "";

                            StringBuilder str = new StringBuilder();

                            List<bool> newData = (List<bool>)data;

                            for (int i = 0; i < newData.Count; i++)
                            {
                                str.Append((newData[i] == true ? "1" : "0"));
                               // str.Append(newData[i].ToString());
                                if (i != 0)
                                {
                                    if (((i+1) % 4) == 0)
                                    {
                                        str.Append("|");
                                    }
                                }
                            }
                            returnValue = str.ToString();

                        }
                        break;
                    case "Short Array":
                        {
                            if(data == null)
                                return returnValue = "";

                            StringBuilder str = new StringBuilder();

                            List<short> newData = (List<short>)data;

                            for (int i = 0; i < newData.Count; i++)
                            {
                                str.Append(newData[i].ToString());
                                if (i != 0)
                                {
                                    if (((i+1) % 4) == 0)
                                    {
                                        str.Append("|");
                                    }
                                }
                            }
                            returnValue = str.ToString();
                        }
                        break;

                    default:
                        returnValue = data == null ? "" : data.ToString();
                        break;
                }

                if (String.IsNullOrEmpty(datainfomation))
                {
                    return returnValue;
                }
                else
                {
                    datainfomation = datainfomation.Trim();
                    string[] datainfo = datainfomation.Split(',');

                    foreach (string info in datainfo)
                    {
                        string[] valueData = info.Split('=');
                        if (valueData[0].Equals(returnValue))
                        {
                            if (valueData.Length == 1)
                            {
                                return returnValue;
                            }
                            else
                            {
                                return returnValue + "  [" + valueData[1].ToString() + "]";
                            }
                        }
                    }

                }
                return returnValue;
            }
            catch (Exception ex)
            {

                Console.WriteLine("GetValueInfo 메서드" + ex.ToString());
                return returnValue;
            }

        }

/*
        protected virtual string GetValueInfo(string data, string datainfomation)
        {
            try
            {
                if (String.IsNullOrEmpty(datainfomation))
                {
                    return data;
                }
                else
                {
                    datainfomation = datainfomation.Trim();
                    string[] datainfo = datainfomation.Split(',');

                    foreach (string info in datainfo)
                    {
                        string[] valueData = info.Split('=');
                        if (valueData[0].Equals(data))
                        {
                            return data + "  [" + valueData[1].ToString() +"]";
                        }
                    }

                }
                return data;
            }
            catch (Exception ex)
            {

                return data;
            }
        }
 */ 
    }
}
