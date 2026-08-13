using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global.Exception;

namespace ECP.Service.Logger
{
    /// <summary>
    /// Binary Format으로 Logging된 Message을 사람이 해독가능한
    /// Redable Format으로 변환 되어야 하며, 이 Class는 변환기 임
    /// </summary>
    public class GenericParser
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = ".GenericParser";

        /// <summary>
        /// Message의 Raw Data
        /// </summary>
        protected byte[] data;

        /// <summary>
        /// Parser의 Type이 Secs Format인 경우
        /// </summary>
        static readonly string SECS_TYPE = "Secs";

        /// <summary>
        /// Parser의 Type이 Hexa Format인 경우
        /// </summary>
        static readonly string HEX_TYPE = "Hex";

        /// <summary>
        /// 기본 생성자
        /// </summary>
        public GenericParser()
        {
        }

        /// <summary>
        /// date byte array로 GenericParser생성
        /// </summary>
        /// <param name="data"></param>
        public GenericParser(byte[] data)
        {
            this.data = data;

        }

        /// <summary>
        /// Parser에 data byte array를 설정
        /// </summary>
        /// <param name="data">logging하고자 하는 data byte array</param>
        public void SetData(byte[] data)
        {
            this.data = data;
        }

        /// <summary>
        /// 가능한 Parse Format을 Return
        /// </summary>
        /// <returns></returns>
        public string[] GetParserTypes()
        {
            string[] pTypes = { HEX_TYPE, SECS_TYPE };
            return pTypes;
        }

        /// <summary>
        /// Parser Type(Secs, Hex)에 따라 binary Data를 Parcing한 결과를 Return
        /// </summary>
        /// <param name="parserType">Parser Type(Secs, Hex)</param>
        /// <returns>arser Type(Secs, Hex)에 따라 binary Data를 Parcing한 결과를 Return</returns>
        public string GetParsedData(string parserType)
        {
            // check for null string
            if (parserType == null)
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE,
                    parserType, "Invalid Parser Type");
            }

            if (parserType.Equals(HEX_TYPE))
            {
                return (GetHexData());
            }

            if (parserType.Equals(SECS_TYPE))
            {
               // return (getSecsData());
            }

            // invalid parser type
            throw new ECPException(ECPExceptionType.INVALID_VALUE,
                           parserType, "Invalid Parser Type");
        }
        /// <summary>
        /// Binary Message을 Hex Format으로 변환후 Return
        /// </summary>
        /// <returns>Binary Message을 Hex Format으로 변환한 String</returns>
        protected string GetHexData()
        {
            StringBuilder sb = new StringBuilder();
            //int b;

            //for (int i = 0; i < data.Length; i++)
            //{
            //    if (i != 0 && i % 20 == 0)
            //    {
            //        sb.Append("\n");
            //    }
            //    b = data[i] & OxFF;
            //    if (b < 16)
            //    {
            //        sb.Append("0");
            //    }
            //    b.append(Convert.toh.toHexString(b) + " ");
            //}

            return sb.ToString();           

        }

        /// <summary>
        /// Binary Message을 Secs Format으로 변환 후 Return
        /// </summary>
        /// <returns>Binary Message을 Secs Format으로 변환한 String</returns>
        protected string GetSecsData()
        {
            string value=null;
            return value;
        }
    }

}
