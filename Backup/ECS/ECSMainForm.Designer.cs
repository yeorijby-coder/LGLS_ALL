namespace HECS
{
    partial class ECSMainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ECSMainForm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.TSMIMovement = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMILocalMovement = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIMovementMonitoring = new System.Windows.Forms.ToolStripMenuItem();
            this.formTestToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIInventory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIHiRackStatus = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIErrorHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISAPIFHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISystem = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIMovementRule = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISystemInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIWindowList = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonLocalMovement = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonMovementMonitroing = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonHiRackStatus = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonErrorHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSAPIFHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonMovementRule = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSystemInfo = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.menuStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.Color.White;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMIMovement,
            this.TSMIInventory,
            this.TSMIHistory,
            this.TSMISystem,
            this.TSMIWindowList});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(937, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // TSMIMovement
            // 
            this.TSMIMovement.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMILocalMovement,
            this.TSMIMovementMonitoring,
            this.formTestToolStripMenuItem});
            this.TSMIMovement.Name = "TSMIMovement";
            this.TSMIMovement.Size = new System.Drawing.Size(81, 20);
            this.TSMIMovement.Text = "입출고 관리";
            // 
            // TSMILocalMovement
            // 
            this.TSMILocalMovement.Name = "TSMILocalMovement";
            this.TSMILocalMovement.Size = new System.Drawing.Size(192, 22);
            this.TSMILocalMovement.Text = "이동 요청 관리";
            this.TSMILocalMovement.Click += new System.EventHandler(this.TSMILocalMovement_Click);
            // 
            // TSMIMovementMonitoring
            // 
            this.TSMIMovementMonitoring.Name = "TSMIMovementMonitoring";
            this.TSMIMovementMonitoring.Size = new System.Drawing.Size(192, 22);
            this.TSMIMovementMonitoring.Text = "입출고 현황 모니터링";
            this.TSMIMovementMonitoring.Click += new System.EventHandler(this.TSMIMovementMonitoring_Click);
            // 
            // formTestToolStripMenuItem
            // 
            this.formTestToolStripMenuItem.Name = "formTestToolStripMenuItem";
            this.formTestToolStripMenuItem.Size = new System.Drawing.Size(192, 22);
            this.formTestToolStripMenuItem.Text = "Form Test";
            this.formTestToolStripMenuItem.Visible = false;
            this.formTestToolStripMenuItem.Click += new System.EventHandler(this.formTestToolStripMenuItem_Click);
            // 
            // TSMIInventory
            // 
            this.TSMIInventory.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMIHiRackStatus});
            this.TSMIInventory.Name = "TSMIInventory";
            this.TSMIInventory.Size = new System.Drawing.Size(69, 20);
            this.TSMIInventory.Text = "재고 관리";
            this.TSMIInventory.Visible = false;
            // 
            // TSMIHiRackStatus
            // 
            this.TSMIHiRackStatus.Name = "TSMIHiRackStatus";
            this.TSMIHiRackStatus.Size = new System.Drawing.Size(177, 22);
            this.TSMIHiRackStatus.Text = "Hi-Rack 현황 관리";
            this.TSMIHiRackStatus.Click += new System.EventHandler(this.TSMIHiRackStatus_Click);
            // 
            // TSMIHistory
            // 
            this.TSMIHistory.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMIErrorHistory,
            this.TSMISAPIFHistory});
            this.TSMIHistory.Name = "TSMIHistory";
            this.TSMIHistory.Size = new System.Drawing.Size(97, 20);
            this.TSMIHistory.Text = "작업 이력 관리";
            // 
            // TSMIErrorHistory
            // 
            this.TSMIErrorHistory.Name = "TSMIErrorHistory";
            this.TSMIErrorHistory.Size = new System.Drawing.Size(176, 22);
            this.TSMIErrorHistory.Text = "오류 이력 조회";
            this.TSMIErrorHistory.Click += new System.EventHandler(this.TSMIErrorHistory_Click);
            // 
            // TSMISAPIFHistory
            // 
            this.TSMISAPIFHistory.Name = "TSMISAPIFHistory";
            this.TSMISAPIFHistory.Size = new System.Drawing.Size(176, 22);
            this.TSMISAPIFHistory.Text = "SAP I/F 이력 조회";
            this.TSMISAPIFHistory.Visible = false;
            this.TSMISAPIFHistory.Click += new System.EventHandler(this.TSMISAPIFHistory_Click);
            // 
            // TSMISystem
            // 
            this.TSMISystem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMIMovementRule,
            this.TSMISystemInfo});
            this.TSMISystem.Name = "TSMISystem";
            this.TSMISystem.Size = new System.Drawing.Size(81, 20);
            this.TSMISystem.Text = "시스템 관리";
            this.TSMISystem.Visible = false;
            // 
            // TSMIMovementRule
            // 
            this.TSMIMovementRule.Name = "TSMIMovementRule";
            this.TSMIMovementRule.Size = new System.Drawing.Size(184, 22);
            this.TSMIMovementRule.Text = "반송 최적화 룰 관리";
            this.TSMIMovementRule.Click += new System.EventHandler(this.TSMIMovementRule_Click);
            // 
            // TSMISystemInfo
            // 
            this.TSMISystemInfo.Name = "TSMISystemInfo";
            this.TSMISystemInfo.Size = new System.Drawing.Size(184, 22);
            this.TSMISystemInfo.Text = "시스템 정보 관리";
            this.TSMISystemInfo.Click += new System.EventHandler(this.TSMISystemInfo_Click);
            // 
            // TSMIWindowList
            // 
            this.TSMIWindowList.Name = "TSMIWindowList";
            this.TSMIWindowList.Size = new System.Drawing.Size(61, 20);
            this.TSMIWindowList.Text = "Window";
            // 
            // toolStrip1
            // 
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.BackColor = System.Drawing.Color.White;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonLocalMovement,
            this.toolStripButtonMovementMonitroing,
            this.toolStripSeparator1,
            this.toolStripButtonHiRackStatus,
            this.toolStripSeparator3,
            this.toolStripButtonErrorHistory,
            this.toolStripButtonSAPIFHistory,
            this.toolStripSeparator2,
            this.toolStripButtonMovementRule,
            this.toolStripButtonSystemInfo,
            this.toolStripSeparator4});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(937, 37);
            this.toolStrip1.TabIndex = 4;
            this.toolStrip1.Text = "바로가기 버튼";
            // 
            // toolStripButtonLocalMovement
            // 
            this.toolStripButtonLocalMovement.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonLocalMovement.Image = global::HECS.Properties.Resources.bullet53483900;
            this.toolStripButtonLocalMovement.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonLocalMovement.Name = "toolStripButtonLocalMovement";
            this.toolStripButtonLocalMovement.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonLocalMovement.Text = "이동요청관리";
            this.toolStripButtonLocalMovement.Click += new System.EventHandler(this.toolStripButtonLocalMovement_Click);
            // 
            // toolStripButtonMovementMonitroing
            // 
            this.toolStripButtonMovementMonitroing.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonMovementMonitroing.Image = global::HECS.Properties.Resources.bullet53515300;
            this.toolStripButtonMovementMonitroing.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMovementMonitroing.Name = "toolStripButtonMovementMonitroing";
            this.toolStripButtonMovementMonitroing.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonMovementMonitroing.Text = "입출고 현황 모니터링";
            this.toolStripButtonMovementMonitroing.Click += new System.EventHandler(this.toolStripButtonMovementMonitroing_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 37);
            // 
            // toolStripButtonHiRackStatus
            // 
            this.toolStripButtonHiRackStatus.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonHiRackStatus.Image = global::HECS.Properties.Resources.bullet50202700;
            this.toolStripButtonHiRackStatus.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonHiRackStatus.Name = "toolStripButtonHiRackStatus";
            this.toolStripButtonHiRackStatus.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonHiRackStatus.Text = "하이랙 현황 관리";
            this.toolStripButtonHiRackStatus.Visible = false;
            this.toolStripButtonHiRackStatus.Click += new System.EventHandler(this.toolStripButtonHiRackStatus_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 37);
            // 
            // toolStripButtonErrorHistory
            // 
            this.toolStripButtonErrorHistory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonErrorHistory.Image = global::HECS.Properties.Resources.bullet53484100;
            this.toolStripButtonErrorHistory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonErrorHistory.Name = "toolStripButtonErrorHistory";
            this.toolStripButtonErrorHistory.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonErrorHistory.Text = "오류 이력 조회";
            this.toolStripButtonErrorHistory.Click += new System.EventHandler(this.toolStripButtonErrorHistory_Click);
            // 
            // toolStripButtonSAPIFHistory
            // 
            this.toolStripButtonSAPIFHistory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSAPIFHistory.Image = global::HECS.Properties.Resources.bullet50261700;
            this.toolStripButtonSAPIFHistory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSAPIFHistory.Name = "toolStripButtonSAPIFHistory";
            this.toolStripButtonSAPIFHistory.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonSAPIFHistory.Text = "SAP 인터페이스 이력 조회";
            this.toolStripButtonSAPIFHistory.Visible = false;
            this.toolStripButtonSAPIFHistory.Click += new System.EventHandler(this.toolStripButtonSAPIFHistory_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 37);
            // 
            // toolStripButtonMovementRule
            // 
            this.toolStripButtonMovementRule.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonMovementRule.Image = global::HECS.Properties.Resources.Guage;
            this.toolStripButtonMovementRule.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMovementRule.Name = "toolStripButtonMovementRule";
            this.toolStripButtonMovementRule.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonMovementRule.Text = "입출고 최적화 룰 관리";
            this.toolStripButtonMovementRule.Visible = false;
            this.toolStripButtonMovementRule.Click += new System.EventHandler(this.toolStripButtonMovementRule_Click);
            // 
            // toolStripButtonSystemInfo
            // 
            this.toolStripButtonSystemInfo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSystemInfo.Image = global::HECS.Properties.Resources.bullet500556001;
            this.toolStripButtonSystemInfo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSystemInfo.Name = "toolStripButtonSystemInfo";
            this.toolStripButtonSystemInfo.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonSystemInfo.Text = "시스템 정보 관리";
            this.toolStripButtonSystemInfo.Visible = false;
            this.toolStripButtonSystemInfo.Click += new System.EventHandler(this.toolStripButtonSystemInfo_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 37);
            // 
            // ECSMainForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.AutoScrollMargin = new System.Drawing.Size(100, 100);
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(937, 584);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "ECSMainForm";
            this.Text = "ECS Main Menu";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.ECSMainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem TSMIMovement;
        private System.Windows.Forms.ToolStripMenuItem TSMILocalMovement;
        private System.Windows.Forms.ToolStripMenuItem TSMIInventory;
        private System.Windows.Forms.ToolStripMenuItem TSMIHiRackStatus;
        private System.Windows.Forms.ToolStripMenuItem TSMIHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMIErrorHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMISystem;
        private System.Windows.Forms.ToolStripMenuItem TSMISAPIFHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMIMovementRule;
        private System.Windows.Forms.ToolStripMenuItem TSMISystemInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIWindowList;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButtonLocalMovement;
        private System.Windows.Forms.ToolStripButton toolStripButtonMovementMonitroing;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButtonHiRackStatus;
        private System.Windows.Forms.ToolStripButton toolStripButtonErrorHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonSAPIFHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonMovementRule;
        private System.Windows.Forms.ToolStripButton toolStripButtonSystemInfo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem TSMIMovementMonitoring;
        private System.Windows.Forms.ToolStripMenuItem formTestToolStripMenuItem;
    }
}

