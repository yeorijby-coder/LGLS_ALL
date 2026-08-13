using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Service.Logger
{
    [Serializable]
    public class Category
    {
        private int _value;
        private static int _size = 4;

        private static Category[] _array = new Category[_size];
        public static String[] displayStrings = new String[4];

        public const int _COM = 0;
        public static Category COM = new Category(_COM);

        public const int _DEBUG = 1;
        public static Category DEBUG = new Category(_DEBUG);

        public const int _ERROR = 2;
        public static Category ERROR = new Category(_ERROR);

        public const int _INFO = 3;
        public static Category INFO = new Category(_INFO);

        public static Category IT_ENUM_MAX = new Category(int.MaxValue);

        static Category()
        {
            displayStrings[0] = "COM";
            displayStrings[1] = "DEBUG";
            displayStrings[2] = "ERROR";
            displayStrings[3] = "INFO";
        }


        public override string ToString()
        {
            return displayStrings[_value];
        }


        public int Value()
        {
            return _value;
        }

        public static Category From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
                return (IT_ENUM_MAX);
        }

        protected Category(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
                _array[_value] = this;
        }
        public bool Equals(Category otherEnum)
        {
            return (otherEnum.Value() == _value);
        }
    }
}
