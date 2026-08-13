using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using HECS.Gui.Widget;
using ECP.Util.Server;
using HECS.Device;
using ECP.Device;
using ECP.Global.Device;
using HECS.Device.Unit;
using ECP.Service.Device;
using HECS.Util;
using HECS.Gui.Monitor.Popups;

namespace HECS.Gui.Monitor.Panels.Monitoring
{
    public delegate void WidgetEventDelegate(object sender, EventArgs e);

    public partial class MonitorAllEquipmentPanel : UserControl
    {

        Timer refreshTimer = new Timer();
        private int refreshCount = 0;
        private int stackerTop = 0;
        public event WidgetEventDelegate WidgetClick;
        private Dictionary<string, HECS.Gui.Widget.MarkPlate> markplateWidgets = new Dictionary<string, HECS.Gui.Widget.MarkPlate>();
        private Dictionary<string, HECS.Gui.Widget.ConveyorWidget> conveyorWidgets = new Dictionary<string, HECS.Gui.Widget.ConveyorWidget>();
        private Dictionary<string, AGVWidget> agvWidgets = new Dictionary<string, AGVWidget>();
        private Dictionary<string, RGVWidget> rgvWidgets = new Dictionary<string, RGVWidget>();
        private Dictionary<string, LiftWidget> liftWidgets = new Dictionary<string, LiftWidget>();
        private Dictionary<string, StackerCraneWidget> stackerWidgets = new Dictionary<string, StackerCraneWidget>();
        public MonitorAllEquipmentPanel()
        {
            InitializeComponent();
            RegisterEvent();
            LinkingDevice(this);
            CacheMarkplate();
            CacheAgvWidget();
            CacheRgvWidget();
            CacheStackerWidget();
            CacheLiftWidget();
            CacheConveyorWidget();
            
            refreshTimer.Interval = 1000; 
            refreshTimer.Enabled = true;
            refreshTimer.Tick += new EventHandler(refreshTimer_Tick);
            refreshTimer.Start();

            labelVersion.Text  = "Ver " + Application.ProductVersion ;
        }

   

        void refreshTimer_Tick(object sender, EventArgs e)
        {
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if(vehicle.OwnerId.Contains ("AGV"))
                {
                    RefreshAgvVehicle(vehicle);
                }
                else if (vehicle.OwnerId.Contains("RGV"))
                {
                    RefreshRgvVehicle(vehicle);
                }
                else if (vehicle.OwnerId.Contains("LIFT"))
                {
                    RefreshLiftVehicle(vehicle);
                }
                else if (vehicle.OwnerId.Contains("STK"))
                {
                    RefreshStockerVehicle(vehicle);
                }
            }
            if (ECP.Global.GlobalConstant.START_MODE != "ECS")
            {
                refreshCount++;
                if (refreshCount < 10)
                {
                    return;
                }
                try
                {
                    refreshCount = 0;
                    this.Refresh();
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("MonitorAllEquipmentPanel.refreshTimer_Tick : " + ex.StackTrace);
                }
            }
        }

        private void RefreshLiftVehicle(Vehicle vehicle)
        {
            HECS.Gui.Widget.ConveyorWidget conveyorWidget = null;
            string portId = "";
            if (string.IsNullOrEmpty(vehicle.TransferCompleteLocation03))
            {
                portId = "38";
            }
            else
            {
                portId = int.Parse(vehicle.TransferCompleteLocation03 ).ToString();
            }
            if (conveyorWidgets.ContainsKey("PORT:" + portId))
            {
                conveyorWidget = conveyorWidgets["PORT:" + portId];
            }
            if (conveyorWidget == null)
            {
                return;
            }
            LiftWidget liftWidget = null;
            if (liftWidgets.ContainsKey(vehicle.ElementId))
            {
                liftWidget = liftWidgets[vehicle.ElementId];
            }
            if (liftWidget == null)
            {
                return;
            }
            liftWidget.Left = conveyorWidget.Left ;
        }

