using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using HECS.ConveyorSimulator.Devices;
using ECP.Device.Observe;

namespace HECS.ConveyorSimulator.Controls
{
    public partial class VehicleControl : UserControl
    {
        private delegate void UpdateControlDelegate(string observerId, object value);
        Timer refreshTimer = new Timer();
        private string vehicleId = "";
        private Vehicle vehicleObject = null;
        public string VehicleId
        {
            get
            {
                return vehicleId;
            }
            set
            {
                vehicleId = value;
                SetVehicleObject();
            }
        }

        private void SetVehicleObject()
        {
            //labelConveyorID = conveyorId;
            if (DeviceManager.ExistVehicle(this.vehicleId) == false)
            {
                return;
            }
            this.vehicleObject = DeviceManager.GetVehicle(this.vehicleId);
            UInObjectValueSynch();
            refreshTimer.Enabled = true;
            this.labelVehicleId.Text = this.vehicleId + " [" + this.vehicleObject.Ownerid + "]";
        }

        private void UInObjectValueSynch()
        {
            if (vehicleObject.Observables.Count == 0)
            {
                return;
            }
            try
            {
                this.buttonAlarmResetReport.BackColor = (bool)vehicleObject.Observables["ALARM_RESET_REPORT"].Value ? Color.Lime : Color.LightGray;
                this.buttonAlarmResetReportACK.BackColor = (bool)vehicleObject.Observables["ALARM_RESET_REPORT_ACK"].Value ? Color.Lime : Color.LightGray;
                this.buttonAlarmSetReport.BackColor = (bool)vehicleObject.Observables["ALARM_SET_REPORT"].Value ? Color.Lime : Color.LightGray;
                this.buttonAlarmSetReportACK.BackColor = (bool)vehicleObject.Observables["ALARM_SET_REPORT_ACK"].Value ? Color.Lime : Color.LightGray;
                this.buttonLoadComplete.BackColor = (bool)vehicleObject.Observables["LOAD_COMPLETE"].Value ? Color.Lime : Color.LightGray;
                this.buttonLoadCompleteACK.BackColor = (bool)vehicleObject.Observables["LOAD_COMPLETE_ACK"].Value ? Color.Lime : Color.LightGray;
                this.buttonPalletExist.BackColor = (bool)vehicleObject.Observables["PALLET_EXIST_FLAG"].Value ? Color.Lime : Color.LightGray;
                this.buttonTransferRequestACK.BackColor = (bool)vehicleObject.Observables["TRANSFER_ACK"].Value ? Color.Lime : Color.LightGray;
                this.buttonTransferRequest.BackColor = (bool)vehicleObject.Observables["TRANSFER_REQUEST"].Value ? Color.Lime : Color.LightGray;
                this.buttonUnloadComplete.BackColor = (bool)vehicleObject.Observables["UNLOAD_COMPLETE"].Value ? Color.Lime : Color.LightGray;
                this.buttonUnloadCompleteACK.BackColor = (bool)vehicleObject.Observables["UNLOAD_COMPLETE_ACK"].Value ? Color.Lime : Color.LightGray;

                this.textBoxAlarmResetCode.Text = ((short)vehicleObject.Observables["ALARM_RESET_CODE"].Value).ToString();
                this.textBoxAlarmSetCode.Text = ((short)vehicleObject.Observables["ALARM_SET_CODE"].Value).ToString();
                this.textBoxOperationMode.Text = ((short)vehicleObject.Observables["OPERATION_MODE"].Value).ToString();
                this.textBoxVehicleState.Text = ((short)vehicleObject.Observables["SUBSYSTEM_STATUS"].Value).ToString();

                this.textBoxFrom1.Text = (string)vehicleObject.Observables["FROM_01"].Value;
                this.textBoxFrom2.Text = (string)vehicleObject.Observables["FROM_02"].Value;
                this.textBoxFrom3.Text = (string)vehicleObject.Observables["FROM_03"].Value;
                this.textBoxPalletIdTransfer.Text = (string)vehicleObject.Observables["PALLET_ID"].Value;
                this.textBoxPalletIdOnVehicle.Text = (string)vehicleObject.Observables["PALLET_ON_VEHICLE"].Value;
                this.textBoxLocation1.Text = (string)vehicleObject.Observables["SUBSYSTEM_LOCATION_01"].Value;
                this.textBoxLocation2.Text = (string)vehicleObject.Observables["SUBSYSTEM_LOCATION_02"].Value;
                this.textBoxLocation3.Text = (string)vehicleObject.Observables["SUBSYSTEM_LOCATION_03"].Value;
                this.textBoxTo1.Text = (string)vehicleObject.Observables["TO_01"].Value;
                this.textBoxTo2.Text = (string)vehicleObject.Observables["TO_02"].Value;
                this.textBoxTo3.Text = (string)vehicleObject.Observables["TO_03"].Value;
                this.textBoxCompleteLocation1.Text = (string)vehicleObject.Observables["TRANSFER_COMPLETE_LOCATION_01"].Value;
                this.textBoxCompleteLocation2.Text = (string)vehicleObject.Observables["TRANSFER_COMPLETE_LOCATION_02"].Value;
                this.textBoxCompleteLocation3.Text = (string)vehicleObject.Observables["TRANSFER_COMPLETE_LOCATION_03"].Value;
            }
            catch (Exception)
            {
            }
        }

 
        public VehicleControl()
        {
            InitializeComponent();
            refreshTimer.Interval = 500;
            refreshTimer.Tick += new EventHandler(refreshTimer_Tick);
        }

