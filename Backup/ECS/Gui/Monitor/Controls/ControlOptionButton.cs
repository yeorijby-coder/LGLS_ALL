using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using ECP.Event;
using ECP.Util.Client;

using HECS.Device;
using HECS.Util;

namespace HECS.Gui.Monitor.Controls
{
    public delegate void ControlOptionButtonEvent(object sender, ControlOptionButtonEventArgs e);

    public partial class ControlOptionButton :UserControl
    {
        public new event ControlOptionButtonEvent Click;

        private string elementId;
        public string ElementId
        {
            get { return elementId; }
            set { elementId = value; }
        }

        private bool isManualRecovery = false;

        public bool IsManualRecovery
        {
            get { return isManualRecovery; }
            set 
            { 
                isManualRecovery = value;
                chkRecovery.Checked = value;
            }
        }

        private bool isRecoveryUseFlag = false;
        public bool IsRecoveryUseFlag
        {
            get { return isRecoveryUseFlag; }
            set { isRecoveryUseFlag = value; chkRecovery.Visible = value; }
        }


        private string _title = "";
        private string _leftText = "";
        private string _rightText = "";
        private Color _lampColor = new Color();




        private bool leftCheck = true;
        public bool LeftCheck
        {
            get 
            {

                return leftCheck; 
            }
            set 
            { 

                leftCheck = value;
                if (leftCheck)
                {
                    this.buttonEnable.Image = global:: HECS.Properties.Resources.system52158600;
                    this.buttonDisable.Image = null;
                }
                else
                {
                    this.buttonEnable.Image = null;
                    this.buttonDisable.Image = global:: HECS.Properties.Resources.system52158600;

                }
            }
        }


        public Color LampColor
        {
            get
            {
                return _lampColor;
            }
            set
            {
                _lampColor = value;
                panelBackLamp.BackColor = value;
            }
        }
        public string Title
        {
            get
            {
                return _title;
            }
            set
            {
                _title = value;
                labelTop.Text = value;
            }
        }

        public string LeftText
        {
            get
            {
                return _leftText;
            }
            set
            {
                _leftText = value;
                buttonEnable.Text = value;
            }
        }

        public string RightText
        {
            get
            {
                return _rightText;
            }
            set
            {
                _rightText = value;
                buttonDisable.Text = value;
            }
        }
              
        protected virtual void ClickEventRaised(object sender, ControlOptionButtonEventArgs e)
        {
            if (Click != null)
            {
                Click(sender, e);
            }
        }

        public ControlOptionButton()
        {
            InitializeComponent();
            InitializeOptionButton();           
        }

        private void InitializeOptionButton()
        {
            panelBackLamp.BackColor = this.BackColor;
            chkRecovery.Checked = false;

       
        }

        private void buttonEnable_Click(object sender, EventArgs e)
        {
            switch (this.labelTop.Text)
            {

                case "R/M H.P SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_HP_SENSOR, true));
                    break;

                case "R/M M.D SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_MD_SENSOR, true));
                    break;

                case "R/M O.P SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_OP_SENSOR, true));
                    break;

                case "PORTS SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_PORT_SENSOR, true));
                    break;

                case "EQUIPMENTS SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_EQP_SENSOR, true));
                    break;

                case "Auto Empty/Double Reset":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.EMPTY_DOUBLE_REC, true));
                    break;

