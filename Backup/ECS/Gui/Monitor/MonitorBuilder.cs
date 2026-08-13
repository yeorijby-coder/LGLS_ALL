using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using HECS.Global;
using System.IO;
using HECS.Gui.Widget;
using ECP.Util.Server;
using HECS.Device;
using HECS.Device.Unit;
using HECS.Util;
using HECS.Gui.Jaeit.Panels.GroupBox;
using ECP.Global.Device;

using ECP.Gui.Widget;
using System.Reflection;
using System.Runtime.Remoting;
using ECP.Global.Gui;

using ECP.Gui.Device.Oserve;
using ECP.Util;
using ECP.Gui.Util;
using ECP.Db;
using System.Data.SqlClient;
using System.ComponentModel;
 
using System.Drawing.Drawing2D;
using ECP.Service.Gui;
using ECP.Service.Logger;

namespace HECS.Gui.Monitor
{
    public class MonitorBuilder
    {
        const string CLASS = "MonitorBuilder";
        static ObservableSpyForm spyForm = null;
        static EventViewerForm eventViewerForm = null;
        private static GlobalKeyboardHook globalHook = new GlobalKeyboardHook();


        public MonitorBuilder()
        {

            //BuildAll();
        }

        public static void BuildAll(StockerMainForm form)
        {
            BuildFormMain(form);
            //Splasher.Status = "Main Form을 생성합니다.";
            //BuildOperation();
            //Splasher.Status = "Operation UI를 생성합니다.";
            //BuildMaintForm();
            //Splasher.Status = "Maint UI를 생성합니다.";
            //BuildInformForm();
            //Splasher.Status = "Inform UI을 생성합니다.";
            //BuildLogForm();
            //BuildGadgetForm();
            //LoadWidget();
            //Splasher.Status = "Widget을 로딩합니다.";
            //EventDelegation();

            //Splasher.Status = "Widget을 초기화 합니다.";
            //Form stockerFormMain = (Form)SimpleCache.GetObject("FormMain", "StockerMainForm");
            //stockerFormMain.WindowState = FormWindowState.Maximized;
            //stockerFormMain.Show();
            //stockerFormMain.PerformAutoScale();

            //System.Timers.Timer timerMonitorUpdate = new System.Timers.Timer();
            //timerMonitorUpdate.Interval = 3000;
            //timerMonitorUpdate.Start();
            //Panel panel = (Panel)SimpleCache.GetObject("Operation", "PanelWidgetContainer");


            //TrackBar trackBarZoom = (TrackBar)SimpleCache.GetObject("Operation", "TrackBarZoom");

        }




        private static void LoadWidget()
        {
            //const string METHOD = CLASS + ".LoadWidget";
            //Panel panel = (Panel)SimpleCache.GetObject("Operation", "PanelWidgetContainer");
            //GUIManager guiManager = Naming.GetGUIManager();
            //Dictionary<string, TabData> tabDataList = guiManager.TabDataList;
            //List<WidgetData> widgetList = guiManager.GetWidgetDataListByTabName("MONITOR");
            //foreach (WidgetData widgetData in widgetList)
            //{
            //    BaseWidget newWidget = null;
            //    ObjectHandle objHandle = null;

            //    try
            //    {
            //        objHandle = Activator.CreateInstance(widgetData.widgetAssemblyName, widgetData.widgetClassName);
            //        newWidget = (BaseWidget)objHandle.Unwrap();
            //    }
            //    catch (Exception ex)
            //    {
            //        try
            //        {
            //            objHandle = Activator.CreateInstance("Stocker", widgetData.widgetClassName);
            //            newWidget = (BaseWidget)objHandle.Unwrap();
            //        }
            //        catch (Exception ex2)
            //        {
            //            try
            //            {
            //                objHandle = Activator.CreateInstance("ECP", widgetData.widgetClassName);
            //                newWidget = (BaseWidget)objHandle.Unwrap();
            //            }
            //            catch (Exception ex3)
            //            {
            //                Log.log(5, METHOD, "error", Category.ERROR, "", "MonitorBuilder",
            //                "failed to reflect widget : " + widgetData.elementId +
            //                "\n" + ex3.Message.ToString());
            //                return;
            //            }
            //        }
            //    }
            //    newWidget.WidgetData = widgetData;
                
            //    newWidget.SetBounds(widgetData.x, widgetData.y, widgetData.width, widgetData.height);
            //    panel.Controls.Add(newWidget);
            //    SimpleCache.AddObject(newWidget.GetType().Name, newWidget.ElementId, newWidget);
            //}
            //panel.Width = panel.Width + 50;        //강제로 50px 만큼 더해준다. 
            //panel.ResumeLayout(true);
        }

