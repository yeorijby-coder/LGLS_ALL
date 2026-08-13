using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using System.Data.SqlClient;
using ECP.Service.Logger;

namespace ECP.Service.Route
{
    /// <summary>
    /// Itineraray 관리 Class
    /// </summary>
    public class Itineraray : Persistence
    {
        const string CLASS = "Itineraray";

        public enum ItinerarayStatus
        { 
            Online = 1,
            Offline = 2
        }

        public enum MoveStartFlag
        {
            Y = 1,
            N = 2
        }

        public Itineraray()
        { 

        }

        /// <summary>
        /// Itinarary ID
        /// </summary>
        private string _pathId;

        public string PathId
        {
            get { return _pathId; }
            set { _pathId = value; }
        }

        /// <summary>
        /// 출발 Device
        /// </summary>
        private string _fromDeviceId;

        public string FromDeviceId
        {
            get { return _fromDeviceId; }
            set { _fromDeviceId = value; }
        }

        /// <summary>
        /// 목적지 Device
        /// </summary>
        private string _toDeviceId;

        public string ToDeviceId
        {
            get { return _toDeviceId; }
            set { _toDeviceId = value; }
        }

        /// <summary>
        /// Itinarary 우선 순위
        /// </summary>
        private string _priority;

        public string Priority
        {
            get { return _priority; }
            set { _priority = value; }
        }

        /// <summary>
        /// Itinarary 상태
        /// </summary>
        private ItinerarayStatus _status;

        public ItinerarayStatus Status
        {
            get { return _status; }
            set { _status = value; }
        }

        /// <summary>
        /// 목적지 Location
        /// </summary>
        private string _toLocationId;

        public string ToLocationId
        {
            get { return _toLocationId; }
            set { _toLocationId = value; }
        }

        /// <summary>
        /// 출발지 출구 Device
        /// </summary>
        private string _fromLocationId;

        public string FromLocationID
        {
            get { return _fromLocationId; }
            set { _fromLocationId = value; }
        }

        /// <summary>
        /// 시작 Device Flag
        /// </summary>
        /// <summary>
        /// Itinarary 상태
        /// </summary>
        private MoveStartFlag _startFlag;

        public MoveStartFlag StartFlag
        {
            get { return _startFlag; }
            set { _startFlag = value; }
        }

        ///// <summary>
        ///// Itinarary ID
        ///// </summary>
        //private string _iD;

        //public string ID
        //{
        //    get { return _iD; }
        //    set { _iD = value; }
        //}
        ///// <summary>
        ///// 출발 Device
        ///// </summary>
        //private string _OriginDevice;

        //public string OriginDevice
        //{
        //    get { return _OriginDevice; }
        //    set { _OriginDevice = value; }
        //}

        ///// <summary>
        ///// 목적지 Device
        ///// </summary>
        //private string _DestinationDevice;

        //public string DestinationDevice
        //{
        //    get { return _DestinationDevice; }
        //    set { _DestinationDevice = value; }
        //}

        ///// <summary>
        ///// Itinarary 우선 순위
        ///// </summary>
        //private string _Priority;

        //public string Priority
        //{
        //    get { return _Priority; }
        //    set { _Priority = value; }
        //}

        ///// <summary>
        ///// Itinarary 상태
        ///// </summary>
        //private ItinerarayStatus _Status;

        //public ItinerarayStatus Status
        //{
        //    get { return _Status; }
        //    set { _Status = value; }
        //}

        ///// <summary>
        ///// 목적지 입구 Device
        ///// </summary>
        //private string _Destination_Entry_LocationID;

        //public string Destination_Entry_LocationID
        //{
        //    get { return _Destination_Entry_LocationID; }
        //    set { _Destination_Entry_LocationID = value; }
        //}

        ///// <summary>
        ///// 출발지 출구 Device
        ///// </summary>
        //private string _Origin_Exit_LocationID;

        //public string Origin_Exit_LocationID
        //{
        //    get { return _Origin_Exit_LocationID; }
        //    set { _Origin_Exit_LocationID = value; }
        //}

        public static Dictionary<string, Itineraray> LoadItineraries()
        {
            const string METHOD = CLASS + ".LoadItineraries";

            Dictionary<string, Itineraray> itineraries = new Dictionary<string, Itineraray>();

            Itineraray itineraray = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "SELECT *  FROM TB_ITINERARY ";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    itineraray = new Itineraray();

                    itineraray._pathId = (string)reader["PATHID"];
                    itineraray._fromDeviceId = reader["FROMDEVICEID"].Equals(DBNull.Value) ? String.Empty : (string)reader["FROMDEVICEID"];
                    itineraray._fromLocationId = reader["FROMLOCATIONID"].Equals(DBNull.Value) ? String.Empty : (string)reader["FROMLOCATIONID"];
                    itineraray._toDeviceId = reader["TODEVICEID"].Equals(DBNull.Value) ? String.Empty : (string)reader["TODEVICEID"];
                    itineraray._toLocationId = reader["TOLOCATIONID"].Equals(DBNull.Value) ? String.Empty : (string)reader["TOLOCATIONID"];

//LGLS 주석 처리(2009.12.28)
                    //itineraray._priority = reader["PRIORITY"].Equals(DBNull.Value) ? String.Empty : (string)reader["PRIORITY"];
                    //itineraray._status = reader["STATUS"].Equals(DBNull.Value) ? ItinerarayStatus.Offline : (((string)reader["STATUS"]).Equals(ItinerarayStatus.Offline.ToString()) ? ItinerarayStatus.Offline : ItinerarayStatus.Online);
                    
                    itineraray._startFlag = reader["STARTFLAG"].Equals(DBNull.Value) ? MoveStartFlag.N : (((string)reader["STARTFLAG"]).Equals(MoveStartFlag.N.ToString()) ? MoveStartFlag.N : MoveStartFlag.Y);
                }
                catch (SqlException ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, CLASS, METHOD, "failed to load ITINERARIES because SQL Error : \n" + ex.Message.ToString());
                    throw ex;
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, CLASS, METHOD, "failed to load ITINERARIES : \n" + ex.Message.ToString());
                    throw ex;
                }

                itineraries.Add(itineraray._pathId, itineraray);

            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return itineraries;

        }

        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);


        }
        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }

        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

      
    }
}
