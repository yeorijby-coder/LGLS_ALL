using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Device.Observe 
{
    public delegate void DelegateObservableValueChanged(object sender, object value);

    public class ObservableChangeEventWrapper : MarshalByRefObject
    {
        public event DelegateObservableValueChanged WrappedObservableValueChanged = null;

        public void WrapperObservableValueChanged(object sender, object value)
        {
            if (WrappedObservableValueChanged != null)
            {
                WrappedObservableValueChanged(sender, value);
            }
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }

        public delegate void DelegateObservableEvent(object sender);

    }

    public delegate void DelegateObservableEvent(object sender);

}
