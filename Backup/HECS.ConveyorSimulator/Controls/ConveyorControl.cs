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
    public partial class ConveyorControl : UserControl
    {
        private delegate void UpdateControlDelegate(string observerId, object value);
        Timer refreshTimer = new Timer();
        private string conveyorId = "";
        private Conveyor conveyorObject = null;
        public string ConveyorId
        {
            get
            {
                return conveyorId;
            }
            set
            {
                conveyorId = value;
                SetConveyorObject();
            }
        }

        private void SetConveyorObject()
        {
            //labelConveyorID = conveyorId;
            if (DeviceManager.ExistConveyor(this.conveyorId) == false)
            {
                return;
            }
            this.conveyorObject = DeviceManager.GetConveyor(this.conveyorId);
            UInObjectValueSynch();
            refreshTimer.Enabled = true;
            this.labelConveyorID.Text = this.conveyorId;
        }

        private void UInObjectValueSynch()
        {

            if (conveyorObject.Observables["PALLET_EXIST01"].Value != null)
            {
                this.textBoxPalletId01.Text = (string)conveyorObject.Observables["PALLET_EXIST01"].Value;
            }
            if (conveyorObject.Observables["PALLET_EXIST02"].Value != null)
            {
                this.textBoxPalletId02.Text = (string)conveyorObject.Observables["PALLET_EXIST02"].Value;
            }
            if (conveyorObject.Observables["PALLET_EXIST03"].Value != null)
            {
                this.textBoxPalletId03.Text = (string)conveyorObject.Observables["PALLET_EXIST03"].Value;
            }
            if (conveyorObject.Observables["PALLET_EXIST04"].Value != null)
            {
                this.textBoxPalletId04.Text = (string)conveyorObject.Observables["PALLET_EXIST04"].Value;
            }
            if (conveyorObject.Observables["PALLET_EXIST05"].Value != null)
            {
                this.textBoxPalletId05.Text = (string)conveyorObject.Observables["PALLET_EXIST05"].Value;
            }
            if (conveyorObject.Observables["DIRECTION_MODE"].Value != null)
            {
                this.textBoxDirection.Text = (string)conveyorObject.Observables["DIRECTION_MODE"].Value;
            }

            if (conveyorObject.Observables["PALLET_EXIST_FLAG_01"].Value != null)
            {
                if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_01"].Value == false)
                {
                    this.buttonPalletExitFlag01.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonPalletExitFlag01.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["PALLET_EXIST_FLAG_02"].Value != null)
            {
                if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_02"].Value == false)
                {
                    this.buttonPalletExitFlag02.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonPalletExitFlag02.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["PALLET_EXIST_FLAG_03"].Value != null)
            {
                if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_03"].Value == false)
                {
                    this.buttonPalletExitFlag03.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonPalletExitFlag03.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["PALLET_EXIST_FLAG_04"].Value != null)
            {
                if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_04"].Value == false)
                {
                    this.buttonPalletExitFlag04.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonPalletExitFlag04.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["PALLET_EXIST_FLAG_05"].Value != null)
            {
                if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_05"].Value == false)
                {
                    this.buttonPalletExitFlag05.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonPalletExitFlag05.BackColor = Color.Lime;
                }
            }

            if (conveyorObject.Observables["IN_READY_02"].Value != null)
            {
                if ((bool)conveyorObject.Observables["IN_READY_02"].Value == false)
                {
                    this.buttonInputReady.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonInputReady.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["LOAD_COMPLETE_01"].Value != null)
            {
                if ((bool)conveyorObject.Observables["LOAD_COMPLETE_01"].Value == false)
                {
                    this.buttonLoadComplete01.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonLoadComplete01.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["LOAD_COMPLETE_02"].Value != null)
            {
                if ((bool)conveyorObject.Observables["LOAD_COMPLETE_02"].Value == false)
                {
                    this.buttonLoadComplete02.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonLoadComplete02.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["UNLOAD_COMPLETE_01"].Value != null)
            {
                if ((bool)conveyorObject.Observables["UNLOAD_COMPLETE_01"].Value == false)
                {
                    this.buttonUnloadComplete01.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonUnloadComplete01.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["UNLOAD_COMPLETE_02"].Value != null)
            {
                if ((bool)conveyorObject.Observables["UNLOAD_COMPLETE_02"].Value == false)
                {
                    this.buttonUnloadComplete02.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonUnloadComplete02.BackColor = Color.Lime;
                }
            }
            if (conveyorObject.Observables["WAIT_IN"].Value != null)
            {
                if ((bool)conveyorObject.Observables["WAIT_IN"].Value == false)
                {
                    this.buttonWaitIn.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonWaitIn.BackColor = Color.Lime;
                }
            }


            if (conveyorObject.Observables["WAIT_OUT"].Value != null)
            {
                if ((bool)conveyorObject.Observables["WAIT_OUT"].Value == false)
                {
                    this.buttonWaitOut.BackColor = Color.LightGray;
                }
                else
                {
                    this.buttonWaitOut.BackColor = Color.Lime;
                }
            }

        }

        public ConveyorControl()
        {
            InitializeComponent();
            refreshTimer.Interval = 500;
            refreshTimer.Tick += new EventHandler(refreshTimer_Tick);
        }

        void refreshTimer_Tick(object sender, EventArgs e)
        {
            this.UInObjectValueSynch();
        }

        private void buttonInputReady_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["IN_READY_02"].Value == true)
            {
                conveyorObject.Observables["IN_READY_02"].Value = false;
            }
            else
            {
                conveyorObject.Observables["IN_READY_02"].Value = true;
            }
        }

        private void buttonLoadComplete01_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["LOAD_COMPLETE_01"].Value == true)
            {
                conveyorObject.Observables["LOAD_COMPLETE_01"].Value = false;
            }
            else
            {
                conveyorObject.Observables["LOAD_COMPLETE_01"].Value = true;
            }
        }

        private void buttonLoadComplete02_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["LOAD_COMPLETE_02"].Value == true)
            {
                conveyorObject.Observables["LOAD_COMPLETE_02"].Value = false;
            }
            else
            {
                conveyorObject.Observables["LOAD_COMPLETE_02"].Value = true;
            }
        }

        private void buttonUnloadComplete01_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["UNLOAD_COMPLETE_01"].Value == true)
            {
                conveyorObject.Observables["UNLOAD_COMPLETE_01"].Value = false;
            }
            else
            {
                conveyorObject.Observables["UNLOAD_COMPLETE_01"].Value = true;
            }
        }

        private void buttonUnloadComplete02_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["UNLOAD_COMPLETE_02"].Value == true)
            {
                conveyorObject.Observables["UNLOAD_COMPLETE_02"].Value = false;
            }
            else
            {
                conveyorObject.Observables["UNLOAD_COMPLETE_02"].Value = true;
            }
        }

        private void buttonWaitIn_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["WAIT_IN"].Value == true)
            {
                conveyorObject.Observables["WAIT_IN"].Value = false;
            }
            else
            {
                conveyorObject.Observables["WAIT_IN"].Value = true;
            }
        }

        private void buttonWaitOut_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["WAIT_OUT"].Value == true)
            {
                conveyorObject.Observables["WAIT_OUT"].Value = false;
            }
            else
            {
                conveyorObject.Observables["WAIT_OUT"].Value = true;
            }
        }

        private void buttonPalletExitFlag01_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_01"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_01"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_01"].Value = true;
            }
        }

        private void buttonPalletExitFlag02_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_02"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_02"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_02"].Value = true;
            }
        }

        private void buttonPalletExitFlag03_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_03"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_03"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_03"].Value = true;
            }
        }

        private void buttonPalletExitFlag04_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_04"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_04"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_04"].Value = true;
            }
        }

        private void buttonPalletExitFlag05_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_05"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_05"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_05"].Value = true;
            }
        }

        private void buttonPalletExitFlag06_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_06"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_06"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_06"].Value = true;
            }
        }

        private void buttonPalletExitFlag07_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_07"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_07"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_07"].Value = true;
            }
        }

        private void buttonPalletExitFlag08_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_08"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_08"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_08"].Value = true;
            }
        }

        private void buttonPalletExitFlag09_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["PALLET_EXIST_FLAG_09"].Value == true)
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_09"].Value = false;
            }
            else
            {
                conveyorObject.Observables["PALLET_EXIST_FLAG_09"].Value = true;
            }

        }

        private void buttonReserved_Click(object sender, EventArgs e)
        {

        }

        private void textBoxPalletIdInput01_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                conveyorObject.Observables["PALLET_EXIST01"].Value = textBoxPalletIdInput01.Text;
            }
        }



        private void textBoxPalletIdInput02_KeyDown(object sender, KeyEventArgs e)
        {

            if (e.KeyCode == Keys.Enter)
            {
                conveyorObject.Observables["PALLET_EXIST02"].Value = textBoxPalletIdInput02.Text;
            }
        }

        private void textBoxPalletIdInput01_KeyDown(object sender, KeyEventArgs e)
        {

            if (e.KeyCode == Keys.Enter)
            {
                conveyorObject.Observables["PALLET_EXIST01"].Value = textBoxPalletIdInput01.Text;
            }
        }

        private void textBoxPalletIdInput03_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                conveyorObject.Observables["PALLET_EXIST03"].Value = textBoxPalletIdInput03.Text;
            }

        }

        private void textBoxPalletIdInput04_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                conveyorObject.Observables["PALLET_EXIST04"].Value = textBoxPalletIdInput04.Text;
            }
        }

        private void textBoxPalletIdInput05_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                conveyorObject.Observables["PALLET_EXIST05"].Value = textBoxPalletIdInput05.Text;
            }
        }



        private void buttonOperationMode_Click(object sender, EventArgs e)
        {
            if ((bool)conveyorObject.Observables["OPERATION_MODE"].Value == true)
            {
                conveyorObject.Observables["OPERATION_MODE"].Value = false;
            }
            else
            {
                conveyorObject.Observables["OPERATION_MODE"].Value = true;
            }
        }

        private void textBoxPalletIdInput01_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBoxDirectionInput_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBoxDirectionInput_KeyDown(object sender, KeyEventArgs e)
        {

            if (e.KeyCode == Keys.Enter)
            {
                conveyorObject.Observables["DIRECTION_MODE"].Value = textBoxDirectionInput.Text;
            }

        }


    }
}
