namespace HECS.Gui.Monitor.Panels.Monitoring
{
    partial class HiRackDetailPanel
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HiRackDetailPanel));
            this.labelRight = new System.Windows.Forms.Label();
            this.labelLeft = new System.Windows.Forms.Label();
            this.panel6 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.equipmentWidget7 = new HECS.Gui.Widget.EquipmentWidget();
            this.conveyorWidget26 = new HECS.Gui.Widget.ConveyorWidget();
            this.conveyorWidget27 = new HECS.Gui.Widget.ConveyorWidget();
            this.conveyorWidget28 = new HECS.Gui.Widget.ConveyorWidget();
            this.equipmentWidget6 = new HECS.Gui.Widget.EquipmentWidget();
            this.conveyorWidget25 = new HECS.Gui.Widget.ConveyorWidget();
            this.conveyorWidget23 = new HECS.Gui.Widget.ConveyorWidget();
            this.conveyorWidget24 = new HECS.Gui.Widget.ConveyorWidget();
            this.hiRackProcessRight = new HECS.Gui.Monitor.Panels.Monitoring.HiRackProcessRIghtPanel();
            this.stackerCraneWidget1 = new HECS.Gui.Widget.StackerCraneWidget();
            this.hiRackProcessLeft = new HECS.Gui.Monitor.Panels.Monitoring.HiRackProcessLeftPanel();
            this.panel6.SuspendLayout();
            this.equipmentWidget7.SuspendLayout();
            this.equipmentWidget6.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelRight
            // 
            this.labelRight.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelRight.ForeColor = System.Drawing.SystemColors.ControlText;
            this.labelRight.Location = new System.Drawing.Point(8, 261);
            this.labelRight.Name = "labelRight";
            this.labelRight.Size = new System.Drawing.Size(52, 168);
            this.labelRight.TabIndex = 18;
            this.labelRight.Text = "RIGHT";
            this.labelRight.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelLeft
            // 
            this.labelLeft.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelLeft.ForeColor = System.Drawing.SystemColors.ControlText;
            this.labelLeft.Location = new System.Drawing.Point(8, 0);
            this.labelLeft.Name = "labelLeft";
            this.labelLeft.Size = new System.Drawing.Size(55, 181);
            this.labelLeft.TabIndex = 17;
            this.labelLeft.Text = "LEFT";
            this.labelLeft.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel6
            // 
            this.panel6.Controls.Add(this.stackerCraneWidget1);
            this.panel6.Controls.Add(this.label1);
            this.panel6.Location = new System.Drawing.Point(61, 208);
            this.panel6.Name = "panel6";
            this.panel6.Size = new System.Drawing.Size(601, 29);
            this.panel6.TabIndex = 15;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label1.Location = new System.Drawing.Point(11, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(590, 3);
            this.label1.TabIndex = 0;
            // 
            // equipmentWidget7
            // 
            this.equipmentWidget7.BackColor = System.Drawing.Color.Yellow;
            this.equipmentWidget7.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.equipmentWidget7.Controls.Add(this.conveyorWidget26);
            this.equipmentWidget7.Controls.Add(this.conveyorWidget27);
            this.equipmentWidget7.Controls.Add(this.conveyorWidget28);
            this.equipmentWidget7.EquipmentId = "";
            this.equipmentWidget7.Location = new System.Drawing.Point(8, 184);
            this.equipmentWidget7.Name = "equipmentWidget7";
            this.equipmentWidget7.Size = new System.Drawing.Size(55, 23);
            this.equipmentWidget7.TabIndex = 20;
            // 
            // conveyorWidget26
            // 
            this.conveyorWidget26.AutoSizing = false;
            this.conveyorWidget26.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget26.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget26.BorderWidth = 1;
            this.conveyorWidget26.Cursor = System.Windows.Forms.Cursors.Hand;
            this.conveyorWidget26.ElementId = null;
            this.conveyorWidget26.ErrorState = false;
            this.conveyorWidget26.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget26.ImHere = false;
            this.conveyorWidget26.IsAvailible = true;
            this.conveyorWidget26.IsReserved = false;
            this.conveyorWidget26.IsSafetySensorDetected = false;
            this.conveyorWidget26.IsShared = false;
            this.conveyorWidget26.Location = new System.Drawing.Point(34, 2);
            this.conveyorWidget26.Name = "conveyorWidget26";
            this.conveyorWidget26.OwnerId = "";
            this.conveyorWidget26.Radius = 1;
            this.conveyorWidget26.Selected = false;
            this.conveyorWidget26.Size = new System.Drawing.Size(17, 17);
            this.conveyorWidget26.TabIndex = 3;
            this.conveyorWidget26.UseMouseHover = true;
            this.conveyorWidget26.WidgetBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget26.WidgetBackGroundImage = "";
            this.conveyorWidget26.WidgetBackGroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget26.WidgetBorderStyle = System.Windows.Forms.BorderStyle.None;
            this.conveyorWidget26.WidgetData = ((ECP.Global.Gui.WidgetData)(resources.GetObject("conveyorWidget26.WidgetData")));
            this.conveyorWidget26.WidgetForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget26.WidgetText = "27";
            this.conveyorWidget26.WidgetTextAlignment = System.Drawing.ContentAlignment.TopLeft;
            this.conveyorWidget26.WidgetTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget26.WidgetTextCornerMargin = 0;
            this.conveyorWidget26.WidgetTextFont = new System.Drawing.Font("Arial", 8.25F);
            // 
            // conveyorWidget27
            // 
            this.conveyorWidget27.AutoSizing = false;
            this.conveyorWidget27.BackColor = System.Drawing.Color.Black;
            this.conveyorWidget27.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget27.BorderWidth = 1;
            this.conveyorWidget27.Cursor = System.Windows.Forms.Cursors.Hand;
            this.conveyorWidget27.ElementId = null;
            this.conveyorWidget27.ErrorState = false;
            this.conveyorWidget27.ForeColor = System.Drawing.Color.Transparent;
            this.conveyorWidget27.ImHere = false;
            this.conveyorWidget27.IsAvailible = true;
            this.conveyorWidget27.IsReserved = false;
            this.conveyorWidget27.IsSafetySensorDetected = false;
            this.conveyorWidget27.IsShared = false;
            this.conveyorWidget27.Location = new System.Drawing.Point(2, 2);
            this.conveyorWidget27.Name = "conveyorWidget27";
            this.conveyorWidget27.OwnerId = "";
            this.conveyorWidget27.Radius = 1;
            this.conveyorWidget27.Selected = false;
            this.conveyorWidget27.Size = new System.Drawing.Size(17, 17);
            this.conveyorWidget27.TabIndex = 0;
            this.conveyorWidget27.UseMouseHover = true;
            this.conveyorWidget27.WidgetBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget27.WidgetBackGroundImage = "";
            this.conveyorWidget27.WidgetBackGroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget27.WidgetBorderStyle = System.Windows.Forms.BorderStyle.None;
            this.conveyorWidget27.WidgetData = ((ECP.Global.Gui.WidgetData)(resources.GetObject("conveyorWidget27.WidgetData")));
            this.conveyorWidget27.WidgetForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.conveyorWidget27.WidgetText = "3";
            this.conveyorWidget27.WidgetTextAlignment = System.Drawing.ContentAlignment.TopLeft;
            this.conveyorWidget27.WidgetTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(255)))));
            this.conveyorWidget27.WidgetTextCornerMargin = 0;
            this.conveyorWidget27.WidgetTextFont = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            // 
            // conveyorWidget28
            // 
            this.conveyorWidget28.AutoSizing = false;
            this.conveyorWidget28.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget28.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget28.BorderWidth = 1;
            this.conveyorWidget28.Cursor = System.Windows.Forms.Cursors.Hand;
            this.conveyorWidget28.ElementId = null;
            this.conveyorWidget28.ErrorState = false;
            this.conveyorWidget28.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget28.ImHere = false;
            this.conveyorWidget28.IsAvailible = true;
            this.conveyorWidget28.IsReserved = false;
            this.conveyorWidget28.IsSafetySensorDetected = false;
            this.conveyorWidget28.IsShared = false;
            this.conveyorWidget28.Location = new System.Drawing.Point(18, 2);
            this.conveyorWidget28.Name = "conveyorWidget28";
            this.conveyorWidget28.OwnerId = "";
            this.conveyorWidget28.Radius = 1;
            this.conveyorWidget28.Selected = false;
            this.conveyorWidget28.Size = new System.Drawing.Size(17, 17);
            this.conveyorWidget28.TabIndex = 2;
            this.conveyorWidget28.UseMouseHover = true;
            this.conveyorWidget28.WidgetBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget28.WidgetBackGroundImage = "";
            this.conveyorWidget28.WidgetBackGroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget28.WidgetBorderStyle = System.Windows.Forms.BorderStyle.None;
            this.conveyorWidget28.WidgetData = ((ECP.Global.Gui.WidgetData)(resources.GetObject("conveyorWidget28.WidgetData")));
            this.conveyorWidget28.WidgetForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget28.WidgetText = "28";
            this.conveyorWidget28.WidgetTextAlignment = System.Drawing.ContentAlignment.TopLeft;
            this.conveyorWidget28.WidgetTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget28.WidgetTextCornerMargin = 0;
            this.conveyorWidget28.WidgetTextFont = new System.Drawing.Font("Arial", 8.25F);
            // 
            // equipmentWidget6
            // 
            this.equipmentWidget6.BackColor = System.Drawing.Color.Yellow;
            this.equipmentWidget6.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.equipmentWidget6.Controls.Add(this.conveyorWidget25);
            this.equipmentWidget6.Controls.Add(this.conveyorWidget23);
            this.equipmentWidget6.Controls.Add(this.conveyorWidget24);
            this.equipmentWidget6.EquipmentId = "";
            this.equipmentWidget6.Location = new System.Drawing.Point(8, 238);
            this.equipmentWidget6.Name = "equipmentWidget6";
            this.equipmentWidget6.Size = new System.Drawing.Size(55, 23);
            this.equipmentWidget6.TabIndex = 19;
            // 
            // conveyorWidget25
            // 
            this.conveyorWidget25.AutoSizing = false;
            this.conveyorWidget25.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget25.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget25.BorderWidth = 1;
            this.conveyorWidget25.Cursor = System.Windows.Forms.Cursors.Hand;
            this.conveyorWidget25.ElementId = null;
            this.conveyorWidget25.ErrorState = false;
            this.conveyorWidget25.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget25.ImHere = false;
            this.conveyorWidget25.IsAvailible = true;
            this.conveyorWidget25.IsReserved = false;
            this.conveyorWidget25.IsSafetySensorDetected = false;
            this.conveyorWidget25.IsShared = false;
            this.conveyorWidget25.Location = new System.Drawing.Point(34, 2);
            this.conveyorWidget25.Name = "conveyorWidget25";
            this.conveyorWidget25.OwnerId = "";
            this.conveyorWidget25.Radius = 1;
            this.conveyorWidget25.Selected = false;
            this.conveyorWidget25.Size = new System.Drawing.Size(17, 17);
            this.conveyorWidget25.TabIndex = 3;
            this.conveyorWidget25.UseMouseHover = true;
            this.conveyorWidget25.WidgetBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget25.WidgetBackGroundImage = "";
            this.conveyorWidget25.WidgetBackGroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget25.WidgetBorderStyle = System.Windows.Forms.BorderStyle.None;
            this.conveyorWidget25.WidgetData = ((ECP.Global.Gui.WidgetData)(resources.GetObject("conveyorWidget25.WidgetData")));
            this.conveyorWidget25.WidgetForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget25.WidgetText = "26";
            this.conveyorWidget25.WidgetTextAlignment = System.Drawing.ContentAlignment.TopLeft;
            this.conveyorWidget25.WidgetTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget25.WidgetTextCornerMargin = 0;
            this.conveyorWidget25.WidgetTextFont = new System.Drawing.Font("Arial", 8.25F);
            // 
            // conveyorWidget23
            // 
            this.conveyorWidget23.AutoSizing = false;
            this.conveyorWidget23.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget23.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget23.BorderWidth = 1;
            this.conveyorWidget23.Cursor = System.Windows.Forms.Cursors.Hand;
            this.conveyorWidget23.ElementId = null;
            this.conveyorWidget23.ErrorState = false;
            this.conveyorWidget23.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.conveyorWidget23.ImHere = false;
            this.conveyorWidget23.IsAvailible = true;
            this.conveyorWidget23.IsReserved = false;
            this.conveyorWidget23.IsSafetySensorDetected = false;
            this.conveyorWidget23.IsShared = false;
            this.conveyorWidget23.Location = new System.Drawing.Point(2, 2);
            this.conveyorWidget23.Name = "conveyorWidget23";
            this.conveyorWidget23.OwnerId = "";
            this.conveyorWidget23.Radius = 1;
            this.conveyorWidget23.Selected = false;
            this.conveyorWidget23.Size = new System.Drawing.Size(17, 17);
            this.conveyorWidget23.TabIndex = 0;
            this.conveyorWidget23.UseMouseHover = true;
            this.conveyorWidget23.WidgetBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget23.WidgetBackGroundImage = "";
            this.conveyorWidget23.WidgetBackGroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget23.WidgetBorderStyle = System.Windows.Forms.BorderStyle.None;
            this.conveyorWidget23.WidgetData = ((ECP.Global.Gui.WidgetData)(resources.GetObject("conveyorWidget23.WidgetData")));
            this.conveyorWidget23.WidgetForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.conveyorWidget23.WidgetText = "4";
            this.conveyorWidget23.WidgetTextAlignment = System.Drawing.ContentAlignment.TopLeft;
            this.conveyorWidget23.WidgetTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget23.WidgetTextCornerMargin = 0;
            this.conveyorWidget23.WidgetTextFont = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Bold);
            // 
            // conveyorWidget24
            // 
            this.conveyorWidget24.AutoSizing = false;
            this.conveyorWidget24.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget24.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget24.BorderWidth = 1;
            this.conveyorWidget24.Cursor = System.Windows.Forms.Cursors.Hand;
            this.conveyorWidget24.ElementId = null;
            this.conveyorWidget24.ErrorState = false;
            this.conveyorWidget24.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget24.ImHere = false;
            this.conveyorWidget24.IsAvailible = true;
            this.conveyorWidget24.IsReserved = false;
            this.conveyorWidget24.IsSafetySensorDetected = false;
            this.conveyorWidget24.IsShared = false;
            this.conveyorWidget24.Location = new System.Drawing.Point(18, 2);
            this.conveyorWidget24.Name = "conveyorWidget24";
            this.conveyorWidget24.OwnerId = "";
            this.conveyorWidget24.Radius = 1;
            this.conveyorWidget24.Selected = false;
            this.conveyorWidget24.Size = new System.Drawing.Size(17, 17);
            this.conveyorWidget24.TabIndex = 2;
            this.conveyorWidget24.UseMouseHover = true;
            this.conveyorWidget24.WidgetBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget24.WidgetBackGroundImage = "";
            this.conveyorWidget24.WidgetBackGroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.conveyorWidget24.WidgetBorderStyle = System.Windows.Forms.BorderStyle.None;
            this.conveyorWidget24.WidgetData = ((ECP.Global.Gui.WidgetData)(resources.GetObject("conveyorWidget24.WidgetData")));
            this.conveyorWidget24.WidgetForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(173)))), ((int)(((byte)(216)))), ((int)(((byte)(230)))));
            this.conveyorWidget24.WidgetText = "25";
            this.conveyorWidget24.WidgetTextAlignment = System.Drawing.ContentAlignment.TopLeft;
            this.conveyorWidget24.WidgetTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.conveyorWidget24.WidgetTextCornerMargin = 0;
            this.conveyorWidget24.WidgetTextFont = new System.Drawing.Font("Arial", 8.25F);
            // 
            // hiRackProcessRight
            // 
            this.hiRackProcessRight.Location = new System.Drawing.Point(61, 239);
            this.hiRackProcessRight.Name = "hiRackProcessRight";
            this.hiRackProcessRight.Size = new System.Drawing.Size(583, 200);
            this.hiRackProcessRight.Stocker = null;
            this.hiRackProcessRight.TabIndex = 16;
            // 
            // stackerCraneWidget1
            // 
            this.stackerCraneWidget1.AutoSizing = false;
            this.stackerCraneWidget1.BackColor = System.Drawing.Color.Transparent;
            this.stackerCraneWidget1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.stackerCraneWidget1.BorderWidth = 1;
            this.stackerCraneWidget1.ElementId = null;
            this.stackerCraneWidget1.ErrorState = false;
            this.stackerCraneWidget1.ForeColor = System.Drawing.Color.Transparent;
            this.stackerCraneWidget1.ImHere = false;
            this.stackerCraneWidget1.IsAvailible = true;
            this.stackerCraneWidget1.IsReserved = false;
            this.stackerCraneWidget1.IsSafetySensorDetected = false;
            this.stackerCraneWidget1.IsShared = false;
            this.stackerCraneWidget1.Location = new System.Drawing.Point(24, 0);
            this.stackerCraneWidget1.Name = "stackerCraneWidget1";
            this.stackerCraneWidget1.OwnerId = "";
            this.stackerCraneWidget1.Radius = 1;
            this.stackerCraneWidget1.Selected = false;
            this.stackerCraneWidget1.Size = new System.Drawing.Size(24, 24);
            this.stackerCraneWidget1.TabIndex = 1;
            this.stackerCraneWidget1.UseMouseHover = false;
            this.stackerCraneWidget1.WidgetBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.stackerCraneWidget1.WidgetBackGroundImage = "";
            this.stackerCraneWidget1.WidgetBackGroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.stackerCraneWidget1.WidgetBorderStyle = System.Windows.Forms.BorderStyle.None;
            this.stackerCraneWidget1.WidgetData = ((ECP.Global.Gui.WidgetData)(resources.GetObject("stackerCraneWidget1.WidgetData")));
            this.stackerCraneWidget1.WidgetForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(255)))), ((int)(((byte)(0)))));
            this.stackerCraneWidget1.WidgetText = "1";
            this.stackerCraneWidget1.WidgetTextAlignment = System.Drawing.ContentAlignment.TopLeft;
            this.stackerCraneWidget1.WidgetTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.stackerCraneWidget1.WidgetTextCornerMargin = 0;
            this.stackerCraneWidget1.WidgetTextFont = new System.Drawing.Font("Arial", 9F);
            // 
            // hiRackProcessLeft
            // 
            this.hiRackProcessLeft.Location = new System.Drawing.Point(61, 3);
            this.hiRackProcessLeft.Name = "hiRackProcessLeft";
            this.hiRackProcessLeft.Size = new System.Drawing.Size(583, 205);
            this.hiRackProcessLeft.Stocker = null;
            this.hiRackProcessLeft.TabIndex = 14;
            // 
            // HiRackDetailPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.AutoScroll = true;
            this.Controls.Add(this.equipmentWidget7);
            this.Controls.Add(this.equipmentWidget6);
            this.Controls.Add(this.labelRight);
            this.Controls.Add(this.labelLeft);
            this.Controls.Add(this.hiRackProcessRight);
            this.Controls.Add(this.panel6);
            this.Controls.Add(this.hiRackProcessLeft);
            this.Name = "HiRackDetailPanel";
            this.Size = new System.Drawing.Size(674, 452);
            this.panel6.ResumeLayout(false);
            this.equipmentWidget7.ResumeLayout(false);
            this.equipmentWidget6.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelRight;
        private System.Windows.Forms.Label labelLeft;
        private HiRackProcessRIghtPanel hiRackProcessRight;
        private System.Windows.Forms.Panel panel6;
        private HiRackProcessLeftPanel hiRackProcessLeft;
        private System.Windows.Forms.Label label1;
        private HECS.Gui.Widget.StackerCraneWidget stackerCraneWidget1;
        private HECS.Gui.Widget.EquipmentWidget equipmentWidget7;
        private HECS.Gui.Widget.ConveyorWidget conveyorWidget26;
        private HECS.Gui.Widget.ConveyorWidget conveyorWidget27;
        private HECS.Gui.Widget.ConveyorWidget conveyorWidget28;
        private HECS.Gui.Widget.EquipmentWidget equipmentWidget6;
        private HECS.Gui.Widget.ConveyorWidget conveyorWidget25;
        private HECS.Gui.Widget.ConveyorWidget conveyorWidget23;
        private HECS.Gui.Widget.ConveyorWidget conveyorWidget24;

    }
}
