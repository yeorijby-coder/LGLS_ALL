using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HECS.Device.Unit;
using HECS.Device;

namespace HECS.Gui.Monitor.Popups
{
    public partial class ConveyorForm : DialogBaseForm
    {
        private delegate void delegatInitialize();
        Timer timerRefresh = new Timer();
        private Conveyor conveyorObject = null;
        private bool isFirst = false;
        private bool isCached = false;
        private Dictionary<string, Control> controlCache = new Dictionary<string, Control>();
        public Conveyor ConveyorObject
        {
            get
            {
                return conveyorObject;
            }
            set
            {
                conveyorObject = value;
                InitDelegate();
                timerRefresh.Start();
            }
        }
        public ConveyorForm()
        {
            InitializeComponent();

            timerRefresh.Interval = 1000;
            timerRefresh.Tick += new EventHandler(timerRefresh_Tick);
        }



        private void InitializeConveyorInfo()
        {
            if (conveyorObject == null)
            {
                return;
            }
            RefreshConveyorInformation();
            RefreshConveyorStatus();
            InitFormDisable(this);
            isCached = true;
            InitPortNumber();
            InitPortState();

        }

        private void InitPortState()
        {
            if (conveyorObject == null)
            {
                return;
            }
            foreach (Port port in conveyorObject.Ports.Values)
            {
                string palletId = this.conveyorObject.GetPalletFromPLC(port);
                Control portControl = controlCache["labelPalletId" + string.Format("{0:0#}", int.Parse(port.PortOrder))];
                if (portControl != null)
                {
                    portControl.Text = palletId;
                }

                portControl = controlCache["panelPalletExist" + string.Format("{0:0#}", int.Parse(port.PortOrder))];
                if (port.IsPalletExist == true && string.IsNullOrEmpty(palletId) == false)
                {
                    portControl.BackColor = Color.Cyan;
                }
                else if (port.IsPalletExist == true && string.IsNullOrEmpty(palletId) == true)
                {
                    portControl.BackColor = Color.DarkGreen;
                }
                else if (string.IsNullOrEmpty(palletId) == false)
                {
                    portControl.BackColor = Color.Crimson;
                }
                else
                {
                    portControl.BackColor = Color.White;
                }
                //if (isFirst == false)
                //{
                //    portControl = controlCache["textBoxPalletID" + string.Format("{0:0#}", int.Parse(port.PortOrder))];
                //    portControl.Text = conveyorObject.GetPalletFromPLC(port);
                //}
                //else
                //{
                //    string palletId = "";
                //    palletId = conveyorObject.GetPallet(port) ;
                //    if(string.IsNullOrEmpty(palletId)==fa)
                //    {
                //        portControl = controlCache["textBoxPalletID" + string.Format("{0:0#}", int.Parse(port.PortOrder))];
                //        portControl.Text = 
                //    }
                //}
            }
            if (isFirst == false)
            {
                isFirst = true;
            }
           if( conveyorObject.IsAvailible  == true)
            {
               checkBoxAvailible.Checked = false ; 
            }
            else
            {
                checkBoxAvailible.Checked = true; 
            }

        }

        private void InitFormDisable(Control container)
        {

            foreach (Control control in container.Controls)
            {
                switch (control.Name.Substring(0, control.Name.Length - 2))
                {
                    case "buttonForce":
                    case "buttonRemapping":
                    case "comboBoxTransfer":
                    case "labelConveyorId":
                    case "labelPalletId":
                    case "labelTransferDetail":
                    case "panelForce":
                    case "panelPalletExist":
                    case "panelPalletInput":
                    case "panelPort":
                    case "panelRemapping":
                    case "panelTransfer":
                    case "textBoxPalletID":
                        if (isCached == false)
                        {
                            if (controlCache.ContainsKey(control.Name) == false)
                            {
                                controlCache.Add(control.Name, control);
                            }
                        }
                        int controlNumber = 0;
                        int.TryParse(control.Name.Substring(control.Name.Length - 2), out controlNumber);
                        if (controlNumber > conveyorObject.Ports.Count)
                        {
                            control.Visible = false;
                        }
                        else
                        {
                            control.Visible = true;
                        }
                        break;
                }
                if (control.Controls.Count > 0)
                {
                    InitFormDisable(control);
                }
            }


        }

        private void InitPortNumber()
        {
            if (conveyorObject == null)
            {
                return;
            }
            foreach (Port port in conveyorObject.Ports.Values)
            {
                Control portId = controlCache["labelConveyorId" + string.Format("{0:0#}", int.Parse(port.PortOrder))];
                if (portId != null)
                {
                    portId.Text = port.ElementId.Split(':')[1];
                }
            }
        }

