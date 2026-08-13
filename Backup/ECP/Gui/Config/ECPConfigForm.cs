using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ECP.Gui.Widget;
using ECP.Service.Gui;
using ECP.Util.Client;
using ECP.Global.Gui;
using ECP.Service.User;
using ECP.Service.User.PermissionTarget;


namespace ECP.Gui.Config
{
    [Serializable]
    public partial class ECPConfigForm : Form
    {
        public const int ARROW = 0;
        public const int PICTURE = 1;
        public const int LINEDRAW = 2;
        public const int ARROWLINEDRAW = 3;
        public const int TEXTWRITE = 4;
        public const int FIXEDLABEL = 5;

        public const int SAMEDISTANCE = 100;
        public const int ALIGNTOP = 101;
        public const int ALIGNBOTTOM = 102;
        public const int ALIGNLEFT = 103;
        public const int ALIGNRIGHT = 104;
        public const int CENTERVERTICAL = 105;
        public const int CENTERHORIZONTAL = 106;
        public const int BRINGTOFRONT = 107;
        public const int SENTTOBACK = 108;
        public const int WIDGETLISTVIEWER = 109;
        //사용자 권한 문자열 상수
        const string PERMISIION_WIDGET_DROW = "Client.DrowWidget";
        const string CLASS = "ECPConfigForm";

        bool buttonLocked = false;

        bool dirtyFlag = false;

        public bool DirtyFlag
        {
            get { return dirtyFlag; }
            set { dirtyFlag = value; }
        }

        protected int selectedButtionId = 0;

        public int SelectedButtionId
        {
            get { return selectedButtionId; }
            set { selectedButtionId = value; }
        }

        ToolStripButton selectedButton = null;

        public ToolStripButton SelectedButton
        {
            get { return selectedButton; }
            set { selectedButton = value; }
        }

        protected ECPConfigPanel configPanel = null;

        public ECPConfigForm()
        {
            InitializeComponent();
            InitializePermission();
            selectedButton = this.arrowButton;
        }

        public virtual void Initialize()
        {
            this.configPanel.Owner = this;
            LoadLayout();
        }

        private void InitializePermission()
        {
            PermissionTarget target;
            target = new TargetTypeConstructor();

            List<IPermission> targets = new List<IPermission>();
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.saveButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.pictureButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.lineArrowButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.lineButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.textButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.alignBottomButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.alignLeftButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.alignRightButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.alignTopButton));
            targets.Add(target.CreateType(PERMISIION_WIDGET_DROW, this.sameDistanceButton));