        private void RefreshStockerVehicle(Vehicle vehicle)
        {
            
            StackerCraneWidget stackerWidget = null;
            if (stackerWidgets.ContainsKey(vehicle.ElementId))
            {
                stackerWidget = stackerWidgets[vehicle.ElementId];
            }
            if (stackerWidget == null)
            {
                return;
            }
            int column = 0;
            if(string.IsNullOrEmpty(vehicle.VehicleLocation02))
            {
                column = 0;
            }
            if (int.TryParse(vehicle.VehicleLocation02, out column) == false)
            {
                column = 0;
            }
            stackerWidget.Top = stackerTop + (int)(column * 3.25);
        }
        private void RefreshRgvVehicle(Vehicle vehicle)
        {
            HECS.Gui.Widget.ConveyorWidget conveyorWidget = null;
            string portId = "";
            if (string.IsNullOrEmpty(vehicle.VehicleLocation03))
            {
                portId = "24";
            }
            else
            {
                portId = int.Parse(vehicle.VehicleLocation03).ToString();
            }
            if (conveyorWidgets.ContainsKey("PORT:" + portId))
            {
                conveyorWidget = conveyorWidgets["PORT:" + portId];
            }
            if (conveyorWidget == null)
            {
                return;
            }
            RGVWidget rgvWidget = null;
            if (rgvWidgets.ContainsKey(vehicle.ElementId))
            {
                rgvWidget = rgvWidgets[vehicle.ElementId];
            }
            if (rgvWidget == null)
            {
                return;
            }
            rgvWidget.Left = conveyorWidget.Left-2 ;

        }
        private void RefreshAgvVehicle(Vehicle vehicle)
        {
            HECS.Gui.Widget.MarkPlate markplate = null;
            if (markplateWidgets.ContainsKey(vehicle.AgvMarkPlate))
            {
                markplate = markplateWidgets[vehicle.AgvMarkPlate];
            }
            if (markplate == null)
            {
                return;
            }
            AGVWidget agvWidget = null;
            if (agvWidgets.ContainsKey(vehicle.ElementId))
            {
                agvWidget = agvWidgets[vehicle.ElementId];
            }
            if (agvWidget == null)
            {
                return;
            }
            agvWidget.Left = markplate.Left-10;
            agvWidget.Top  = markplate.Top -7; 
        }
        private void CacheAgvWidget()
        {
            foreach (Control control in this.Controls)
            {
                if (control is AGVWidget)
                {
                    AGVWidget agvWidget = control as AGVWidget;
                    if (control == null)
                    {
                        return;
                    }
                    if (agvWidgets.ContainsKey(agvWidget.ElementId ) == false)
                    {
                        agvWidgets.Add(agvWidget.ElementId, agvWidget);
                    }
                }
            }
        }
        private void CacheRgvWidget()
        {
            foreach (Control control in this.Controls)
            {
                if (control is RGVWidget)
                {
                    RGVWidget rgvWidget = control as RGVWidget;
                    if (control == null)
                    {
                        return;
                    }
                    if (rgvWidgets.ContainsKey(rgvWidget.ElementId) == false)
                    {
                        rgvWidgets.Add(rgvWidget.ElementId, rgvWidget);
                    }
                }
            }
        }
        private void CacheLiftWidget()
        {
            foreach (Control control in this.Controls)
            {
                if (control is LiftWidget)
                {
                    LiftWidget liftWidget = control as LiftWidget;
                    if (control == null)
                    {
                        return;
                    }
                    if (liftWidgets.ContainsKey(liftWidget.ElementId) == false)
                    {
                        liftWidgets.Add(liftWidget.ElementId, liftWidget);
                    }
                }
            }
        }
        private void CacheStackerWidget()
        {
            foreach (Control control in this.Controls)
            {
                if (control is StackerCraneWidget)
                {
                    StackerCraneWidget stackerWidget = control as StackerCraneWidget;
                    if (control == null)
                    {
                        return;
                    }
                    if (stackerWidgets.ContainsKey(stackerWidget.ElementId) == false)
                    {
                        stackerWidgets.Add(stackerWidget.ElementId, stackerWidget);
                        if (stackerTop == 0)
                        {
                            stackerTop = stackerWidget.Top;
                        }
                    }
                }
            }
        }
        private void CacheConveyorWidget()
        {
            foreach (Control control in this.Controls)
            {
                if (control is ConveyorWidget)
                {
                    ConveyorWidget conveyorWidget = control as ConveyorWidget;
                    if (control == null)
                    {
                        return;
                    }
                    if (string.IsNullOrEmpty(conveyorWidget.ElementId))
                    {
                        continue;
                    }
                    if (conveyorWidgets.ContainsKey(conveyorWidget.ElementId) == false)
                    {
                        conveyorWidgets.Add(conveyorWidget.ElementId, conveyorWidget);
                    }
                }
            }
        }
        private void CacheMarkplate()
        {
            foreach (Control control in this.Controls)
            {
                if (control is HECS.Gui.Widget.MarkPlate)
                {
                    HECS.Gui.Widget.MarkPlate markplate = control as HECS.Gui.Widget.MarkPlate;
                    if (control == null)
                    {
                        return; 
                    }
                    if (markplateWidgets.ContainsKey (markplate.WidgetText ) == false)
                    {
                        markplateWidgets.Add(markplate.WidgetText, markplate);
                    }
                }
            }
        }

        private delegate void WidgetRefresh();

