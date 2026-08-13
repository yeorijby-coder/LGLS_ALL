using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global
{
    public class PropertyInfo
    {
        const string CLASS = "PropertyInfo";


        public static int PROPERTY_TYPE_STRING = 0;

        public static int PROPERTY_TYPE_NUMBER = 1;

        public static int PROPERTY_TYPE_IMAGE = 2;

        public static int PROPERTY_TYPE_COLOR = 3;

        public static int PROPERTY_TYPE_FONT = 4;

        public static int PROPERTY_TYPE_BOOLEAN = 5;
     

        protected int type;

        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        protected string name;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        protected string defaultValue;

        public string DefaultValue
        {
            get { return defaultValue; }
            set { defaultValue = value; }
        }

        public PropertyInfo(string name, int type, string defaultValue)
        {
            this.name = name;
            this.type = type;
            this.defaultValue = defaultValue;
        }
    }
}