        void refreshTimer_Tick(object sender, EventArgs e)
        {
            this.UInObjectValueSynch();
        }

        private void buttonAlarmSetReport_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["ALARM_SET_REPORT"].Value == true)
            {
                vehicleObject.Observables["ALARM_SET_REPORT"].Value = false;
            }
            else
            {
                vehicleObject.Observables["ALARM_SET_REPORT"].Value = true;
            }
        }

        private void buttonAlarmResetReport_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["ALARM_RESET_REPORT"].Value == true)
            {
                vehicleObject.Observables["ALARM_RESET_REPORT"].Value = false;
            }
            else
            {
                vehicleObject.Observables["ALARM_RESET_REPORT"].Value = true;
            }
        }

        private void buttonLoadComplete_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["LOAD_COMPLETE"].Value == true)
            {
                vehicleObject.Observables["LOAD_COMPLETE"].Value = false;
            }
            else
            {
                vehicleObject.Observables["LOAD_COMPLETE"].Value = true;
            }
        }

        private void buttonUnloadComplete_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["UNLOAD_COMPLETE"].Value == true)
            {
                vehicleObject.Observables["UNLOAD_COMPLETE"].Value = false;
            }
            else
            {
                vehicleObject.Observables["UNLOAD_COMPLETE"].Value = true;
            }
        }

        private void buttonTransferRequest_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["TRANSFER_REQUEST"].Value == true)
            {
                vehicleObject.Observables["TRANSFER_REQUEST"].Value = false;
            }
            else
            {
                vehicleObject.Observables["TRANSFER_REQUEST"].Value = true;
            }
        }

        private void buttonPalletExist_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["PALLET_EXIST_FLAG"].Value == true)
            {
                vehicleObject.Observables["PALLET_EXIST_FLAG"].Value = false;
            }
            else
            {
                vehicleObject.Observables["PALLET_EXIST_FLAG"].Value = true;
            }
        }

        private void buttonAlarmSetReportACK_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["ALARM_SET_REPORT_ACK"].Value == true)
            {
                vehicleObject.Observables["ALARM_SET_REPORT_ACK"].Value = false;
            }
            else
            {
                vehicleObject.Observables["ALARM_SET_REPORT_ACK"].Value = true;
            }
        }

        private void buttonAlarmResetReportACK_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["ALARM_RESET_REPORT_ACK"].Value == true)
            {
                vehicleObject.Observables["ALARM_RESET_REPORT_ACK"].Value = false;
            }
            else
            {
                vehicleObject.Observables["ALARM_RESET_REPORT_ACK"].Value = true;
            }
        }

        private void buttonLoadCompleteACK_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["LOAD_COMPLETE_ACK"].Value == true)
            {
                vehicleObject.Observables["LOAD_COMPLETE_ACK"].Value = false;
            }
            else
            {
                vehicleObject.Observables["LOAD_COMPLETE_ACK"].Value = true;
            }
        }

        private void buttonUnloadCompleteACK_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["UNLOAD_COMPLETE_ACK"].Value == true)
            {
                vehicleObject.Observables["UNLOAD_COMPLETE_ACK"].Value = false;
            }
            else
            {
                vehicleObject.Observables["UNLOAD_COMPLETE_ACK"].Value = true;
            }
        }

        private void buttonTransferRequestACK_Click(object sender, EventArgs e)
        {
            if ((bool)vehicleObject.Observables["TRANSFER_ACK"].Value == true)
            {
                vehicleObject.Observables["TRANSFER_ACK"].Value = false;
            }
            else
            {
                vehicleObject.Observables["TRANSFER_ACK"].Value = true;
            }

        }

        private void textBoxInputFrom1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["FROM_01"].Value = textBoxInputFrom1.Text;
            }
        }

        private void textBoxInputFrom2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["FROM_02"].Value = textBoxInputFrom2.Text;
            }
        }

        private void textBoxInputFrom3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["FROM_03"].Value = textBoxInputFrom3.Text;
            }
        }

        private void textBoxInputTo1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["TO_01"].Value = textBoxInputTo1.Text;
            }
        }

        private void textBoxInputTo2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["TO_02"].Value = textBoxInputTo2.Text;
            }
        }

        private void textBoxInputTo3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["TO_03"].Value = textBoxInputTo3.Text;
            }
        }

        private void textBoxInputPalletIdTransfer_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["PALLET_ID"].Value = textBoxInputPalletIdTransfer.Text;
            }

        }

        private void textBoxInputLocation1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["SUBSYSTEM_LOCATION_01"].Value = textBoxInputLocation1.Text;
            }
        }

        private void textBoxInputLocation2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["SUBSYSTEM_LOCATION_02"].Value = textBoxInputLocation2.Text;
            }
        }

        private void textBoxInputLocation3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["SUBSYSTEM_LOCATION_03"].Value = textBoxInputLocation3.Text;
            }

        }

        private void textBoxInputCompleteLocation1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["TRANSFER_COMPLETE_LOCATION_01"].Value = textBoxInputCompleteLocation1.Text;
            }

        }

        private void textBoxInputCompleteLocation2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["TRANSFER_COMPLETE_LOCATION_02"].Value = textBoxInputCompleteLocation2.Text;
            }

        }

        private void textBoxInputCompleteLocation3_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["TRANSFER_COMPLETE_LOCATION_03"].Value = textBoxInputCompleteLocation3.Text;
            }

        }

        private void textBoxInputPalletIdOnVehicle_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["PALLET_ON_VEHICLE"].Value = textBoxInputPalletIdOnVehicle.Text;
            }

        }

        private void textBoxInputAlarmSetCode_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["ALARM_SET_CODE"].Value = textBoxInputAlarmSetCode.Text;
            }
        }

        private void textBoxInputAlarmResetCode_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["ALARM_RESET_CODE"].Value = textBoxInputAlarmResetCode.Text;
            }
        }

        private void textBoxInputOperationMode_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["OPERATION_MODE"].Value = textBoxInputOperationMode.Text;
            }
        }

        private void textBoxInputVehicleState_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                vehicleObject.Observables["SUBSYSTEM_STATUS"].Value = textBoxInputVehicleState.Text;
            }
        }

    }
}