        private static void BuildGadgetForm()
        {
            //spyForm = new ECP.Gui.Device.Oserve.
            //    ObservableSpyForm(ECP.Device.AbstractElement.Global_Observables);
            //globalHook.KeyDown += new KeyEventHandler(globalHook_KeyDown);
        }

        static void globalHook_KeyDown(object sender, KeyEventArgs e)
        {
            //if (e.Control && e.Shift)
            //{
            //    if (e.KeyCode == Keys.F10 && !spyForm.Visible)
            //    {
            //        spyForm.Show();
            //    }
            //    else if (e.KeyCode == Keys.F11)
            //    {
            //        eventViewerForm = new EventViewerForm();
            //        eventViewerForm.Show();
            //    }
            //}
        }

        public static void BuildFormMain(StockerMainForm form)
        {
            ////            Form stockerMainForm = new Form();
            ////Form stockerMainForm = new StockerMainForm();
            //StockerMainForm stockerMainForm = form;
            //Panel topPanel = new Panel();
            //Panel bodyPanel = new Panel();
            //Panel bottomPanel = new Panel();
            //stockerMainForm.SuspendLayout();
            //stockerMainForm.Size = new Size(1024, 768);
            //stockerMainForm.BackColor = System.Drawing.Color.GhostWhite;
            //stockerMainForm.StartPosition = FormStartPosition.CenterScreen;
            //stockerMainForm.WindowState = FormWindowState.Maximized;
            //stockerMainForm.Controls.Add(bodyPanel);
            //stockerMainForm.Controls.Add(bottomPanel);
            //stockerMainForm.Controls.Add(topPanel);
            //stockerMainForm.Visible = false;
            //stockerMainForm.Name = "StockerMainForm";
            //stockerMainForm.Text = "StockerController for LGDisplay P8 Version : " + Assembly.GetExecutingAssembly().GetName().Version.ToString() + " 기동시간 :" + DateTime.Now;
    

            //bodyPanel.BackColor = Color.GhostWhite;
            ////controlCenterPanel.BorderStyle = BorderStyle.FixedSingle;
            //bodyPanel.Dock = DockStyle.Fill;
            //bodyPanel.Location = new Point(0, 72);
            //bodyPanel.Name = "panelMain";
            //// 
            //// panelBottom
            //// 
            //bottomPanel.BackColor = Color.GhostWhite;
            //bottomPanel.Dock = DockStyle.Bottom;
            //bottomPanel.Location = new Point(0, 524);
            //bottomPanel.Name = "panelBottom";
            //bottomPanel.Size = new Size(813, 85);
            //// 
            //// panelTop
            //// 
            //topPanel.BackColor = Color.GhostWhite;
            //topPanel.Dock = DockStyle.Top;
            //topPanel.Location = new Point(0, 0);
            //topPanel.Name = "HeaderTopPanel";
            //topPanel.Size = new Size(0, 65);

            //stockerMainForm.ResumeLayout(false);
            //stockerMainForm.Visible = true;
            //stockerMainForm.Show();

            ////Main Application Expire for
            //stockerMainForm.FormClosed += new FormClosedEventHandler(mainApplicationShutDownTerminated);

            //SimpleCache.AddObject("FormMain", "StockerMainForm", stockerMainForm);
            //SimpleCache.AddObject("FormMain", "BodyPanel", bodyPanel);
            //SimpleCache.AddObject("FormMain", "TopPanel", topPanel);
            //SimpleCache.AddObject("FormMain", "BottomPanel", bottomPanel);
        }