            UserPermission.AddTarget(CLASS, targets);
            UserPermission.GetInstance().DisableFuncationByUserPermission();
        }

        public virtual void LoadLayout()
        {
            List<WidgetData> widgetDataList = Naming.GetGUIManager().GetWidgetDataListByTabName("MONITOR");

            if (widgetDataList != null)
            {
                foreach (WidgetData wd in widgetDataList)
                {
                    this.configPanel.CreateWidget(wd, false, true);
                }
            }
        }

        public virtual void Save()
        {
            this.Cursor = Cursors.WaitCursor;

      
            Naming.GetGUIManager().SaveWidgetDatasByTab(new TabData("MONITOR", 0, this.configPanel.GetWidgetDatas()));

            this.dirtyFlag = false;
            this.Cursor = Cursors.Arrow;
        }

        public virtual string GetSelectedButtonWidgetClassName()
        {
            switch (this.selectedButtionId)
            {
                case PICTURE:
                    return "ECP.Gui.Widget.PictureWidget";
                case LINEDRAW:
                    return "ECP.Gui.Widget.LineWidget";
                case ARROWLINEDRAW:
                    return "ECP.Gui.Widget.LineWidgetWithArrow";
                case TEXTWRITE:
                    return "ECP.Gui.Widget.TextWidget";
                case FIXEDLABEL:
                    return "ECP.Gui.Widget.TextWidget";
                default: return "";
            }
        }

        public virtual string GetSelectedButtonWidgetAssemblyName()
        {
            switch (this.selectedButtionId)
            {
                case PICTURE:
                    return "ECP";
                case LINEDRAW:
                    return "ECP";
                case ARROWLINEDRAW:
                    return "ECP";
                case TEXTWRITE:
                    return "ECP";
                case FIXEDLABEL:
                    return "ECP";
                default: return "";
            }
        }
        public virtual string GetUniqueWidgetElemantId(int hashCode)
        {
            return "UNKWIDGET" + hashCode;
        }
        public virtual void HandleButtons(object sender, EventArgs e)
        {
            ToolStripButton button = (ToolStripButton)sender;
            
            //Default
            if (button == this.exitButton)
            {
                this.WindowClosing();
                return;
            }
            else if (button == this.saveButton)
            {
                this.dirtyFlag = true;
                if (this.dirtyFlag == true)
                {
                    Save();
                }
                this.saveButton.Checked = false;
                return;
            }

            //Drawing Tools
            else if (button == this.arrowButton)
            {
                this.selectedButtionId = ARROW;
            }            
            else if (button == this.pictureButton)
            {
                this.selectedButtionId = PICTURE;
            }
            else if (button == this.lineButton)
            {
                this.selectedButtionId = LINEDRAW;
            }
            else if (button == this.lineArrowButton)
            {
                this.selectedButtionId = ARROWLINEDRAW;
            }
            else if (button == this.textButton)
            {
                this.selectedButtionId = TEXTWRITE;
            }

            //Alignment
            else if (button == this.sameDistanceButton)
            {
                this.selectedButtionId = SAMEDISTANCE;
                this.configPanel.ToolButtonActionHander();
                return;
            }
            else if (button == this.alignTopButton)
            {
                this.selectedButtionId = ALIGNTOP;
                this.configPanel.ToolButtonActionHander();
                return;
            }
            else if (button == this.alignBottomButton)
            {
                this.selectedButtionId = ALIGNBOTTOM;
                this.configPanel.ToolButtonActionHander();
                return;
            }
            else if (button == this.alignLeftButton)
            {
                this.selectedButtionId = ALIGNLEFT;
                this.configPanel.ToolButtonActionHander();
                return;
            }
            else if (button == this.alignRightButton)
            {
                this.selectedButtionId = ALIGNRIGHT;
                this.configPanel.ToolButtonActionHander();
                return;
            }
            else if (button == this.centerVerticalButton)
            {
                this.selectedButtionId = CENTERVERTICAL;
                this.configPanel.ToolButtonActionHander();
                return;
            }
            else if (button == this.centerHorizontalButton)
            {
                this.selectedButtionId = CENTERHORIZONTAL;
                this.configPanel.ToolButtonActionHander();
                return;
            }

            //Widget Order
            else if (button == this.BringToFrontButton)
            {
                this.selectedButtionId = BRINGTOFRONT;
                this.configPanel.ToolButtonActionHander();
                return;
            }
            else if (button == this.SendToBackButton)
            {
                this.selectedButtionId = SENTTOBACK;
                this.configPanel.ToolButtonActionHander();
                return;
            }

            //Append Function
            else if (button == this.WidgetListViewer)
            {
                this.selectedButtionId = WIDGETLISTVIEWER;
                this.configPanel.ToolButtonActionHander();
                return;
            }

            if (this.selectedButton == button && selectedButton != this.arrowButton)
            {
                buttonLocked = true;
            }
            else
            {
                buttonLocked = false;
            }

            this.selectedButton.Checked = false;

            this.selectedButton = button;
            button.Checked = true;
        }

        public void ResetButtons()
        {
            if (!buttonLocked)
            {
                selectedButton.Checked = false;
                selectedButtionId = ARROW;
                selectedButton = arrowButton;
                arrowButton.Checked = true;
            }
        }

        // Window Event Handler
        void WindowClosing()
        {
            //Application.Exit();
        }
        private void WindowFormClosed(object sender, FormClosedEventArgs e)
        {
            this.WindowClosing();
        }

        private void WindowFormClosing(object sender, FormClosingEventArgs e)
        {
            this.WindowClosing();
        }

        private void ECPConfigForm_Load(object sender, EventArgs e)
        {

        }
 
    }
}