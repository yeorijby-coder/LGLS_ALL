using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using System.Data.SqlClient;

using ECP.Db;
using ECP.Service.Logger;
using ECP.Global.Gui;
using ECP.Util;

using ECP.Global;
using ECP.Global.Exception;
using ECP.Util.UDProperty;

namespace ECP.Service.Gui
{

    /// <summary>
    /// GUIManager는 Client PC를 위한 GUI화면에 저장될 Widget의 정보 및 
    /// Widget의 User Defined Property정보들에 대한 서비스를 제공하는
    /// 객체임
    /// 
    /// Client PC는 여러 화면의 Display Section을 가질 수 있으며,
    /// 한 Section은 하나의 Tab에 해당 할 수 있다
    /// 
    /// 한나의 Tab은 한 화면을 표시하면, 화면내에서 표현될 수 있는 
    /// 장비, 라인, 그림들과 같은 GUI Object들을 Widget이라고 
    /// 표현 할 수 있다.
    /// 
    /// 
    /// </summary>
    public class GUIManager : Manager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "GUIManager";

        /// <summary>
        /// Wiget의 User Defined Property를 위한 Prefix
        /// </summary>
        private const string WIDGET_PROPERTIES_CLASS = "GUIOBJECT";

        /// <summary>
        /// Client GUI화면의 Tab화면을 위한 Container
        /// </summary>
        private Dictionary<string, TabData> tabDataList = new Dictionary<string, TabData>();

        /// <summary>
        /// Client GUI화면의 Tab화면을 위한 Container
        /// </summary>
        public Dictionary<string, TabData> TabDataList
        {
            get
            {
                return tabDataList;
            }
            set
            {
                tabDataList = value;
            }
        }

        /// <summary>
        /// 주어진 OwnerId(ProcessId)와 ManagerId로 GUIManager를 생성한다.
        /// Manager생성시 Layout을 Database로부터 Load
        /// </summary>
        /// <param name="ownerId">GUIManager의 OwnerId(Process, Server)</param>
        /// <param name="id">GUIManager의 Id</param>
        public GUIManager(string ownerId, string id)
            : base(ownerId, id)
        {
            this.LoadLayout();

        }

