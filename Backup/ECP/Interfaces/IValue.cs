
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace ECP.Interfaces
{
    [Serializable]
    public delegate void DelegateValueChanged(object sender, object value);

    public class ValueChangeEventWrapper : MarshalByRefObject
    {
        public event DelegateValueChanged WrappedOnValueChanged = null;

        public void WrappedValueChanged(object sender, object value)
        {
            if (WrappedOnValueChanged != null)
            {
                WrappedOnValueChanged(sender, value);
            }
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }
    }

    public interface IValue
    {
        object Value
        {
            get;
            set;
        }

        int SetValue(object oValue);
        event DelegateValueChanged OnValueChanged;
    }
}
