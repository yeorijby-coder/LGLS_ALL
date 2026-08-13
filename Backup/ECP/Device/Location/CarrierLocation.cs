using System;
using System.Collections.Generic;
using System.Text;

using System.Reflection;
using System.Runtime.Remoting;

using ECP.Service.Material;
using ECP.Db;
using ECP.Global.Device;
using ECP.Global.Exception;
using ECP.Service.Logger;

namespace ECP.Device.Location
{
    public class CarrierLocation : MaterialLocation
    {
        private const string CLASS = "CarrierLocation";

        public new static string className = "ECP.Device.Location.CarrierLocation";
        public new static string assemblyName = "ECP";


        #region Attribute_Not_Persistance

        private string carrierId;
        public string CarrierId
        {
            get { return this.carrierId; }
            set { this.carrierId = value; this.materialId = value; }
        }
        #endregion Attribute_Not_Persistance

        #region Constructor

        public static CarrierLocation CreateCarrierLocation(string ownerId, ElementType ownerType, string locationId,
           LocationLogicalState logicalState)
        {
            const string METHOD = CLASS + ".CreateCarrierLocation";
            CarrierLocation location = null;  
            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                location = (CarrierLocation)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "carrier location", "failed to create carrier location : " + locationId +
                    "\n" + ex.Message.ToString());
            }

            location.locationId = locationId;
            location.locationType = LocationType.CarrierLocation;
            location.logicalState = logicalState;

            
            location.ownerId = ownerId;
            
      
            location.Add();

            return location;
        }

        #endregion Constructor

        #region Method

        /// <summary>
        /// 현재 위치의 Carrier ID 제거 
        /// </summary>
        public void RemoveCarrier()
        {
            this.CarrierId = String.Empty;
        }

        public void AddCarrier(string carrierId)
        {
            if(String.IsNullOrEmpty(carrierId))
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE, "", "carrier id has invalid value");
            }
            else
            {
                this.CarrierId = carrierId;
            }
        }

        /// <summary>
        /// 현재 Location에 Carrier가 존재하는지 여부를 판단하여 결과 판단.
        /// </summary>
        /// <returns>true: 있음</returns>
        public bool IsCarrierExist()
        {
            if (String.IsNullOrEmpty(this.carrierId))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        /// <summary>
        /// 현재 위치의 CarrierID 
        /// </summary>
        /// <returns></returns>
        public string GetCarrier()
        {
            if (String.IsNullOrEmpty(this.carrierId))
            {
                return String.Empty;
            }
            else
            {
                return this.carrierId;
            }
        }
        #endregion Method

    }
}