        public static void BuildOperation()
        {
            //Form stockerFormMain = (Form)SimpleCache.GetObject("FormMain", "StockerMainForm");

            //Panel bodyPanel = (Panel)SimpleCache.GetObject("FormMain", "BodyPanel");
            //Panel topPanel = (Panel)SimpleCache.GetObject("FormMain", "TopPanel");
            //Panel bottomPanel = (Panel)SimpleCache.GetObject("FormMain", "BottomPanel");


            //TopMonitorPanel topMonitorPanel;
            //BottomNavigationPanel bottomNavigationPanel;
            //SplitContainer operationPanel;
            //GroupBoxChamfer widgetRectPanel;
            //Panel transferPanel;
            //TransferCommandPanel transferCommandPanel;
            //LogInformationPanel logInformationPanel;
            //TabControl tabCommandAndLog;
            //TabPage tabCommand;
            //TabPage tabLog;

            //ShelfInformationPanel shelfInformationPanel;
            //RackmasterStatePanel rackmasterStatePanel;
            ////OptionPanel optionPanel;
            //Panel panelWidgetContainer;
            //AlarmCodeList alarmCodeList;
            //DoWorkingPanel doWorkingPanel;
            //ChatForm chatForm;
            ////ZoneDataView zoneDataview;

            //// RackMaster Exception Form
            //ExceptionForm exceptionHomeRacMasterForm;
            //ExceptionForm exceptionRearRacMasterForm;

            //// 금지단 및 정보 변경 사유 입력 창
            //WriteReasonForm writeReasonForm;

            //LoginForm loginForm;

            //// OderMeter Clear Form
            //OdermeterClearForm odeMeterClearForm;

            //// MessageDialog Popup
            //MessageDialog messageDialog;

            //// Operation Panel zoom in and out
            //TrackBar trackBarZoom = new TrackBar();

            //topMonitorPanel = new TopMonitorPanel();
            //bottomNavigationPanel = new BottomNavigationPanel();
            //operationPanel = new SplitContainer();
            //widgetRectPanel = new GroupBoxChamfer();
            //panelWidgetContainer = new Panel();
            //transferPanel = new Panel();
            //transferCommandPanel = new TransferCommandPanel();
            //logInformationPanel = new LogInformationPanel();
            //tabCommandAndLog = new System.Windows.Forms.TabControl();
            //tabCommand = new System.Windows.Forms.TabPage();
            //tabLog = new System.Windows.Forms.TabPage();
            //shelfInformationPanel = new ShelfInformationPanel();
            //rackmasterStatePanel = new RackmasterStatePanel();
            //doWorkingPanel = new DoWorkingPanel();
            //alarmCodeList = new AlarmCodeList();
            //chatForm = new ChatForm();
            /////////////////////////  POP UP FORM CREATE ///////////////////////
            //exceptionHomeRacMasterForm = new ExceptionForm();
            //exceptionRearRacMasterForm = new ExceptionForm();
            //writeReasonForm = new WriteReasonForm();

            //loginForm = new LoginForm();
            //odeMeterClearForm = new OdermeterClearForm();
            //messageDialog = new MessageDialog();



            //bodyPanel.SuspendLayout();
            //bottomPanel.SuspendLayout();
            //topPanel.SuspendLayout();
            //topMonitorPanel.SuspendLayout();
            //operationPanel.SuspendLayout();
            //widgetRectPanel.SuspendLayout();
            //transferPanel.SuspendLayout();
            //rackmasterStatePanel.SuspendLayout();
            //tabCommandAndLog.SuspendLayout();
            //tabCommand.SuspendLayout();
            //tabLog.SuspendLayout();


            //stockerFormMain.SuspendLayout();
            //// 
            //// panelMain
            //// 
            //bodyPanel.Controls.Add(operationPanel);
            //bodyPanel.Controls.Add(doWorkingPanel);

            //// 
            //// panelBottom
            //// 
            //bottomPanel.Controls.Add(bottomNavigationPanel);
            //// 
            //// panelTop
            //// 
            //topPanel.Controls.Add(topMonitorPanel);
            ////panelTop.Size = new System.Drawing.Size(1272, 58);

            ////
            ////DoWorkingPanel
            ////
            //doWorkingPanel.Dock = DockStyle.Fill;
            //doWorkingPanel.Hide();

            //// 
            //// panelTopInfo
            //// 
            //topMonitorPanel.BlinkInterval = 0;
            //topMonitorPanel.BorderColor = System.Drawing.Color.Beige;
            //topMonitorPanel.BorderThick = new Padding(0);
            //topMonitorPanel.Dock = DockStyle.Fill;
            //topMonitorPanel.Location = new System.Drawing.Point(0, 0);
            //topMonitorPanel.Name = "TopMonitorPanel";
            //topMonitorPanel.Size = new System.Drawing.Size(1272, 58);
            //// 
            //// PanelBottomMenu
            //// 
            //bottomNavigationPanel.BackColor = System.Drawing.Color.Beige;
            //bottomNavigationPanel.Dock = DockStyle.Fill;
            //bottomNavigationPanel.Location = new System.Drawing.Point(0, 0);
            //bottomNavigationPanel.Margin = new Padding(0);
            //bottomNavigationPanel.Name = "BottomNavigationPanel";
            //bottomNavigationPanel.Size = new System.Drawing.Size(1272, 84);
            //// 
            //// panelOperation
            //// 
            //operationPanel.BackColor = System.Drawing.Color.RoyalBlue;
            //operationPanel.ForeColor = System.Drawing.Color.Beige;
            //operationPanel.Panel1.BackColor = System.Drawing.Color.Beige;
            //operationPanel.Panel2.BackColor = System.Drawing.Color.Beige;
            //operationPanel.Panel1.ForeColor = System.Drawing.Color.Beige;
            //operationPanel.Panel2.ForeColor = System.Drawing.Color.Beige;
            //operationPanel.Panel1.Controls.Add(widgetRectPanel);
            //operationPanel.Panel2.Controls.Add(tabCommandAndLog);
            //operationPanel.Panel1.Controls.Add(shelfInformationPanel);
            //operationPanel.Dock = DockStyle.Fill;
            //operationPanel.Location = new System.Drawing.Point(0, 0);
            //operationPanel.Name = "OperationContainerPanel";
            //operationPanel.Size = new System.Drawing.Size(1272, 855);
            //operationPanel.Panel1MinSize = 300;
            //operationPanel.Panel2MinSize = 185;
            //operationPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            //operationPanel.SplitterDistance = 670;
            ////
            //// panelWork
            ////




            //widgetRectPanel.BackColor = System.Drawing.Color.Beige;
            //widgetRectPanel.BorderWidth = 6;
            //widgetRectPanel.ColorScheme = HECS.Gui.Jaeit.Panels.GroupBox.EnmColorScheme.Green;
            //widgetRectPanel.Controls.Add(panelWidgetContainer);
            //widgetRectPanel.Dock = DockStyle.Fill;
            //widgetRectPanel.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            //widgetRectPanel.FontColor = System.Drawing.Color.FromArgb(((int)(((byte)(57)))), ((int)(((byte)(66)))), ((int)(((byte)(1)))));
            //widgetRectPanel.Location = new System.Drawing.Point(0, 58);
            //widgetRectPanel.Name = "OperationPanel";
            //widgetRectPanel.Padding = new Padding(11, 8, 10, 10);
            //widgetRectPanel.ShadowColor = System.Drawing.Color.FromArgb(((int)(((byte)(142)))), ((int)(((byte)(143)))), ((int)(((byte)(116)))));
            //widgetRectPanel.Size = new System.Drawing.Size(961, 670);
            //widgetRectPanel.TabStop = false;
            //widgetRectPanel.Text = "Operation";
            //widgetRectPanel.Controls.Add(rackmasterStatePanel);

            //trackBarZoom.Cursor = System.Windows.Forms.Cursors.Hand;
            //trackBarZoom.TabStop  = false; 
            //trackBarZoom.Dock = System.Windows.Forms.DockStyle.Left;
            //trackBarZoom.LargeChange = 10;
            //trackBarZoom.Location = new System.Drawing.Point(0, 0);
            //trackBarZoom.Maximum = 150;
            //trackBarZoom.Minimum = 50;
            //trackBarZoom.Name = "trackBarZoom";
            //trackBarZoom.Orientation = System.Windows.Forms.Orientation.Vertical;
            //trackBarZoom.Size = new System.Drawing.Size(30, 561);
            //trackBarZoom.SmallChange = 10;
            //trackBarZoom.TickFrequency = 10;
            //trackBarZoom.TickStyle = System.Windows.Forms.TickStyle.Both;
            //trackBarZoom.Value = 100;
            //trackBarZoom.BackColor = Color.LemonChiffon;
            ////trackBarZoom.Visible = false;
            //widgetRectPanel.Controls.Add(trackBarZoom);
            //// 
            //// panelWorkBoard
            //// 
            //panelWidgetContainer.BackColor = System.Drawing.Color.LemonChiffon;
            //panelWidgetContainer.Dock = DockStyle.Fill;
            //panelWidgetContainer.Location = new System.Drawing.Point(11, 22);
            //panelWidgetContainer.Name = "PanelWidgetContainer";
            //panelWidgetContainer.AutoScroll = true;
            //panelWidgetContainer.Size = new System.Drawing.Size(940, 677);

            //// 
            //// panelCommandQueue
            //// 
            //transferCommandPanel.Dock = DockStyle.Fill;
            //transferCommandPanel.Location = new System.Drawing.Point(0, 0);
            //transferCommandPanel.Name = "TransferCommandPanel";
            //transferCommandPanel.Size = new System.Drawing.Size(1024, 175);
            //// 
            //// panelWorkItem
            //// 
            ////            transferPanel.Controls.Add(transferDetailPanel);
            //transferPanel.Dock = DockStyle.Fill;
            //transferPanel.Location = new System.Drawing.Point(0, 0);
            //transferPanel.Name = "TransferPanel";
            //transferPanel.Size = new System.Drawing.Size(1024, 180);
            //transferPanel.Controls.Add(transferCommandPanel);

            //// 
            //// panelLogMessage
            //// 
            //logInformationPanel.BackColor = System.Drawing.Color.Beige;
            //logInformationPanel.Dock = DockStyle.Fill;
            //logInformationPanel.Location = new System.Drawing.Point(0, 767);
            //logInformationPanel.Name = "LogInformationPanel";
            //logInformationPanel.Padding = new Padding(2);
            //logInformationPanel.Size = new System.Drawing.Size(1272, 180);

            //// 
            //// tabCommand
            //// 
            //tabCommand.BackColor = System.Drawing.Color.Beige;
            //tabCommand.Location = new System.Drawing.Point(0, 0);
            //tabCommand.Name = "tabCommand";
            //tabCommand.Padding = new System.Windows.Forms.Padding(0);
            //tabCommand.Size = new System.Drawing.Size(640, 170);
            //tabCommand.TabIndex = 0;
            //tabCommand.Text = "반송명령";
            //tabCommand.Controls.Add(transferPanel);
            //// 
            //// tabLog
            //// 
            //tabLog.Location = new System.Drawing.Point(0, 0);
            //tabLog.Name = "tabLog";
            //tabLog.Padding = new System.Windows.Forms.Padding(0);
            //tabLog.Size = new System.Drawing.Size(640, 170);
            //tabLog.TabIndex = 1;
            //tabLog.Text = "운영로그";
            ////tabLog.UseVisualStyleBackColor = true;
            //tabLog.Controls.Add(logInformationPanel);


            //tabCommandAndLog.Alignment = System.Windows.Forms.TabAlignment.Left;
            //tabCommandAndLog.Dock = DockStyle.Fill;
            //tabCommandAndLog.Controls.Add(tabCommand);
            //tabCommandAndLog.Controls.Add(tabLog);
            //tabCommandAndLog.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            //tabCommandAndLog.Location = new System.Drawing.Point(0, 700);
            //tabCommandAndLog.Multiline = true;
            //tabCommandAndLog.Name = "CommandAndLog";
            //tabCommandAndLog.SelectedIndex = 0;
            //tabCommandAndLog.Size = new System.Drawing.Size(1200, 200);
            //tabCommandAndLog.TabIndex = 0;

            //// 
            //// panelGlassInfo
            //// 
            //shelfInformationPanel.BackColor = Color.Beige;
            //shelfInformationPanel.Dock = DockStyle.Top;
            //shelfInformationPanel.Location = new System.Drawing.Point(0, 0);
            //shelfInformationPanel.Name = "ShelfInformationPanel";
            //shelfInformationPanel.Size = new System.Drawing.Size(1272, 58);

            //rackmasterStatePanel.Font = new Font("Arial", 8.25F, FontStyle.Regular);
            //rackmasterStatePanel.Dock = DockStyle.Bottom;
            //rackmasterStatePanel.Location = new System.Drawing.Point(0, 0);
            //rackmasterStatePanel.Name = "RackmasterStatePanel";
            //rackmasterStatePanel.Size = new System.Drawing.Size(600, 20);
            //rackmasterStatePanel.ForeColor = Color.Black;
            //rackmasterStatePanel.BackColor = Color.Beige;

            //// 
            //// FormMain
            //// 
            //rackmasterStatePanel.ResumeLayout(true);
            //bodyPanel.ResumeLayout(true);
            //bottomPanel.ResumeLayout(true);
            //topPanel.ResumeLayout(true);
            //topMonitorPanel.ResumeLayout(true);
            //operationPanel.ResumeLayout(true);
            //operationPanel.Panel1.ResumeLayout(true);
            //operationPanel.Panel2.ResumeLayout(true);
            //widgetRectPanel.ResumeLayout(true);
            //transferPanel.ResumeLayout(true);
            //tabCommandAndLog.ResumeLayout(true);
            //tabCommand.ResumeLayout(true);
            //tabLog.ResumeLayout(true);
            //stockerFormMain.ResumeLayout(true);
            

            //////////////////////  MAIN FORM ADD  ////////////////////
            //SimpleCache.AddObject("FormMain", "TopMonitorPanel", topMonitorPanel);
            //SimpleCache.AddObject("FormMain", "BottomNavigationPanel", bottomNavigationPanel);
            //SimpleCache.AddObject("FormMain", "ShelfInformationPanel", shelfInformationPanel);

            //////////////////////  OPERATION FORM ADD  ////////////////////
            //SimpleCache.AddObject("Operation", "OperationPanel", operationPanel);
            //SimpleCache.AddObject("Operation", "WidgetRectPanel", widgetRectPanel);
            //SimpleCache.AddObject("Operation", "PanelWidgetContainer", panelWidgetContainer);
            //SimpleCache.AddObject("Operation", "TransferPanel", transferPanel);
            //SimpleCache.AddObject("Operation", "TransferCommandPanel", transferCommandPanel);
            //SimpleCache.AddObject("Operation", "LogInformationPanel", logInformationPanel);
            //SimpleCache.AddObject("Operation", "RackmasterStatePanel", rackmasterStatePanel);
            //SimpleCache.AddObject("Operation", "TrackBarZoom", trackBarZoom);

            //////////////////////  ETC FORM ADD  ////////////////////
            //SimpleCache.AddObject("Popup", "OperationPanel", alarmCodeList);
            //SimpleCache.AddObject("Popup", "ChatForm", chatForm );
            //SimpleCache.AddObject("Popup", "ExceptionHomeRacMasterForm", exceptionHomeRacMasterForm);
            //SimpleCache.AddObject("Popup", "ExceptionRearRacMasterForm", exceptionRearRacMasterForm);
            //SimpleCache.AddObject("Popup", "LoginForm", loginForm);
            //SimpleCache.AddObject("Popup", "WriteReasonForm", writeReasonForm);
            //SimpleCache.AddObject("Popup", "OderMeterClearForm", odeMeterClearForm);
            //SimpleCache.AddObject("Popup", "MessageDialog", messageDialog);
            //SimpleCache.AddObject("Operation", "DoWorkingPanel", doWorkingPanel);

        }

