using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using HECS.ConveyorSimulator.Devices;

namespace HECS.ConveyorSimulator.Controls
{
    public partial class AgvVehicleControl : UserControl
    {
        Timer refreshTimer = new Timer();
        private Vehicle vehicleObject = null;

        public Vehicle VehicleObject
        {
            get
            {
                return vehicleObject;
            }
            set
            {
                vehicleObject = value;

            }
        }
        public AgvVehicleControl()
        {
            InitializeComponent();
            refreshTimer.Tick += new EventHandler(refreshTimer_Tick);
            refreshTimer.Interval = 1000;
            refreshTimer.Enabled = true;
            refreshTimer.Start();
            Font font = new Font(FontFamily.GenericMonospace, 9);
            this.richTextBoxAVG1.Font = font;
        }
        private delegate void DisplayDelegate();
        private void RefreshAgvState()
        {
            if (vehicleObject == null)
            {
                return;
            }
            DisplayDelegate updateAgvState = delegate
            {
                this.textBoxVehicleName.Text = vehicleObject.DeviceId;
                this.textBoxVehicleState.Text = vehicleObject.VehicleRunState.ToString();
                if (vehicleObject.VehicleRunState == Vehicle.VEHICLE_RUN_STATE.DOWN)
                {
                    this.textBoxVehicleState.BackColor = Color.Red;
                }
                else if (vehicleObject.VehicleRunState == Vehicle.VEHICLE_RUN_STATE.RUN )
                {
                    this.textBoxVehicleState.BackColor = Color.Lime;
                }
                else
                {
                    this.textBoxVehicleState.BackColor = Color.Yellow ;
                }

                this.textBoxAlarmCode.Text = vehicleObject.AgvAlarmCode.ToString();
                this.textBoxCommand.Text = vehicleObject.AgvCommandId;
                this.textBoxDirection.Text = vehicleObject.AgvCommandDirection == "0" ? "To Host" : "From AGV";
                this.textBoxSEQ.Text = vehicleObject.AgvCommandSeq;
                this.textBoxReqReply.Text = vehicleObject.AgvCommandRequestReply == "1" ? "Need Reply" : "No Reply";
                this.textBoxSapCommand.Text = vehicleObject.AgvSapCommandId;
                this.textBoxSapSEQ.Text = vehicleObject.AgvSapSeq;
                this.textBoxVehicleId.Text = vehicleObject.AgvVehicleId;
                this.textBoxPalletId.Text = vehicleObject.AgvPalletId;
                this.textBoxFrom.Text = vehicleObject.AgvFrom;
                this.textBoxTo.Text = vehicleObject.AgvTo;
                this.textBoxPriority.Text = vehicleObject.AgvPriority;
                this.textBoxVehicleMarkplet.Text  = vehicleObject.AgvMarkPlate;
                this.textBoxVehicleLocation.Text = vehicleObject.AgvLocation;
                this.buttonVehicleState.Text = vehicleObject.VehicleState.ToString();
                if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.READY)
                {
                    this.buttonVehicleState.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonVehicleState.BackColor = Color.Lime;
                }


            };
            BeginInvoke(updateAgvState);
        }
        void refreshTimer_Tick(object sender, EventArgs e)
        {
            if (this.IsHandleCreated == false)
            {
                return;
            }
            if (vehicleObject != null)
            {
                RefreshAgvState();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (this.vehicleObject.VehicleRunState == Vehicle.VEHICLE_RUN_STATE.DOWN)
            {
                if (string.IsNullOrEmpty(this.vehicleObject.AgvPalletId))
                {
                    vehicleObject.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.IDLE;
                }
                else
                {
                    vehicleObject.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.RUN;
                }

                //알람이 있으면 알람해제 
                if (string.IsNullOrEmpty(vehicleObject.AgvAlarmCode) == false)
                {
                    this.vehicleObject.AgvAlarmCode = "";
                }
            }
            else
            {
                this.vehicleObject.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.DOWN;
                this.vehicleObject.AgvAlarmCode = "99";
           }
        }
    }
}