        private void LinkingDevice(Control containerControl)
        {

            DeviceManager deviceManager = null;
            try
            {
                deviceManager = (DeviceManager)Naming.GetDeviceManager();
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("MonitorAllEquipmentPanel:" + ex.StackTrace);
                return;
            }

            foreach (Control control in containerControl.Controls)
            {
                if (control as EquipmentWidget != null)
                {
                    EquipmentWidget widget = control as EquipmentWidget;
                    Conveyor conveyor = ECSDeviceManager.GetConveyor(widget.ElementId);
                    if (conveyor != null)
                    {
                        widget.ConveyorObject = conveyor;
                    }

                }
                else if (control as ConveyorWidget != null)
                {
                    ConveyorWidget widget = control as ConveyorWidget;
                    Port port = ECSDeviceManager.GetPort(widget.ElementId);
                    if (port != null)
                    {
                        widget.PortObject = port;
                    }
                    else
                    {
                        Conveyor conveyor = ECSDeviceManager.GetConveyor(widget.ElementId);
                        if (conveyor != null)
                        {
                            widget.ConveyorObject = conveyor;
                        }
                    }

                }
                else if (control as AGVWidget != null)
                {
                    AGVWidget widget = control as AGVWidget;
                    Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                    if (vehicle != null)
                    {
                        widget.VehicleObject = vehicle;
                    }
                }
                else if (control as RGVWidget != null)
                {
                    RGVWidget widget = control as RGVWidget;
                    Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                    if (vehicle != null)
                    {
                        widget.VehicleObject = vehicle;
                    }
                }
                else if (control as LiftWidget  != null)
                {
                    LiftWidget widget = control as LiftWidget;
                    Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                    if (vehicle != null)
                    {
                        widget.VehicleObject = vehicle;
                    }
                }
                else if (control as StackerCraneWidget != null)
                {
                    StackerCraneWidget widget = control as StackerCraneWidget;
                    Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                    if (vehicle != null)
                    {
                        widget.VehicleObject = vehicle;
                    }
                }
                else if (control as WorkTableWidget != null)
                {
                    WorkTableWidget widget = control as WorkTableWidget;
                    Conveyor conveyor = ECSDeviceManager.GetConveyor(widget.ElementId);
                    if (conveyor != null)
                    {
                        widget.ConveyorObject = conveyor;
                    }
                }

                if (control.Controls.Count > 0)
                {
                    LinkingDevice(control);
                }

            }
        }


        private void RegisterEvent()
        {
            foreach (Control control in this.Controls)
            {
                control.Click += new EventHandler(Control_Click);
                foreach (Control eventControl in control.Controls)
                {
                    eventControl.Click += new EventHandler(Control_Click);
                }
            }

        }

        void Control_Click(object sender, EventArgs e)
        {
            if (sender == null)
            {
                return;
            }
            if (WidgetClick != null)
            {
                WidgetClick(sender, new EventArgs());
            }
        }

        private void markPlate23_Load(object sender, EventArgs e)
        {

        }

        private void MonitorAllEquipmentPanel_Load(object sender, EventArgs e)
        {

        }

        private void agvWidget2_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

            if (sender is ConveyorWidget)
            {
                ConveyorWidget widget = sender as ConveyorWidget ;
                Port port = ECSDeviceManager.GetPort(widget.ElementId);
                if(port ==null)
                {
                    return ; 
                }
                Conveyor conveyor = ECSDeviceManager.GetConveyor(port.OwnerId);
                if (conveyor == null)
                {
                    return;
                }

                ConveyorForm form = (ConveyorForm)SimpleCache.GetObject("Popup", "ConveyorForm");
                if (form == null)
                {
                    form = new ConveyorForm();
                    SimpleCache.AddObject("Popup", "ConveyorForm", form);
                }
                form.ConveyorObject = conveyor;
                form.Show();
            }
            else if (sender is StackerCraneWidget)
            {
                StackerCraneWidget widget = sender as StackerCraneWidget;
                Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                if (vehicle == null)
                {
                    return;
                }
                StackerForm form = (StackerForm)SimpleCache.GetObject("Popup", "StackerForm");
                if (form == null)
                {
                    form = new StackerForm();
                    SimpleCache.AddObject("Popup", "StackerForm", form); 

                }
                form.VehicleObject = vehicle;
                form.Show();
            }
            else if (sender is RGVWidget)
            {
                RGVWidget widget = sender as RGVWidget;
                Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                if (vehicle == null)
                {
                    return;
                }
                RGVForm form = (RGVForm)SimpleCache.GetObject("Popup", "RGVForm");
                if (form == null)
                {
                    form = new RGVForm();
                    SimpleCache.AddObject("Popup", "RGVForm", form);

                }
                form.VehicleObject = vehicle;
                form.Show();
            }
            else if (sender is LiftWidget )
            {
                LiftWidget widget = sender as LiftWidget;
                Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                if (vehicle == null)
                {
                    return;
                }
                LiftForm form = (LiftForm)SimpleCache.GetObject("Popup", "LiftForm");
                if (form == null)
                {
                    form = new LiftForm();
                    SimpleCache.AddObject("Popup", "LiftForm", form);

                }
                form.VehicleObject = vehicle;
                form.Show();
            }
            else if (sender is AGVWidget)
            {
                AGVWidget widget = sender as AGVWidget;
                Vehicle vehicle = ECSDeviceManager.GetVehicle(widget.ElementId);
                if (vehicle == null)
                {
                    return;
                }
                AGVForm form = (AGVForm)SimpleCache.GetObject("Popup", "AGVForm");
                if (form == null)
                {
                    form = new AGVForm();
                    SimpleCache.AddObject("Popup", "AGVForm", form);

                }
                form.VehicleObject = vehicle;
                form.Show();
            }

        }

        private void rgvWidget1_Load(object sender, EventArgs e)
        {

        }
    }
}