        public void InitDelegate()
        {
            try
            {
                this.BeginInvoke(new delegatInitialize(InitializeConveyorInfo));
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("InitDelegate:" + ex.StackTrace);
            }
        }
        void timerRefresh_Tick(object sender, EventArgs e)
        {
            if (conveyorObject == null)
            {
                return;
            }
            RefreshConveyorInformation();
            RefreshConveyorStatus();
            InitPortState();
        }

        private void RefreshConveyorInformation()
        {

            this.labelConveyorName.Text = conveyorObject.ElementId;
            this.labelConveyorPart.Text = GetWorkshopName(conveyorObject.ElementId);

        }

        private void RefreshConveyorStatus()
        {
            if (conveyorObject.RunState == Conveyor.CONVEYOR_RUN_STATE.IDLE)
            {
                this.labelConveyorStatus.GradientColorOne = Color.Gray;
                this.labelConveyorStatus.GradientColorTwo = Color.Gray;
                this.labelConveyorStatus.Text = "IDLE";
            }
            else if (conveyorObject.RunState == Conveyor.CONVEYOR_RUN_STATE.RUN)
            {
                this.labelConveyorStatus.GradientColorOne = Color.Lime;
                this.labelConveyorStatus.GradientColorTwo = Color.Lime;
                this.labelConveyorStatus.Text = "RUN";

            }
            else
            {
                this.labelConveyorStatus.GradientColorOne = Color.Red;
                this.labelConveyorStatus.GradientColorTwo = Color.Red;
                this.labelConveyorStatus.Text = "DOWN";
            }


        }

        private string GetWorkshopName(string conveyorId)
        {
            string retValue = "";
            switch (conveyorId)
            {
                case "CONVEYOR:2":
                    retValue = "[KR01] Hi-Rack#1호기 입/출고";
                    break;
                case "CONVEYOR:3":
                    retValue = "[KR01] Hi-Rack#2호기 출고";
                    break;
                case "CONVEYOR:4":
                    retValue = "[KR01] Hi-Rack#2호기 입고";
                    break;
                case "CONVEYOR:5":
                    retValue = "[KR01] Hi-Rack#3호기 출고";
                    break;
                case "CONVEYOR:6":
                    retValue = "[KR01] Hi-Rack#3호기 입고";
                    break;
                case "CONVEYOR:7":
                    retValue = "[KR01] Hi-Rack#4호기 출고";
                    break;
                case "CONVEYOR:8":
                    retValue = "[KR01] Hi-Rack#4호기 입고";
                    break;
                case "CONVEYOR:9":
                    retValue = "[KR01] Hi-Rack#5호기 출고";
                    break;
                case "CONVEYOR:10":
                    retValue = "[KR01] Hi-Rack#5호기 입고";
                    break;
                case "CONVEYOR:11":
                    retValue = "[KR00] 외부 전용 입/출고";
                    break;
                case "CONVEYOR:12":
                    retValue = "[KR02] 생산동 입고";
                    break;
                case "CONVEYOR:13":
                    retValue = "[KR02] 생산동 출고";
                    break;
                case "CONVEYOR:14":
                    retValue = "[KR03] 피킹존 출고";
                    break;
                case "CONVEYOR:15":
                    retValue = "[KR03] 피킹존 입고 ";
                    break;
                default:
                    retValue = "[ERROR], 알수없는 위치";
                    break;
            }
            return retValue;
        }

        private void buttonYes_Click(object sender, EventArgs e)
        {
            timerRefresh.Stop();
            this.Hide();
        }



        private void buttonForce_Click(object sender, EventArgs e)
        {
            if (conveyorObject == null)
            {
                return;
            }
            Control buttonControl = (Control)sender;
            string portIndex = buttonControl.Name.Substring(buttonControl.Name.Length - 2);
            Port port = conveyorObject.GetPortByOrder(int.Parse(portIndex).ToString());
            if (port == null)
            {
                return;
            }

            if (controlCache.ContainsKey("textBoxPalletID" + portIndex) == false)
            {
                return;
            }
            string writePalletId = controlCache["textBoxPalletID" + portIndex].Text;
            if (writePalletId == conveyorObject.GetPallet(port))
            {
                return;
            }

            conveyorObject.SetPallet(port, writePalletId);
            controlCache["textBoxPalletID" + portIndex].Text = "";
        }

        private void buttonRemapping_Click(object sender, EventArgs e)
        {

        }

        private void checkBoxAvailible_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxAvailible.Checked)
            {
                conveyorObject.IsAvailible = false;
            }
            else
            {
                conveyorObject.IsAvailible = true;
            }
        }

        private void panel5_Paint(object sender, PaintEventArgs e)
        {

        }


    }
}