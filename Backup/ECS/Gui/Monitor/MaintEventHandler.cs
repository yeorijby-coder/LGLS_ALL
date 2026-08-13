using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ECP.Util.Client;
using HECS.Util;
using HECS.Gui.Monitor.Controls;
using HECS.Device;
using HECS.Device.Unit;
 
 
using System.Collections.Specialized;
using System.Configuration;
using HECS.Global;

namespace HECS.Gui.Monitor
{
    public static class MaintEventHandler
    {

        struct INTERLOCK_MAKER_TYPE
        {
            public string MakerName;
            public bool IsRackMaster;
        }

        //public static void OnStockerInitPanelClick(object sender, StockerMaintInitPanelEventArg e)
        //{
        //    StockerSEM stockerSem = (StockerSEM)Naming.GetDeviceManager().GetDevice("");
        //    bool tem = true;


        //}



     

        public static void OnContorlOptionClick(object sender, ControlOptionButtonEventArgs e)
        {


        }


        //public static void OnMaintNavigationButtonClick(object sender, MaintPanelEventArgs e)
        //{
        //    StockerSEM stksem = (StockerSEM)Naming.GetDeviceManager().GetDevice("");

        //    StockerMaintInitPanel panelStockerInit = (StockerMaintInitPanel)SimpleCache.GetObject("Maint", "StockerInit");
        //    OptionPanel panelOptions = (OptionPanel)SimpleCache.GetObject("Maint", "Options");
        //    RackmasterSensorPanel rackMasterSensor = (RackmasterSensorPanel)SimpleCache.GetObject("Maint", "RackmasterSensorPanel");
        //    PortIOModePanel portIoMode = (PortIOModePanel)SimpleCache.GetObject("Maint", "portIoMove");
        //    ZoneSettingPanel zoneSetting = (ZoneSettingPanel)SimpleCache.GetObject("Maint", "ZoneSettingPanel");
        //    CombineSettingPanel combineSetting = (CombineSettingPanel)SimpleCache.GetObject("Maint", "CombineSettingPanel"); 

        //    //            panelStockerInit.Click += new StockerMaintInitPanelEvent(OnStockerInitPanelClick);
        //    //            panelCycleTest.Click += new CycleTestPanelEvent(OnCycleTestClick);

        //    if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.RECONCILE)
        //    {
                

        //        return;
        //    }

        //    if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.ErrMoveH)
        //    {
              
        //        return;
        //    }

        //    if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.ErrMoveR)
        //    {
        //        return;
        //    }

        //    if (e.EventPanelType != MaintPanelEventArgs.PanelEvent_Type.Stocker_INIT && panelStockerInit.Visible == true)
        //    {
        //        panelStockerInit.Visible = false;
        //    }
        //    if (e.EventPanelType != MaintPanelEventArgs.PanelEvent_Type.OPTIONS && panelOptions.Visible == true)
        //    {
        //        panelOptions.Visible = false;
        //    }

        //    if (e.EventPanelType != MaintPanelEventArgs.PanelEvent_Type.RM_SENSOR && rackMasterSensor.Visible == true)
        //    {
        //        rackMasterSensor.Visible = false;
        //    }

        //    if (e.EventPanelType != MaintPanelEventArgs.PanelEvent_Type.PORT_MODE && portIoMode.Visible == true)
        //    {
        //        portIoMode.Visible = false;
        //    }
        //    if (e.EventPanelType != MaintPanelEventArgs.PanelEvent_Type.ZONE_SETTING  && zoneSetting.Visible == true)
        //    {
        //        zoneSetting.Visible = false;
        //    }
        //    if (e.EventPanelType != MaintPanelEventArgs.PanelEvent_Type.COMBINE_SETTING  && combineSetting.Visible == true)
        //    {
        //        combineSetting.Visible = false;
        //    }




        //    if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.Stocker_INIT && panelStockerInit.Visible == false)
        //    {
        //        panelStockerInit.Visible = true;
        //        return;
        //    }
        //    else if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.OPTIONS && panelOptions.Visible == false)
        //    {
        //        panelOptions.Visible = true;
        //        return;
        //    }
        //    else if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.RM_SENSOR && rackMasterSensor.Visible == false)
        //    {
        //        rackMasterSensor.Visible = true;
        //        return;
        //    }
        //    else if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.PORT_MODE && portIoMode.Visible == false)
        //    {
        //        portIoMode.Visible = true;
        //        return;
        //    }
        //    else if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.ZONE_SETTING  && zoneSetting.Visible == false)
        //    {
        //        zoneSetting.Visible = true;
        //        return;
        //    }
        //    else if (e.EventPanelType == MaintPanelEventArgs.PanelEvent_Type.COMBINE_SETTING  && combineSetting.Visible == false)
        //    {
        //        combineSetting.Visible = true;
        //        return;
        //    }
        //}


 
    }

    public class SystemInterlockInfo
    {
        public SystemInterlockInfo()
        {

        }
        private bool interlockSet;

        public bool InterlockSet
        {
            get { return interlockSet; }
            set { interlockSet = value; }
        }

        private string interlockDetail;

        public string InterlockDetail
        {
            get { return interlockDetail; }
            set { interlockDetail = value; }
        }

    }
}
