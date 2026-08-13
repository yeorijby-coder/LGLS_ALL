using System;
using System.Collections.Generic;
using System.Text;
using ECP.Device.Observe;

namespace HECS.Device.Unit
{
    [Serializable]
    public partial class Rack
	{

		#region Properties for Observable Access
		public Observable @__IO_CSTID
		{
			get
			{
				return this.Observables["CSTID"];
			}
		}
		#endregion

		#region Properties for Observable Access By Value

		public string IO_CSTID
		{
			get
			{
				return this.Observables["CSTID"].AsString;
			}
			set
			{
				this.Observables["CSTID"].AsString = value;
			}
		}
		#endregion
	}
}
