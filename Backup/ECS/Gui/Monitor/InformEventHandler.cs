using System;
using System.Collections.Generic;
using System.Text;
using HECS.Util;

namespace HECS.Gui.Monitor
{
    public static class InformEventHandler
    {
        //public static void OnInformNavigationButtonClick(object sender, InformPanelEventArgs e)
        //{

        //    InformCarrier informCarrier = (InformCarrier)SimpleCache.GetObject("Inform", "InformCarrier");
        //    InformTransfer informTransfer = (InformTransfer)SimpleCache.GetObject("Inform", "InformTransfer");
        //    InformZoneData informZoneData = (InformZoneData)SimpleCache.GetObject("Inform", "InformZoneData");
        //    InformLogError informLogError = (InformLogError)SimpleCache.GetObject("Inform", "InformLogError");
        //    InformLogHost informLogHost = (InformLogHost)SimpleCache.GetObject("Inform", "InformLogHost");
        //    InformLogPLC informLogPLC = (InformLogPLC)SimpleCache.GetObject("Inform", "InformLogPLC");
        //    InformLogDebug informLogDebug = (InformLogDebug)SimpleCache.GetObject("Inform", "InformLogDebug");
        //    InformLogInhibit informLogInhibit = (InformLogInhibit)SimpleCache.GetObject("Inform", "InformLogInhibit");
        //    InformLogOper informLogOper = (InformLogOper)SimpleCache.GetObject("Inform", "InformLogOper");
        //    InformTransferHistory informTransferHistory = (InformTransferHistory)SimpleCache.GetObject("Inform", "InformTransferHistory");
        //    InformServoError informServoError = (InformServoError)SimpleCache.GetObject("Inform", "InformServoError");
        //    InformServoValue informServoValue = (InformServoValue)SimpleCache.GetObject("Inform", "InformServoValue");


        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.CarrierDB && informCarrier.Visible == true)
        //    {
        //        informCarrier.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.TransferDB && informTransfer.Visible == true)
        //    {
        //        informTransfer.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.ZoneData && informZoneData.Visible == true)
        //    {
        //        informZoneData.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.Operation && informLogOper.Visible == true)
        //    {
        //        informLogOper.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.Error && informLogError.Visible == true)
        //    {
        //        informLogError.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.Host && informLogHost.Visible == true)
        //    {
        //        informLogHost.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.PLC && informLogPLC.Visible == true)
        //    {
        //        informLogPLC.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.Debug && informLogDebug.Visible == true)
        //    {
        //        informLogDebug.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.Inhibit && informLogInhibit.Visible == true)
        //    {
        //        informLogInhibit.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.TransferHistory && informTransferHistory.Visible == true)
        //    {
        //        informTransferHistory.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.ServoError && informServoError.Visible == true)
        //    {
        //        informServoError.Visible = false;
        //    }
        //    if (e.EventPanelType != InformPanelEventArgs.PanelEvent_Type.ServoValue && informServoValue.Visible == true)
        //    {
        //        informServoValue.Visible = false;
        //    }


        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.CarrierDB && informCarrier.Visible == false)
        //    {
        //        informCarrier.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.TransferDB)// && informTransfer.Visible == false)
        //    {
        //        informTransfer.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.ZoneData && informZoneData.Visible == false)
        //    {
        //        informZoneData.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.Operation && informLogOper.Visible == false)
        //    {
        //        informLogOper.SetDefaltSerchingDate();
        //        informLogOper.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.Error && informLogError.Visible == false)
        //    {
        //        informLogError.SetDefaltSerchingDate();
        //        informLogError.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.Host && informLogHost.Visible == false)
        //    {
        //        informLogHost.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.PLC && informLogPLC.Visible == false)
        //    {
        //        informLogPLC.SetDefaltSerchingDate();
        //        informLogPLC.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.Debug && informLogDebug.Visible == false)
        //    {
        //        informLogDebug.SetDefaltSerchingDate();
        //        informLogDebug.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.Inhibit && informLogInhibit.Visible == false)
        //    {
        //        informLogInhibit.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.TransferHistory && informTransferHistory.Visible == false)
        //    {
        //        informTransferHistory.SetDefaltSerchingDate();
        //        informTransferHistory.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.ServoError && informServoError.Visible == false)
        //    {
        //        informServoError.SetDefaltSerchingDate();
        //        informServoError.Visible = true;
        //        return;
        //    }
        //    if (e.EventPanelType == InformPanelEventArgs.PanelEvent_Type.ServoValue && informServoValue.Visible == false)
        //    {
        //        informServoValue.Visible = true;
        //        return;
        //    }
        //}
    }
}