        /// <summary>
        /// 모든 Tab의 모든 Layout를 Database로부터 Load하기 위하여 사용되어짐
        /// </summary>
        protected void LoadLayout()
        {
            DBConnection theDB = DBControl.GetConnection();

            Dictionary<string, List<WidgetData>> tabWidgetMap = this.LoadWidgetDatas();

            try
            {
                SqlDataReader reader = theDB.ExecuteQuery("select * from TB_GUITAB order by TABNAME");

                while (reader.Read())
                {
                    string tabName = (string)reader["TABNAME"];
                    int tabBackGroundColor = (int)reader["TABCOLOR"];

                    List<WidgetData> widgetList = null;

                    try
                    {
                        widgetList = tabWidgetMap[tabName];

                    }
                    catch (Exception ex)
                    {
                        widgetList = null;
                    }

                    if (widgetList != null)
                    {

                    }
                    else
                    {
                        widgetList = new List<WidgetData>();
                    }
                    this.tabDataList.Add(tabName, new TabData(tabName, tabBackGroundColor, widgetList));
                }
            }
            catch (SqlException ex)
            {
                System.Console.WriteLine("LoadLayout:" + ex.StackTrace);
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);

            }


        }

        /// <summary>
        /// 주어진 Tab Id에 해당하는 Tab이 가지고 있는
        /// 모든 Wiget의 Data에 대한 List값을 Return
        /// </summary>
        /// <param name="tabName">주어진 Tab 명</param>
        /// <returns>Tab이 담고 있는 모든 Wiget의 List를 Return</returns>
        public List<WidgetData> GetWidgetDataListByTabName(string tabName)
        {
            try
            {
                return this.tabDataList[tabName].widgetDataList;
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("GetWidgetDataListByTabName:" + ex.StackTrace);
            }
            return null;
        }

        /// <summary>
        /// Tab명이 담고 있는 모든 Tab Data를 Return
        /// </summary>
        /// <param name="tabName">Tab 명</param>
        /// <returns>Tab이 담고 있는 TabData</returns>
        public TabData GetTabDataByTabName(string tabName)
        {
            try
            {
                return this.tabDataList[tabName];
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("GetTabDataByTabName:" + ex.StackTrace);
            }
            return null;
        }

        /// <summary>
        /// 모든 WigetData를 Return
        /// 
        /// </summary>
        /// <returns>key : Tab명, value (Widget Data의 List)를 Return</returns>
        public Dictionary<string, List<WidgetData>> LoadWidgetDatas()
        {
            const string METHOD = CLASS + ".LoadWidgetDatas";

            Dictionary<string, List<WidgetData>> tabDataMap = new Dictionary<string, List<WidgetData>>();
            Dictionary<string, List<UserDefinedPropertyInfo>> widgetPropertyMap =
                LoadWidgetUserDefinedProperties();

            DBConnection conn = DBControl.GetConnection();
            string sqlstring = "";
            SqlDataReader reader = null;
            string lastTabPage = "";

            try
            {
//                sqlstring = "select * from GUIOBJECT order by TABPAGE, LOADSEQ ASC";
                sqlstring = "select * from TB_GUIOBJECT order by TABPAGE, LOADSEQ ASC";
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch(Exception ex)
            {
//                sqlstring = "select * from GUIOBJECT order by TABPAGE";
                System.Console.WriteLine("LoadWidgetDatas:" + ex.StackTrace);
                sqlstring = "select * from TB_GUIOBJECT order by TABPAGE";
                reader = conn.ExecuteQuery(sqlstring);
            }



            List<WidgetData> widgetDataList = new List<WidgetData>();

            while (reader.Read())
            {

                string newTabPage = (string)reader["TABPAGE"];

                if (!lastTabPage.Equals(newTabPage))
                {
                    if (widgetDataList.Count > 0)
                    {
                        // save previous one
                        tabDataMap.Add(lastTabPage, widgetDataList);
                        // assign new list
                        widgetDataList = new List<WidgetData>();
                    }
                    lastTabPage = newTabPage;
                }

                WidgetData widgetData = new WidgetData();

                widgetData.elementId = (string)reader["ELEMENTID"];
                if (reader.IsDBNull(1))
                {
                    widgetData.guId = Guid.NewGuid();
                }
                else
                {
                    widgetData.guId = (Guid)reader["GUIOBJECTGUID"];
                }

                if (widgetData.guId.Equals(Guid.Empty))
                {
                    widgetData.guId = Guid.NewGuid();
                }

                widgetData.x = (int)reader["X"];
                widgetData.y = (int)reader["Y"];
                widgetData.width = (int)reader["WIDTH"];
                widgetData.height = (int)reader["HEIGHT"];

                widgetData.widgetAssemblyName = 
                    (reader["ASSEMBLYNAME"]).Equals(DBNull.Value) ? "" : (string)reader["ASSEMBLYNAME"];
                widgetData.widgetClassName = (string)reader["CLASSNAME"];


                try
                {
                    widgetData.properties = widgetPropertyMap[widgetData.guId.ToString().ToUpper () ].ToArray();
                }
                catch (KeyNotFoundException ex)
                {
                    widgetData.properties = new UserDefinedPropertyInfo[0];
                }

                widgetDataList.Add(widgetData);
            }

            // save last data

            if (widgetDataList.Count > 0)
            {
                tabDataMap.Add(lastTabPage, widgetDataList);
            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);

            return tabDataMap;

        }


        /// <summary>
        /// EntityId(WigetId)에 해당하는 모든 User Defined PropertyInfo의 List를 Retrun함
        /// </summary>
        /// <returns>WidgetId에 해당하는 모든 UDF PropertyInfo의 Reference을 Return</returns>
        private Dictionary<string, List<UserDefinedPropertyInfo>> LoadWidgetUserDefinedProperties()
        {
            Dictionary<string, List<UserDefinedPropertyInfo>> rtnValue =
                new Dictionary<string, List<UserDefinedPropertyInfo>>();

            try
            {
                UserDefinedPropertyData[] dataList = UserDefinedProperty.
                    GetUserDefinedPropertyDataList(WIDGET_PROPERTIES_CLASS);

                for (int i = 0; i < dataList.Length; i++)
                {
                    List<UserDefinedPropertyInfo> infoes = new List<UserDefinedPropertyInfo>();

                    for (int j = 0; j < dataList[i].propertyList.Length; j++)
                    {
                        infoes.Add(dataList[i].propertyList[j]);
                    }

                    rtnValue.Add(dataList[i].entityId, infoes);
                }
            }
            catch (ECPException ex)
            {
                System.Console.WriteLine("LoadWidgetUserDefinedProperties:" + ex.StackTrace);
            }

            return rtnValue;
        }

        /// <summary>
        /// Widget의 Data을 Data base에 저장함
        /// </summary>
        /// <param name="tabDataList">widgetData를 저장하고 있는 TabData의 List</param>
        public void SaveWidgetDatas(Dictionary<string, TabData> tabDataList)
        {
            const string METHOD = CLASS + ".SaveWidgetDatas";

            DBConnection theDB = null;

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();

                //delete all the gui widget information first
                theDB.ExecuteUpdate("delete from TB_GUIOBJECT");
                theDB.ExecuteUpdate("delete from TB_GUITAB");
                theDB.ExecuteUpdate("delete from TB_GUIOBJECTUDPROPERTY");


                string sqlstring = "insert into TB_GUIOBJECT (TABPAGE, ELEMENTID,GUIOBJECTGUID,X,Y,WIDTH,HEIGHT,ASSEMBLYNAME,CLASSNAME) values (@TABPAGE,@ELEMENTID,@GUIOBJECTGUID,@X,@Y,@WIDTH,@HEIGHT,@ASSEMBLYNAME,@CLASSNAME)";


                foreach (TabData tabData in tabDataList.Values)
                {
                    theDB.ExecuteUpdate("insert into TB_GUITAB (TABNAME,TABCOLOR ) values ('" + tabData.tabName + "'," + tabData.tabBackGroundColor + ")");

                    UserDefinedPropertyData[] udPropertyList = new UserDefinedPropertyData[tabData.widgetDataList.Count];

                    int i = 0;

                    foreach (WidgetData wd in tabData.widgetDataList)
                    {

                        PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                        pss.SetVarChar("TABPAGE", tabData.tabName);
                        pss.SetVarChar("ELEMENTID", wd.elementId);
                        pss.SetUniqueIdentifier("GUIOBJECTGUID", wd.guId);
                        pss.SetInt("X", wd.x);
                        pss.SetInt("Y", wd.y);
                        pss.SetInt("WIDTH", wd.width);
                        pss.SetInt("HEIGHT", wd.height);
                        pss.SetVarChar("ASSEMBLYNAME", wd.widgetAssemblyName);
                        pss.SetVarChar("CLASSNAME", wd.widgetClassName);

                        theDB.ExecuteUpdate(sqlstring, pss);

                        udPropertyList[i] = new UserDefinedPropertyData(wd.guId.ToString().ToUpper(), wd.properties);

                        i++;
                    }

                    UserDefinedProperty.AddUserDefinedPropertyDataList(WIDGET_PROPERTIES_CLASS, udPropertyList);
                }

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "db insert error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());

                if (theDB != null)
                    theDB.Rollback();
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
                this.tabDataList = tabDataList;
            }
        }

        /// <summary>
        /// Tab단위로 Widget Data를 Database에 저장함
        /// </summary>
        /// <param name="tabData">저장할 TabData</param>
        public void SaveWidgetDatasByTab(TabData tabData)
        {
            const string METHOD = CLASS + ".SaveWidgetDatas";

            DBConnection theDB = null;

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();

                //delete all the gui widget information first
                theDB.ExecuteUpdate("delete from TB_GUIOBJECT where TABPAGE ='" + tabData.tabName + "'");
                theDB.ExecuteUpdate("delete from TB_GUITAB where TABNAME  ='" + tabData.tabName + "'");


                try
                {
                    foreach (WidgetData wd in this.tabDataList[tabData.tabName].widgetDataList)
                    {
                        UserDefinedProperty.DeleteAllForEntity(WIDGET_PROPERTIES_CLASS, wd.guId.ToString());
                    }
                }
                catch (Exception ex)
                {
                }
                string sqlstring = "insert into TB_GUIOBJECT (TABPAGE, ELEMENTID,GUIOBJECTGUID,X,Y,WIDTH,HEIGHT,ASSEMBLYNAME,CLASSNAME) values (@TABPAGE,@ELEMENTID,@GUIOBJECTGUID,@X,@Y,@WIDTH,@HEIGHT,@ASSEMBLYNAME,@CLASSNAME)";


                if (tabData.widgetDataList.Count > 0)
                {
                    theDB.ExecuteUpdate("insert into TB_GUITAB (TABNAME,TABCOLOR ) values ('" + tabData.tabName + "'," + tabData.tabBackGroundColor + ")");
                }

                UserDefinedPropertyData[] udPropertyList = new UserDefinedPropertyData[tabData.widgetDataList.Count];

                int i = 0;

                foreach (WidgetData wd in tabData.widgetDataList)
                {

                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                    pss.SetVarChar("TABPAGE", tabData.tabName);
                    pss.SetVarChar("ELEMENTID", wd.elementId);
                    pss.SetUniqueIdentifier("GUIOBJECTGUID", wd.guId);
                    pss.SetInt("X", wd.x);
                    pss.SetInt("Y", wd.y);
                    pss.SetInt("WIDTH", wd.width);
                    pss.SetInt("HEIGHT", wd.height);
                    pss.SetVarChar("ASSEMBLYNAME", wd.widgetAssemblyName);
                    pss.SetVarChar("CLASSNAME", wd.widgetClassName);

                    theDB.ExecuteUpdate(sqlstring, pss);
                    udPropertyList[i] = new UserDefinedPropertyData(wd.guId.ToString(), wd.properties);

                    i++;
                }

                UserDefinedProperty.AddUserDefinedPropertyDataList(WIDGET_PROPERTIES_CLASS, udPropertyList);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "db insert error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());

                if (theDB != null)
                    theDB.Rollback();
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
                this.tabDataList[tabData.tabName] = tabData;
            }
        }
    }


}