                case "Combine":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.COMBINE_OPTION, true));
                    break;

                case "Auto Move Back":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.AUTO_BACK_OPTION, true));
                    break;

                case "EQP I/F Error Auto Reset":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.IFERROR_AUTO_REC, true));
                    break;

                case "CT Option":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.CT_OPTION, true));
                    break;


                case "Eqp Alt Transfer":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.AUTO_EQP_ALT_TRS, true));
                    break;

                case "Dual CIM Control Mode":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.CIM_DUAL, true));
                    break;

                case "Around Port Down":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.AROUND_PORT_DOWN, true));
                    break;


                default:
                    break;

            }

            
        }

        private void buttonDisable_Click(object sender, EventArgs e)
        {
            switch (this.labelTop.Text)
            {

                case "R/M H.P SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_HP_SENSOR, false));
                    break;

                case "R/M M.D SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_MD_SENSOR, false));
                    break;

                case "R/M O.P SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_OP_SENSOR, false));
                    break;

                case "PORTS SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_PORT_SENSOR, false));
                    break;

                case "EQUIPMENTS SIDE SENSOR":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.RM_EQP_SENSOR, false));
                    break;

                case "Auto Empty/Double Reset":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.EMPTY_DOUBLE_REC, false));
                    break;

                case "Combine":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.COMBINE_OPTION, false));
                    break;

                case "Auto Move Back":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.AUTO_BACK_OPTION, false));
                    break;

                case "EQP I/F Error Auto Reset":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.IFERROR_AUTO_REC, false));
                    break;

                case "CT Option":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.CT_OPTION, false));
                    break;


                case "Eqp Alt Transfer":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs( ControlOptionButtonEventArgs.BUTTON_TYPE.AUTO_EQP_ALT_TRS, false));
                    break;

                case "Dual CIM Control Mode":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.CIM_DUAL, false));
                    break;

                case "Around Port Down":
                    ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.AROUND_PORT_DOWN, false));
                    break;

                default:
                    break;

            }

        }              

        private void chkRecovery_MouseDown(object sender, MouseEventArgs e)
        {
            if (chkRecovery.Checked)
            {
                switch (this.labelTop.Text)
                {

                    case "R/M H.P SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_HP_SENSOR_CHECK, false));
                        break;

                    case "R/M M.D SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_MD_SENSOR_CHECK, false));
                        break;

                    case "R/M O.P SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_OP_SENSOR_CHECK, false));
                        break;

                    case "PORTS SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_PORT_SENSOR_CHECK, false));
                        break;

                    case "EQUIPMENTS SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_EQP_SENSOR_CHECK, false));
                        break;
                }
            }
            else
            {
                switch (this.labelTop.Text)
                {

                    case "R/M H.P SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_HP_SENSOR_CHECK, true));
                        break;

                    case "R/M M.D SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_MD_SENSOR_CHECK, true));
                        break;

                    case "R/M O.P SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_OP_SENSOR_CHECK, true));
                        break;

                    case "PORTS SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_PORT_SENSOR_CHECK, true));
                        break;

                    case "EQUIPMENTS SIDE SENSOR":
                        ClickEventRaised(sender, new ControlOptionButtonEventArgs(ControlOptionButtonEventArgs.BUTTON_TYPE.RM_EQP_SENSOR_CHECK, true));
                        break;
                }
            }
        }

    }

    public class ControlOptionButtonEventArgs : EventArgs
    {
        public enum BUTTON_TYPE : int
        {
            None = 0,
            RM_HP_SENSOR = 1,
            RM_MD_SENSOR = 2,
            RM_OP_SENSOR = 3,
            RM_EQP_SENSOR =4,
            RM_PORT_SENSOR = 5,
            EMPTY_DOUBLE_REC = 6,
            COMBINE_OPTION = 7,
            AUTO_BACK_OPTION = 8,
            IFERROR_AUTO_REC = 9,
            CT_OPTION = 10,
            AUTO_EQP_ALT_TRS = 11,
            CIM_DUAL = 12,
            RM_HP_SENSOR_CHECK = 13,
            RM_MD_SENSOR_CHECK = 14,
            RM_OP_SENSOR_CHECK = 15,
            RM_EQP_SENSOR_CHECK = 16,
            RM_PORT_SENSOR_CHECK = 17,
            AROUND_PORT_DOWN = 18

        }

        private BUTTON_TYPE commandType = BUTTON_TYPE.None;

        public BUTTON_TYPE CommandType
        {
            get
            {
                return commandType;
            }
        }

        private bool optionValue = false;

        public bool OptionValue
        {
            get { return optionValue; }
            set { optionValue = value; }
        }


        private bool isMenualRecovery;

        public bool IsMenualRecovery
        {
            get { return isMenualRecovery; }
            set { isMenualRecovery = value; }
        }


        public ControlOptionButtonEventArgs(BUTTON_TYPE buttonType, bool optionValue)
        {
            this.optionValue = optionValue;
            this.commandType = buttonType;
        }
    }
}
