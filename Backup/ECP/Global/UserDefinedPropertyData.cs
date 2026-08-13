using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global
{
    [Serializable]
    public class UserDefinedPropertyData
    {
        public string entityId = null;

        
        public UserDefinedPropertyInfo [] propertyList = null;

        public UserDefinedPropertyData(string entityId, UserDefinedPropertyInfo[] propertyList)
        {
            this.entityId = entityId;
            this.propertyList = propertyList;
        }


    }

    [Serializable]
    public class UserDefinedPropertyInfo : ICloneable
    {
        public string name = null;

        public string value = null;


        public UserDefinedPropertyInfo()
        {
        }

        public UserDefinedPropertyInfo(string name, string value)
        {
            this.name = name;
            this.value = value;
        }

        public object Clone()
        {
            return MemberwiseClone();
        }
        
    }
}
