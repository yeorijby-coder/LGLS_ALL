/// <summary>
/// 
/// </summary>
/// <filename></filename>
/// <version>1.0.0.0</version>
/// <modifications>
/// 	1.  ver 1.0.0.0     2006-00-00 00:00:00     Jongseok Song   -
///         : Created.
/// </modifications>
/// <copyright>Copyright (c) 2005~2007. EzControl, LG CNS All rights reserved.</copyright>
/// 

using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Com.FieldBus
{
    #region Interface

    public interface ICnnStrEditor
    {
        string ConnString
        {
            get;
            set;
        }

        string GetConnectionString();
        
        void SetConnectionString(string strConnString);
    }

    #endregion Interface

    #region Method
    /// <summary>
    /// 
    /// </summary>
    public class GenericConnectionString : Dictionary<string, object>
    {
        private const char DEFAULT_DELIMETER_01 = ',';
        private const char DEFAULT_DELIMETER_02 = '=';

        private char primeDelimeter = DEFAULT_DELIMETER_01;
        private char PrimaryDelimeter
        {
            get { return this.primeDelimeter; }
            set { this.primeDelimeter = value; }
        }

        private char secondaryDelimeter = DEFAULT_DELIMETER_02;
        private char SecondaryDelimeter
        {
            get { return this.secondaryDelimeter; }
            set { this.secondaryDelimeter = value; }
        }

        private string connectionString = string.Empty;
        public string ConnectionString
        {
            get { return this.connectionString; }
        }

        public GenericConnectionString()
        {
        }

        public GenericConnectionString(string strConnection)
        {
            Parse(strConnection);
        }
        public virtual short this[string key, short defaultValue]
        {
            get 
            {
                return this.ContainsKey(key) ? Convert.ToInt16(this[key]) : defaultValue; 
            }
        }

        public virtual int this[string key, int defaultValue]
        {
            get
            {
                return this.ContainsKey(key) ? Convert.ToInt32(this[key]) : defaultValue;
            }
        }

        public virtual int this[string key, int defaultValue, int iBase]
        {
            get 
            {
                return this.ContainsKey(key) ? Convert.ToInt32(this[key].ToString(), iBase) : defaultValue; 
            }
        }

        public virtual string this[string key, string defaultValue]
        {
            get 
            {
                return this.ContainsKey(key) ? this[key].ToString() : defaultValue; 
            }
        }

        public virtual DateTime this[string key, DateTime defaultValue]
        {
            get 
            {
                return this.ContainsKey(key) ? Convert.ToDateTime(this[key]) : defaultValue; 
            }
        }

        public void Parse(string strConnectionString)
        {
            this.connectionString = strConnectionString;

            string[] sItemList = strConnectionString.Split(this.primeDelimeter);

            string strName = null;
            string strValue = null;

            Clear();

            foreach (string sItem in sItemList)
            {
                string[] sProperty = sItem.Split(this.secondaryDelimeter);

                if (sProperty.Length != 2)
                {
                    continue;
                }

                strName = sProperty[0].Trim().ToUpper();
                strValue = sProperty[1].Trim();

                Add(strName, GetValue(strName, strValue));
            }
        }

        virtual protected object GetValue(string strName, string strValue)
        {
            return strValue;
        }
    }
    #endregion Method

}
