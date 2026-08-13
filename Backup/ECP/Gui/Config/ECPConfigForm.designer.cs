namespace ECP.Gui.Config
{
    partial class ECPConfigForm
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

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ECPConfigForm));
            this.mainToolBar = new System.Windows.Forms.ToolStrip();
            this.exitButton = new System.Windows.Forms.ToolStripButton();
            this.saveButton = new System.Windows.Forms.ToolStripButton();
            this.DrawingTools = new System.Windows.Forms.ToolStripSeparator();
            this.arrowButton = new System.Windows.Forms.ToolStripButton();
            this.pictureButton = new System.Windows.Forms.ToolStripButton();
            this.lineButton = new System.Windows.Forms.ToolStripButton();
            this.lineArrowButton = new System.Windows.Forms.ToolStripButton();
            this.textButton = new System.Windows.Forms.ToolStripButton();
            this.Alignment = new System.Windows.Forms.ToolStripSeparator();
            this.sameDistanceButton = new System.Windows.Forms.ToolStripButton();
            this.alignTopButton = new System.Windows.Forms.ToolStripButton();
            this.alignBottomButton = new System.Windows.Forms.ToolStripButton();
            this.alignLeftButton = new System.Windows.Forms.ToolStripButton();
            this.alignRightButton = new System.Windows.Forms.ToolStripButton();
            this.centerVerticalButton = new System.Windows.Forms.ToolStripButton();
            this.centerHorizontalButton = new System.Windows.Forms.ToolStripButton();
            this.Widget = new System.Windows.Forms.ToolStripSeparator();
            this.BringToFrontButton = new System.Windows.Forms.ToolStripButton();
            this.SendToBackButton = new System.Windows.Forms.ToolStripButton();
            this.AppendFunction = new System.Windows.Forms.ToolStripSeparator();
            this.WidgetListViewer = new System.Windows.Forms.ToolStripButton();
            this.toolBarContainer = new System.Windows.Forms.SplitContainer();
            this.mainSplitContainer = new System.Windows.Forms.SplitContainer();
            this.mainToolBar.SuspendLayout();
            this.toolBarContainer.Panel1.SuspendLayout();
            this.toolBarContainer.SuspendLayout();
            this.mainSplitContainer.Panel1.SuspendLayout();
            this.mainSplitContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainToolBar
            // 
            this.mainToolBar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainToolBar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.mainToolBar.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.mainToolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitButton,
            this.saveButton,
            this.DrawingTools,
            this.arrowButton,
            this.pictureButton,
            this.lineButton,
            this.lineArrowButton,
            this.textButton,
            this.Alignment,
            this.sameDistanceButton,
            this.alignTopButton,
            this.alignBottomButton,
            this.alignLeftButton,
            this.alignRightButton,
            this.centerVerticalButton,
            this.centerHorizontalButton,
            this.Widget,
            this.BringToFrontButton,
            this.SendToBackButton,
            this.AppendFunction,
            this.WidgetListViewer});
            this.mainToolBar.Location = new System.Drawing.Point(0, 0);
            this.mainToolBar.Name = "mainToolBar";
            this.mainToolBar.Padding = new System.Windows.Forms.Padding(1);
            this.mainToolBar.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.mainToolBar.Size = new System.Drawing.Size(246, 28);
            this.mainToolBar.Stretch = true;
            this.mainToolBar.TabIndex = 0;
            this.mainToolBar.Text = "MainToolBar";
            // 
            // exitButton
            // 
            this.exitButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.exitButton.Image = ((System.Drawing.Image)(resources.GetObject("exitButton.Image")));
            this.exitButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.exitButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.exitButton.Name = "exitButton";
            this.exitButton.Size = new System.Drawing.Size(24, 23);
            this.exitButton.Text = "Exit";
            this.exitButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // saveButton
            // 
            this.saveButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveButton.Image = ((System.Drawing.Image)(resources.GetObject("saveButton.Image")));
            this.saveButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.saveButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(24, 23);
            this.saveButton.Text = "Save";
            this.saveButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // DrawingTools
            // 
            this.DrawingTools.Name = "DrawingTools";
            this.DrawingTools.Size = new System.Drawing.Size(6, 26);
            // 
            // arrowButton
            // 
            this.arrowButton.CheckOnClick = true;
            this.arrowButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.arrowButton.Image = ((System.Drawing.Image)(resources.GetObject("arrowButton.Image")));
            this.arrowButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.arrowButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.arrowButton.Name = "arrowButton";
            this.arrowButton.Size = new System.Drawing.Size(23, 23);
            this.arrowButton.Text = "ArrowButton";
            this.arrowButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // pictureButton
            // 
            this.pictureButton.CheckOnClick = true;
            this.pictureButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.pictureButton.Image = ((System.Drawing.Image)(resources.GetObject("pictureButton.Image")));
            this.pictureButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.pictureButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.pictureButton.Name = "pictureButton";
            this.pictureButton.Size = new System.Drawing.Size(23, 23);
            this.pictureButton.Text = "Picture Box";
            this.pictureButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // lineButton
            // 
            this.lineButton.CheckOnClick = true;
            this.lineButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.lineButton.Image = ((System.Drawing.Image)(resources.GetObject("lineButton.Image")));
            this.lineButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.lineButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.lineButton.Name = "lineButton";
            this.lineButton.Size = new System.Drawing.Size(24, 23);
            this.lineButton.Text = "Line Draw";
            this.lineButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // lineArrowButton
            // 
            this.lineArrowButton.CheckOnClick = true;
            this.lineArrowButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.lineArrowButton.Image = ((System.Drawing.Image)(resources.GetObject("lineArrowButton.Image")));
            this.lineArrowButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.lineArrowButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.lineArrowButton.Name = "lineArrowButton";
            this.lineArrowButton.Size = new System.Drawing.Size(23, 23);
            this.lineArrowButton.Text = "Arrow Line Draw";
            this.lineArrowButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // textButton
            // 
            this.textButton.CheckOnClick = true;
            this.textButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.textButton.Image = ((System.Drawing.Image)(resources.GetObject("textButton.Image")));
            this.textButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.textButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.textButton.Name = "textButton";
            this.textButton.Size = new System.Drawing.Size(23, 23);
            this.textButton.Text = "Text Label";
            this.textButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // Alignment
            // 
            this.Alignment.Name = "Alignment";
            this.Alignment.Size = new System.Drawing.Size(6, 26);
            // 
            // sameDistanceButton
            // 
            this.sameDistanceButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.sameDistanceButton.Image = ((System.Drawing.Image)(resources.GetObject("sameDistanceButton.Image")));
            this.sameDistanceButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.sameDistanceButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.sameDistanceButton.Name = "sameDistanceButton";
            this.sameDistanceButton.Size = new System.Drawing.Size(23, 23);
            this.sameDistanceButton.Text = "Same Distance";
            this.sameDistanceButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // alignTopButton
            // 
            this.alignTopButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.alignTopButton.Image = ((System.Drawing.Image)(resources.GetObject("alignTopButton.Image")));
            this.alignTopButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.alignTopButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.alignTopButton.Name = "alignTopButton";
            this.alignTopButton.Size = new System.Drawing.Size(23, 23);
            this.alignTopButton.Text = "Align Top";
            this.alignTopButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // alignBottomButton
            // 
            this.alignBottomButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.alignBottomButton.Image = ((System.Drawing.Image)(resources.GetObject("alignBottomButton.Image")));
            this.alignBottomButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.alignBottomButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.alignBottomButton.Name = "alignBottomButton";
            this.alignBottomButton.Size = new System.Drawing.Size(23, 0);
            this.alignBottomButton.Text = "Align Bottom";
            this.alignBottomButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // alignLeftButton
            // 
            this.alignLeftButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.alignLeftButton.Image = ((System.Drawing.Image)(resources.GetObject("alignLeftButton.Image")));
            this.alignLeftButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.alignLeftButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.alignLeftButton.Name = "alignLeftButton";
            this.alignLeftButton.Size = new System.Drawing.Size(23, 0);
            this.alignLeftButton.Text = "Align Left";
            this.alignLeftButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // alignRightButton
            // 
            this.alignRightButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.alignRightButton.Image = ((System.Drawing.Image)(resources.GetObject("alignRightButton.Image")));
            this.alignRightButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.alignRightButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.alignRightButton.Name = "alignRightButton";
            this.alignRightButton.Size = new System.Drawing.Size(23, 0);
            this.alignRightButton.Text = "Align Right";
            this.alignRightButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // centerVerticalButton
            // 
            this.centerVerticalButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.centerVerticalButton.Image = ((System.Drawing.Image)(resources.GetObject("centerVerticalButton.Image")));
            this.centerVerticalButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.centerVerticalButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.centerVerticalButton.Name = "centerVerticalButton";
            this.centerVerticalButton.Size = new System.Drawing.Size(23, 0);
            this.centerVerticalButton.Text = "Center Vertical";
            this.centerVerticalButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // centerHorizontalButton
            // 
            this.centerHorizontalButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.centerHorizontalButton.Image = ((System.Drawing.Image)(resources.GetObject("centerHorizontalButton.Image")));
            this.centerHorizontalButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.centerHorizontalButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.centerHorizontalButton.Name = "centerHorizontalButton";
            this.centerHorizontalButton.Size = new System.Drawing.Size(23, 0);
            this.centerHorizontalButton.Text = "Center Horizontal";
            this.centerHorizontalButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // Widget
            // 
            this.Widget.Name = "Widget";
            this.Widget.Size = new System.Drawing.Size(6, 0);
            // 
            // BringToFrontButton
            // 
            this.BringToFrontButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.BringToFrontButton.Image = ((System.Drawing.Image)(resources.GetObject("BringToFrontButton.Image")));
            this.BringToFrontButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.BringToFrontButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.BringToFrontButton.Name = "BringToFrontButton";
            this.BringToFrontButton.Size = new System.Drawing.Size(23, 0);
            this.BringToFrontButton.Text = "BringToFront";
            this.BringToFrontButton.ToolTipText = "Bring to front";
            this.BringToFrontButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // SendToBackButton
            // 
            this.SendToBackButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.SendToBackButton.Image = ((System.Drawing.Image)(resources.GetObject("SendToBackButton.Image")));
            this.SendToBackButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.SendToBackButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.SendToBackButton.Name = "SendToBackButton";
            this.SendToBackButton.Size = new System.Drawing.Size(23, 0);
            this.SendToBackButton.Text = "SendToBack";
            this.SendToBackButton.ToolTipText = "Send to back";
            this.SendToBackButton.Click += new System.EventHandler(this.HandleButtons);
            // 
            // AppendFunction
            // 
            this.AppendFunction.Name = "AppendFunction";
            this.AppendFunction.Size = new System.Drawing.Size(6, 0);
            // 
            // WidgetListViewer
            // 
            this.WidgetListViewer.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.WidgetListViewer.Image = ((System.Drawing.Image)(resources.GetObject("WidgetListViewer.Image")));
            this.WidgetListViewer.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.WidgetListViewer.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.WidgetListViewer.Name = "WidgetListViewer";
            this.WidgetListViewer.Size = new System.Drawing.Size(23, 0);
            this.WidgetListViewer.Text = "WidgetListViewer";
            this.WidgetListViewer.ToolTipText = "Widget List viewer";
            this.WidgetListViewer.Click += new System.EventHandler(this.HandleButtons);
            // 
            // toolBarContainer
            // 
            this.toolBarContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolBarContainer.IsSplitterFixed = true;
            this.toolBarContainer.Location = new System.Drawing.Point(0, 0);
            this.toolBarContainer.Margin = new System.Windows.Forms.Padding(0);
            this.toolBarContainer.Name = "toolBarContainer";
            // 
            // toolBarContainer.Panel1
            // 
            this.toolBarContainer.Panel1.Controls.Add(this.mainToolBar);
            this.toolBarContainer.Size = new System.Drawing.Size(550, 28);
            this.toolBarContainer.SplitterDistance = 246;
            this.toolBarContainer.SplitterWidth = 2;
            this.toolBarContainer.TabIndex = 3;
            // 
            // mainSplitContainer
            // 
            this.mainSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.mainSplitContainer.Name = "mainSplitContainer";
            this.mainSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // mainSplitContainer.Panel1
            // 
            this.mainSplitContainer.Panel1.Controls.Add(this.toolBarContainer);
            this.mainSplitContainer.Size = new System.Drawing.Size(550, 320);
            this.mainSplitContainer.SplitterDistance = 28;
            this.mainSplitContainer.SplitterWidth = 1;
            this.mainSplitContainer.TabIndex = 4;
            // 
            // ECPConfigForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(550, 320);
            this.Controls.Add(this.mainSplitContainer);
            this.Name = "ECPConfigForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Stocker Configuration";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.ECPConfigForm_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.WindowFormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.WindowFormClosing);
            this.mainToolBar.ResumeLayout(false);
            this.mainToolBar.PerformLayout();
            this.toolBarContainer.Panel1.ResumeLayout(false);
            this.toolBarContainer.Panel1.PerformLayout();
            this.toolBarContainer.ResumeLayout(false);
            this.mainSplitContainer.Panel1.ResumeLayout(false);
            this.mainSplitContainer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStrip mainToolBar;
        private System.Windows.Forms.ToolStripButton exitButton;
        private System.Windows.Forms.ToolStripButton saveButton;
        private System.Windows.Forms.ToolStripButton arrowButton;
        private System.Windows.Forms.ToolStripButton pictureButton;
        private System.Windows.Forms.ToolStripButton lineButton;
        private System.Windows.Forms.ToolStripButton lineArrowButton;
        public System.Windows.Forms.SplitContainer toolBarContainer;
        protected System.Windows.Forms.SplitContainer mainSplitContainer;
        private System.Windows.Forms.ToolStripButton textButton;
        private System.Windows.Forms.ToolStripButton sameDistanceButton;
        private System.Windows.Forms.ToolStripButton alignTopButton;
        private System.Windows.Forms.ToolStripButton alignBottomButton;
        private System.Windows.Forms.ToolStripButton alignLeftButton;
        private System.Windows.Forms.ToolStripButton alignRightButton;
        private System.Windows.Forms.ToolStripButton centerVerticalButton;
        private System.Windows.Forms.ToolStripButton centerHorizontalButton;
        private System.Windows.Forms.ToolStripSeparator DrawingTools;
        private System.Windows.Forms.ToolStripSeparator Alignment;
        private System.Windows.Forms.ToolStripSeparator Widget;
        private System.Windows.Forms.ToolStripButton BringToFrontButton;
        private System.Windows.Forms.ToolStripButton SendToBackButton;
        private System.Windows.Forms.ToolStripButton WidgetListViewer;
        private System.Windows.Forms.ToolStripSeparator AppendFunction;

    }
}