        public static void BuildMaintForm()
        {
            //Panel bodyPanel = (Panel)SimpleCache.GetObject("FormMain", "BodyPanel");



            //MaintPanel maintPanel = new MaintPanel();
            //maintPanel.Visible = false;
            //bodyPanel.Controls.Add(maintPanel);
            //maintPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            //maintPanel.Location = new System.Drawing.Point(0, 0);
            //maintPanel.Name = "panelMaint";
            //maintPanel.Size = new System.Drawing.Size(1012, 581);

            //SimpleCache.AddObject("Maint", "MaintPanel", maintPanel);
        }




        public static void BuildInformForm()
        {
            //Panel bodyPanel = (Panel)SimpleCache.GetObject("FormMain", "BodyPanel");

            ////panelInform
            //InformPanel informPanel = new InformPanel();
            //informPanel.Visible = false;
            //bodyPanel.Controls.Add(informPanel);
            //informPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            //informPanel.Location = new System.Drawing.Point(0, 0);
            //informPanel.Name = "panelInform";
            //informPanel.Size = new System.Drawing.Size(1012, 581);
            //SimpleCache.AddObject("Inform", "InformPanel", informPanel);


        }
        public static void BuildLogForm()
        {
            //Panel bodyPanel = (Panel)SimpleCache.GetObject("FormMain", "BodyPanel");
        }



