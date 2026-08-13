using System;
using System.Collections.Generic;
using System.Text;

using System.Reflection;
using System.Runtime.Remoting;

using System.Data;
using System.Data.SqlClient;

using ECP.Service.Logger;
using ECP.Device;
using ECP.Db;
using ECP.Global.Device;
using ECP.Util;
using ECP.Db.Entity;


namespace ECP.Service.Material
{
    /// <summary>
    /// CarrierId 와 CarrierId64를 담는 구조체
    /// </summary>
    public struct CarrierIdInfo
    {
        public CarrierIdInfo(string _carrierId, string _carrierId64)
        {
            carrierId = _carrierId;
            carrierId64 = _carrierId64;
        }
        public string carrierId;

        public string carrierId64;
    }

    public class Carrier : AbstractMaterial
    {
        private const string CLASS = "CARRIER";

        private bool dirtyFlag = true;

        public const string PHBCST = "PHB";

        public static string className = "ECP.Service.Material.Carrier";

        public static string assemblyName = "ECP";

        #region Attribute_Persistance
        //Persist
        private string carrierid;

        public string Carrierid
        {
            get { return carrierid; }
            set { carrierid = value; }
        }

        private PalletData palletData;

        internal PalletData PalletData
        {
            get { return palletData; }
            set { palletData = value; }
        }
        #endregion Attribute_Persistance
        
        #region Constructor

        public Carrier()
        {
        }

        #endregion Constructor

        #region Manage_Persistance_Attribute

        protected override void Add(DBConnection theDB)
        {
            //const string METHOD = CLASS + ".Add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);

            }
            catch(SqlException ex)
            {
                //Log.log(5, METHOD, "error", Category.ERROR, "", "Carrier", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }
        protected override void Delete(DBConnection theDB)
        {
            ///const string METHOD = CLASS + ".Delete";

            try
            {
                base.Delete(theDB);
            }
            catch(SqlException ex)
            {
                //Log.log(5, METHOD, "error", Category.ERROR, "", "Carrier", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
        }
        protected override void Load(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Load";

            SqlDataReader reader = null;
            try
            {
                base.Load(theDB);

                this.PalletData.getPalletData(theDB, this.Carrierid);
            }
            catch(SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "Carrier", "sql error \n " + ex.Message.ToString());
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }
        protected override void Save(DBConnection theDB)
        {
            const string METHOD = CLASS + ".save";

            try
            {
                base.Save(theDB);
            }
            catch(SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "Carrier", "sql error \n " + ex.Message.ToString());
            }
        }

        public void Update()
        {
            this.dirtyFlag = true;
            Save();
        }
        #endregion Manage_Persistance_Attribute

        #region Method

        public static Dictionary<string, Carrier> LoadCarriers()
        {
            const string METHOD = CLASS + ".LoadCarriers";

            Dictionary<string, Carrier> carriers = new Dictionary<string, Carrier>();
            if (ECP.Global.GlobalConstant.START_MODE != "ECS")
            {
                return carriers;
            }
            string carrierId = null;
            ObjectHandle objHandle = null;
            Carrier carrier = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "SELECT palletid from TB_PALLET";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while(reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance(assemblyName, className);
                    carrier = (Carrier)objHandle.Unwrap();
                }
                catch(Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "carrier", "failed to load carriers" + ex.Message.ToString());
                }
                carrierId = Helper.PreventNull((string)reader["PALLETID"]);
                carrier.Load(carrierId);

                carriers.Add(carrierId, carrier);
            }

            if(reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return carriers;
        }

        public static Carrier LoadCarrier(string carrierId)
        {
            const string METHOD = CLASS + ".LoadCarrier";

            ObjectHandle objHandle = null;

            Carrier carrier = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                carrier = (Carrier)objHandle.Unwrap();
            }
            catch(Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "carrier", "failed to load carrier : " + carrierId +
                    "\n" + ex.Message.ToString());
            }
            carrier.Load(carrierId);

            return carrier;

        }

        public static Carrier CreateCarrier(
            string carrierId,
            string carrierID64,
            string carrierLoc,
            string carrierSubLoc,
            string installTime,
            CarrierState carrierState,
            CarrierType carrierType,
            CarrierHold carrierHold,
            CarrierProhibit carrierIHB)
        {
            const string METHOD = CLASS + ".CreateCarrier";
            Carrier carrier = null;

            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                carrier = (Carrier)objHandle.Unwrap();
            }
            catch(Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem",
                    "failed to create carrier : " + carrierId +
                    "\n" + ex.Message.ToString());
            }

            carrier.Add();

            return carrier;
        }

        public void Load(string carrierId)
        {
            this.Carrierid = carrierId;
            this.PalletData = new PalletData();
            Load();
        }

        public void SetCarrierCurrentLocation(string carrierLocation, string carrierSubLocation)
        {
        }

        public void SetCarrierCurrentLocation(string carrierLocation, string carrierSubLocation, CarrierState currState)
        {
        }

        public void SetCarrierState(CarrierState currState)
        {
        }

        /// <summary>
        /// Lifter Carrier 用.
        /// </summary>
        /// <param name="carrierId"></param>
        /// <param name="carrierID64"></param>
        /// <param name="carrierLoc"></param>
        /// <param name="carrierSubLoc"></param>
        /// <param name="installTime"></param>
        /// <param name="carrierState"></param>
        /// <param name="carrierType"></param>
        /// <param name="carrierElevType"></param>
        /// <returns></returns>
        public static Carrier CreateCarrier(string carrierId, string carrierID64, string carrierLoc, string carrierSubLoc,
            string installTime, CarrierState carrierState, CarrierType carrierType, CarrierElevType carrierElevType)
        {
            const string METHOD = CLASS + ".CreateCarrier";
            Carrier carrier = null;

            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                carrier = (Carrier)objHandle.Unwrap();
            }
            catch(Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem",
                    "failed to create carrier : " + carrierId +
                    "\n" + ex.Message.ToString());
            }

           //
            carrier.Add();

            return carrier;
        }

        public Carrier Clone()
        {
            Carrier c = new Carrier();

             return c;
        }

        #endregion Method
    }
}
