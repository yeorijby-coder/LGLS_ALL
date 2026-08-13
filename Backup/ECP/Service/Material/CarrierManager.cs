#define LOCK_DEGUB
using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using ECP.Service.Logger;
using ECP.Global.Device;
using System.Runtime.Remoting;
using System.Security.Permissions;
using ECP.Service.Statistics;

namespace ECP.Service.Material
{
    [HostProtectionAttribute(Synchronization = true, ExternalThreading = true)]
    public class CarrierManager : Manager
    {

        private const string CLASS = "CarrierManager";
        public object Collection_Monitor = new object();

        public Dictionary<string, Carrier> carriers = new Dictionary<string, Carrier>();

        public CarrierManager(string ownerId, string id)
            : base(ownerId, id)
        {
            this.loadCarriers();
        }

        public void loadCarriers()
        {
            return;
            this.carriers = Carrier.LoadCarriers();
        }
        public void loadCarrier(string carrierId)
        {
            return ;
            Carrier carrier = Carrier.LoadCarrier(carrierId);

            if(!this.carriers.ContainsKey(carrierId))
            {
                this.carriers.Add(carrierId, carrier);
            }
        }

        public void CreateCarrier(string carrierId)
        {
        }


        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }

        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }

        #region Method

        public void DeleteCarrier(string carrierId)
        {
        }


        public Carrier[] GetCarrierCopyAll()
        {
            Carrier[] carrierArray = new Carrier[carriers.Count];
            try
            {
                carriers.Values.CopyTo(carrierArray, 0);
            }
            catch(Exception ex)
            {
                Log.log(5, "GetCarrierCopyAll", "debug", ECP.Service.Logger.Category.DEBUG, "", "GetCarrierCopyAll", ex.Message.ToString());
            }
            return carrierArray;
        }

        public Dictionary<string, Carrier> GetCarrierAll()
        {
            return carriers;
        }

        /// <summary>
        /// Carrier Mannager로 부터 Carrier를 찾아옴. 
        /// </summary>
        /// <param name="carrierId"></param>
        /// <returns></returns>
        public Carrier GetCarrier(string carrierId)
        {
            if (string.IsNullOrEmpty(carrierId))
            {
                return null;
            }

            Carrier carrier = null;

            try
            {
                if (this.carriers.ContainsKey(carrierId))
                {
                    return carriers[carrierId];
                }
                else
                {
                InvalidOperation:
                    try
                    {
                        foreach (Carrier cst in this.carriers.Values)
                        {
                            if (cst.Carrierid == carrierId )
                            {
                                return cst;
                            }
                        }
                    }
                    catch (InvalidOperationException)
                    {
                        Log.log(5, "GetCarrierUnsafe", "debug", Log.DEBUG, "CarrierManager", "InvalidOperationException", "정상적인 Exception 임");
                        goto InvalidOperation;
                    }
                }
                return carrier;
            }
            catch (Exception)
            {
                return carrier;
            }
        }


        /// <summary>
        /// 등록된 Carrier들의 마지막 3자리로 Key Table을 만듬 (새로운 Key와 중복 되지 않기 위함) 
        /// LiftController에서 사용함. 
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, string> GetKeyTable()
        {
            int creatSequenceIdLength = 3;
            Dictionary<string, string> keyTable = new Dictionary<string, string>();

            lock(Collection_Monitor)
            {
                foreach(string key in this.carriers.Keys)
                {
                    if(key.Length > creatSequenceIdLength)
                    {
                        try
                        {
                            //마지막 Carrier ID 3자리를 가져 옴.
                            string seqkey = key.Substring(key.Length - creatSequenceIdLength, creatSequenceIdLength);
                            if(!keyTable.ContainsKey(seqkey))
                            {
                                keyTable.Add(seqkey, seqkey);
                            }
                        }
                        catch(Exception ex)
                        {
                            Log.log(5, "GetKeyTable", "debug", Log.DEBUG, "GetKeyTable", "", ex.ToString());
                        }
                    }
                    else // Key로 쓸 자리수 보다 작은 Carrier ID는 통채로 Key로 씀.
                    {
                        string seqkey = key;
                        if(!keyTable.ContainsKey(seqkey))
                        {
                            keyTable.Add(seqkey, seqkey);
                        }
                    }
                }
            }

            return keyTable;
        }
 
        /// <summary>
        /// Carrier Mannager로 부터 Carrier를 찾아옴
        /// 주의 Collection_Monitor를 사용하므로 Lock 중첩을 피해야 함.
        /// </summary>
        /// <param name="carrierId64"></param>
        /// <returns></returns>
        public Carrier GetCarrier64(string carrierId64)
        {
            return GetCarrier(carrierId64);
        }

        public Carrier GetCarrierByLocation(string elementID)
        {
            return GetCarrierByLocation(elementID, "1");
        }

        public Carrier GetCarrierByLocation(string elementID, string subID)
        {

                return null;

        }

        public string FindByLoc(string elementID)
        {
           
                return "";
            
        }


        /// <summary>
        ///  요청한 Carrier ID가 Carrier Manager 내에 존재 하는지 확인
        /// </summary>
        /// <param name="carrierId">중복을 체크할 CarrierID</param>
        /// <returns>TRUE: 존재 함., FALSE : 존재 하지 않음. </returns>
        public bool isExistCarrerId(string carrierId)
        {
            const string METHOD = CLASS + ".isExistCarrerId";

            return false;
        }

        public bool Find(string carrierID)
        {
            return false;
        }


        /// <summary>
        /// Location에 존재하는 Carrier 개수
        /// </summary>
        /// <param name="elementId">Loaction elementId</param>
        public int GetExistCarrierCountInLocation(string elementId)
        {
            int cnt = 0;


            return cnt;
        }

        public void CreateCarrier()
        {

            const string METHOD = CLASS + ".CreateCarrier";

        }

        /// <summary>
        /// 특정 Location에 있는 Carrier의 ID를 가져 옴.
        /// </summary>
        /// <param name="elementId"></param>
        /// <param name="subLocation"></param>
        /// <returns></returns>
        public CarrierIdInfo GetCarrierIdByLocation(string elementId, string subLocation)
        {
            return new CarrierIdInfo();
        }

        /// <summary>
        /// Carrier의 총 개수를 알려 줌.
        /// </summary>
        /// <returns></returns>
        public int GetCarrierCount()
        {
            return carriers.Count;
        }

        #endregion Method


    }
}