        //================================================================
        //================================================================
        // Event 처리 루틴들.. 일단 이 부분은 다른 부분으로 빼내야 할듯.
        //================================================================
        //================================================================
        public static void EventDelegation()
        {

            ////////////////////         MAIN FORM            //////////////////
            //Form stockerFormMain = (Form)SimpleCache.GetObject("FormMain", "StockerMainForm");
            //stockerFormMain.FormClosing += new FormClosingEventHandler(MonitorEventHandler.MainFormClosingEvent);


            ////////////////////         상단 메뉴            //////////////////
            //TopMonitorPanel topMonitorPanel = (TopMonitorPanel)SimpleCache.GetObject("FormMain", "TopMonitorPanel");

            ////주 메뉴 

            //BottomNavigationPanel bottomNavigationPanel = (BottomNavigationPanel)SimpleCache.GetObject("FormMain", "BottomNavigationPanel");

            //InformPanel informPanel = (InformPanel)SimpleCache.GetObject("Inform", "InformPanel");
            //informPanel.Click += new InformPanelEvent(InformEventHandler.OnInformNavigationButtonClick);


            //ShelfInformationPanel shelfInformationPanel = (ShelfInformationPanel)SimpleCache.GetObject("FormMain", "ShelfInformationPanel");


            //TransferCommandPanel transferCommandPanel = (TransferCommandPanel)SimpleCache.GetObject("Operation", "TransferCommandPanel");



            //LoginForm loginForm = (LoginForm)SimpleCache.GetObject("Popup", "LoginForm");


            ///////////////////////// MAINT PANEL SUBSCRIBE ///////////////////////
            //MaintPanel maintPanel = (MaintPanel)SimpleCache.GetObject("Maint", "MaintPanel");
            //maintPanel.Click += new MaintPanelEvent(MaintEventHandler.OnMaintNavigationButtonClick);

            //StockerMaintInitPanel panelStockerInit = (StockerMaintInitPanel)SimpleCache.GetObject("Maint", "StockerInit");
            //panelStockerInit.Click += new StockerMaintInitPanelEvent(MaintEventHandler.OnStockerInitPanelClick);




            ///////////////////////// POP UP SUBSCRIBE ///////////////////////


            //ExceptionForm excetionHome = (ExceptionForm)SimpleCache.GetObject("Popup", "ExceptionHomeRacMasterForm");


            //ExceptionForm excetionRear = (ExceptionForm)SimpleCache.GetObject("Popup", "ExceptionRearRacMasterForm");


            //OdermeterClearForm oderMeterFrom = (OdermeterClearForm)SimpleCache.GetObject("Popup", "OderMeterClearForm");

            ///*///// POP UP Listening을 하기 위해 subscribe ////////////*/


            //Panel panelWidgetContainer = (Panel)SimpleCache.GetObject("Operation", "PanelWidgetContainer");
            //panelWidgetContainer.MouseClick += new MouseEventHandler(MonitorEventHandler.PanelWidgetContainer_Scroll);
            //panelWidgetContainer.DragOver += new DragEventHandler(MonitorEventHandler.PanelWidgetContainer_DragOver);
            //panelWidgetContainer.AllowDrop = true;
        }



        #region MonitorClickEventProcess

        public static void mainApplicationShutDownTerminated(object sender, FormClosedEventArgs e)
        {
            Form form = (Form)sender;
            form.Dispose();
        }

        #endregion MonitorClickEventProcess






    }

}
