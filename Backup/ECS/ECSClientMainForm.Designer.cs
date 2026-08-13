namespace HECS
{
    partial class ECSClientMainForm
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.TSMIMovement = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISapMovement = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMILocalMovement = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIECSMovement = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIPalletBlocking = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIMovementMonitroing = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMITransferTimeMonitorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISAPMovementIF = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIInventory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIPalletStatus = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIHiRackStatus = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIMovementHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIPalletHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIPalletBlockingHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIHiRackHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIErrorHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISAPIFHistory = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISystem = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIUserManagement = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIUserInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIProgramInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIRoleInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIRoleForUser = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIProgramForRole = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIMovementRule = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMISystemInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIDeviceInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIMaterialInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIBatchInfo = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMIWindowList = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButtonSapMovement = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonLocalMovement = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonECSMovement = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonPalletBlocking = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonMovementMonitroing = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSAPMovementIF = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonPalletStatus = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonHiRackStatus = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonMovementHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonPalletHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonPalletBlockingHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonHiRackHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonErrorHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSAPIFHistory = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonMovementRule = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSystemInfo = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonDeviceInfo = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonMaterialInfo = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonBatchInfo = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonAbout = new System.Windows.Forms.ToolStripButton();
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
            this.TSMISapMovement,
            this.TSMILocalMovement,
            this.TSMIECSMovement,
            this.TSMIPalletBlocking,
            this.TSMIMovementMonitroing,
            this.TSMITransferTimeMonitorToolStripMenuItem,
            this.TSMISAPMovementIF});
            this.TSMIMovement.Name = "TSMIMovement";
            this.TSMIMovement.Size = new System.Drawing.Size(81, 20);
            this.TSMIMovement.Text = "입출고 관리";
            // 
            // TSMISapMovement
            // 
            this.TSMISapMovement.Name = "TSMISapMovement";
            this.TSMISapMovement.Size = new System.Drawing.Size(256, 22);
            this.TSMISapMovement.Text = "입출고 현황 관리";
            this.TSMISapMovement.Click += new System.EventHandler(this.TSMISapMovement_Click);
            // 
            // TSMILocalMovement
            // 
            this.TSMILocalMovement.Name = "TSMILocalMovement";
            this.TSMILocalMovement.Size = new System.Drawing.Size(256, 22);
            this.TSMILocalMovement.Text = "이동 요청 관리";
            this.TSMILocalMovement.Click += new System.EventHandler(this.TSMILocalMovement_Click);
            // 
            // TSMIECSMovement
            // 
            this.TSMIECSMovement.Name = "TSMIECSMovement";
            this.TSMIECSMovement.Size = new System.Drawing.Size(256, 22);
            this.TSMIECSMovement.Text = "ECS 입출고 요청 관리";
            this.TSMIECSMovement.Click += new System.EventHandler(this.TSMIECSMovement_Click);
            // 
            // TSMIPalletBlocking
            // 
            this.TSMIPalletBlocking.Name = "TSMIPalletBlocking";
            this.TSMIPalletBlocking.Size = new System.Drawing.Size(256, 22);
            this.TSMIPalletBlocking.Text = "Pallet Blocking 관리";
            this.TSMIPalletBlocking.Click += new System.EventHandler(this.TSMIPalletBlocking_Click);
            // 
            // TSMIMovementMonitroing
            // 
            this.TSMIMovementMonitroing.Name = "TSMIMovementMonitroing";
            this.TSMIMovementMonitroing.Size = new System.Drawing.Size(256, 22);
            this.TSMIMovementMonitroing.Text = "입출고 현황 모니터링";
            this.TSMIMovementMonitroing.Click += new System.EventHandler(this.TSMIMovementMonitoring_Click);
            // 
            // TSMITransferTimeMonitorToolStripMenuItem
            // 
            this.TSMITransferTimeMonitorToolStripMenuItem.Name = "TSMITransferTimeMonitorToolStripMenuItem";
            this.TSMITransferTimeMonitorToolStripMenuItem.Size = new System.Drawing.Size(256, 22);
            this.TSMITransferTimeMonitorToolStripMenuItem.Text = "입출고 요청별 대기시간 모니터링";
            this.TSMITransferTimeMonitorToolStripMenuItem.Click += new System.EventHandler(this.TSMITransferTimeMonitorToolStripMenuItem_Click);
            // 
            // TSMISAPMovementIF
            // 
            this.TSMISAPMovementIF.Name = "TSMISAPMovementIF";
            this.TSMISAPMovementIF.Size = new System.Drawing.Size(256, 22);
            this.TSMISAPMovementIF.Text = "입출고 요청 I/F 관리";
            this.TSMISAPMovementIF.Click += new System.EventHandler(this.TSMISAPMovementIF_Click);
            // 
            // TSMIInventory
            // 
            this.TSMIInventory.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMIPalletStatus,
            this.TSMIHiRackStatus});
            this.TSMIInventory.Name = "TSMIInventory";
            this.TSMIInventory.Size = new System.Drawing.Size(69, 20);
            this.TSMIInventory.Text = "재고 관리";
            // 
            // TSMIPalletStatus
            // 
            this.TSMIPalletStatus.Name = "TSMIPalletStatus";
            this.TSMIPalletStatus.Size = new System.Drawing.Size(177, 22);
            this.TSMIPalletStatus.Text = "Pallet 현황 관리";
            this.TSMIPalletStatus.Click += new System.EventHandler(this.TSMIPalletStatus_Click);
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
            this.TSMIMovementHistory,
            this.TSMIPalletHistory,
            this.TSMIPalletBlockingHistory,
            this.TSMIHiRackHistory,
            this.TSMIErrorHistory,
            this.TSMISAPIFHistory});
            this.TSMIHistory.Name = "TSMIHistory";
            this.TSMIHistory.Size = new System.Drawing.Size(97, 20);
            this.TSMIHistory.Text = "작업 이력 관리";
            // 
            // TSMIMovementHistory
            // 
            this.TSMIMovementHistory.Name = "TSMIMovementHistory";
            this.TSMIMovementHistory.Size = new System.Drawing.Size(215, 22);
            this.TSMIMovementHistory.Text = "입출고 이력 조회";
            this.TSMIMovementHistory.Click += new System.EventHandler(this.TSMIMovementHistory_Click);
            // 
            // TSMIPalletHistory
            // 
            this.TSMIPalletHistory.Name = "TSMIPalletHistory";
            this.TSMIPalletHistory.Size = new System.Drawing.Size(215, 22);
            this.TSMIPalletHistory.Text = "Pallet 작업 이력 조회";
            this.TSMIPalletHistory.Click += new System.EventHandler(this.TSMIPalletHistory_Click);
            // 
            // TSMIPalletBlockingHistory
            // 
            this.TSMIPalletBlockingHistory.Name = "TSMIPalletBlockingHistory";
            this.TSMIPalletBlockingHistory.Size = new System.Drawing.Size(215, 22);
            this.TSMIPalletBlockingHistory.Text = "Pallet Blocking 이력 조회";
            this.TSMIPalletBlockingHistory.Click += new System.EventHandler(this.TSMIPalletBlockingHistory_Click);
            // 
            // TSMIHiRackHistory
            // 
            this.TSMIHiRackHistory.Name = "TSMIHiRackHistory";
            this.TSMIHiRackHistory.Size = new System.Drawing.Size(215, 22);
            this.TSMIHiRackHistory.Text = "Hi-Rack 재고 이력 조회";
            this.TSMIHiRackHistory.Click += new System.EventHandler(this.TSMIHiRackHistory_Click);
            // 
            // TSMIErrorHistory
            // 
            this.TSMIErrorHistory.Name = "TSMIErrorHistory";
            this.TSMIErrorHistory.Size = new System.Drawing.Size(215, 22);
            this.TSMIErrorHistory.Text = "오류 이력 조회";
            this.TSMIErrorHistory.Click += new System.EventHandler(this.TSMIErrorHistory_Click);
            // 
            // TSMISAPIFHistory
            // 
            this.TSMISAPIFHistory.Name = "TSMISAPIFHistory";
            this.TSMISAPIFHistory.Size = new System.Drawing.Size(215, 22);
            this.TSMISAPIFHistory.Text = "SAP I/F 이력 조회";
            this.TSMISAPIFHistory.Click += new System.EventHandler(this.TSMISAPIFHistory_Click);
            // 
            // TSMISystem
            // 
            this.TSMISystem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMIUserManagement,
            this.TSMIMovementRule,
            this.TSMISystemInfo,
            this.TSMIDeviceInfo,
            this.TSMIMaterialInfo,
            this.TSMIBatchInfo});
            this.TSMISystem.Name = "TSMISystem";
            this.TSMISystem.Size = new System.Drawing.Size(81, 20);
            this.TSMISystem.Text = "시스템 관리";
            // 
            // TSMIUserManagement
            // 
            this.TSMIUserManagement.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMIUserInfo,
            this.TSMIProgramInfo,
            this.TSMIRoleInfo,
            this.TSMIRoleForUser,
            this.TSMIProgramForRole});
            this.TSMIUserManagement.Name = "TSMIUserManagement";
            this.TSMIUserManagement.Size = new System.Drawing.Size(184, 22);
            this.TSMIUserManagement.Text = "사용자 관리";
            this.TSMIUserManagement.Visible = false;
            // 
            // TSMIUserInfo
            // 
            this.TSMIUserInfo.Name = "TSMIUserInfo";
            this.TSMIUserInfo.Size = new System.Drawing.Size(193, 22);
            this.TSMIUserInfo.Text = "사용자 정보 관리";
            this.TSMIUserInfo.Click += new System.EventHandler(this.TSMIUserInfo_Click);
            // 
            // TSMIProgramInfo
            // 
            this.TSMIProgramInfo.Name = "TSMIProgramInfo";
            this.TSMIProgramInfo.Size = new System.Drawing.Size(193, 22);
            this.TSMIProgramInfo.Text = "프로그램 관리";
            this.TSMIProgramInfo.Click += new System.EventHandler(this.TSMIProgramInfo_Click);
            // 
            // TSMIRoleInfo
            // 
            this.TSMIRoleInfo.Name = "TSMIRoleInfo";
            this.TSMIRoleInfo.Size = new System.Drawing.Size(193, 22);
            this.TSMIRoleInfo.Text = "Role 관리";
            this.TSMIRoleInfo.Click += new System.EventHandler(this.TSMIRoleInfo_Click);
            // 
            // TSMIRoleForUser
            // 
            this.TSMIRoleForUser.Name = "TSMIRoleForUser";
            this.TSMIRoleForUser.Size = new System.Drawing.Size(193, 22);
            this.TSMIRoleForUser.Text = "사용자별 Role 관리";
            this.TSMIRoleForUser.Click += new System.EventHandler(this.TSMIRoleForUser_Click);
            // 
            // TSMIProgramForRole
            // 
            this.TSMIProgramForRole.Name = "TSMIProgramForRole";
            this.TSMIProgramForRole.Size = new System.Drawing.Size(193, 22);
            this.TSMIProgramForRole.Text = "Role별 프로그램 관리";
            this.TSMIProgramForRole.Click += new System.EventHandler(this.TSMIProgramForRole_Click);
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
            // TSMIDeviceInfo
            // 
            this.TSMIDeviceInfo.Name = "TSMIDeviceInfo";
            this.TSMIDeviceInfo.Size = new System.Drawing.Size(184, 22);
            this.TSMIDeviceInfo.Text = "장비 정보 관리";
            this.TSMIDeviceInfo.Click += new System.EventHandler(this.TSMIDeviceInfo_Click);
            // 
            // TSMIMaterialInfo
            // 
            this.TSMIMaterialInfo.Name = "TSMIMaterialInfo";
            this.TSMIMaterialInfo.Size = new System.Drawing.Size(184, 22);
            this.TSMIMaterialInfo.Text = "자재 정보 관리";
            this.TSMIMaterialInfo.Click += new System.EventHandler(this.TSMIMaterialInfo_Click);
            // 
            // TSMIBatchInfo
            // 
            this.TSMIBatchInfo.Name = "TSMIBatchInfo";
            this.TSMIBatchInfo.Size = new System.Drawing.Size(184, 22);
            this.TSMIBatchInfo.Text = "BATCH 정보 관리";
            this.TSMIBatchInfo.Click += new System.EventHandler(this.TSMIBatchInfo_Click);
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
            this.toolStripButtonSapMovement,
            this.toolStripButtonLocalMovement,
            this.toolStripButtonECSMovement,
            this.toolStripButtonPalletBlocking,
            this.toolStripButtonMovementMonitroing,
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem,
            this.toolStripButtonSAPMovementIF,
            this.toolStripSeparator1,
            this.toolStripButtonPalletStatus,
            this.toolStripButtonHiRackStatus,
            this.toolStripSeparator3,
            this.toolStripButtonMovementHistory,
            this.toolStripButtonPalletHistory,
            this.toolStripButtonPalletBlockingHistory,
            this.toolStripButtonHiRackHistory,
            this.toolStripButtonErrorHistory,
            this.toolStripButtonSAPIFHistory,
            this.toolStripSeparator2,
            this.toolStripButtonMovementRule,
            this.toolStripButtonSystemInfo,
            this.toolStripButtonDeviceInfo,
            this.toolStripButtonMaterialInfo,
            this.toolStripButtonBatchInfo,
            this.toolStripSeparator4,
            this.toolStripButtonAbout});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(937, 37);
            this.toolStrip1.TabIndex = 4;
            this.toolStrip1.Text = "바로가기 버튼";
            // 
            // toolStripButtonSapMovement
            // 
            this.toolStripButtonSapMovement.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSapMovement.Image = global::HECS.Properties.Resources.bullet53479000;
            this.toolStripButtonSapMovement.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSapMovement.Name = "toolStripButtonSapMovement";
            this.toolStripButtonSapMovement.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonSapMovement.Text = "입출고 현황관리";
            this.toolStripButtonSapMovement.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.toolStripButtonSapMovement.TextImageRelation = System.Windows.Forms.TextImageRelation.Overlay;
            this.toolStripButtonSapMovement.Click += new System.EventHandler(this.toolStripButtonSapMovement_Click);
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
            // toolStripButtonECSMovement
            // 
            this.toolStripButtonECSMovement.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonECSMovement.Image = global::HECS.Properties.Resources.bullet53483300;
            this.toolStripButtonECSMovement.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonECSMovement.Name = "toolStripButtonECSMovement";
            this.toolStripButtonECSMovement.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonECSMovement.Text = "ECS 입출고 요청 관리";
            this.toolStripButtonECSMovement.Click += new System.EventHandler(this.toolStripButtonECSMovement_Click);
            // 
            // toolStripButtonPalletBlocking
            // 
            this.toolStripButtonPalletBlocking.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonPalletBlocking.Image = global::HECS.Properties.Resources.bullet53433100;
            this.toolStripButtonPalletBlocking.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonPalletBlocking.Name = "toolStripButtonPalletBlocking";
            this.toolStripButtonPalletBlocking.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonPalletBlocking.Text = "Pallet Blocking 관리";
            this.toolStripButtonPalletBlocking.Click += new System.EventHandler(this.toolStripButtonPalletBlocking_Click);
            // 
            // toolStripButtonMovementMonitroing
            // 
            this.toolStripButtonMovementMonitroing.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonMovementMonitroing.Image = global::HECS.Properties.Resources.bullet53471200;
            this.toolStripButtonMovementMonitroing.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMovementMonitroing.Name = "toolStripButtonMovementMonitroing";
            this.toolStripButtonMovementMonitroing.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonMovementMonitroing.Text = "입출고 현황 모니터링";
            this.toolStripButtonMovementMonitroing.Click += new System.EventHandler(this.toolStripButtonMovementMonitroing_Click);
            // 
            // toolStripButtonTransferTimeMonitorToolStripMenuItem
            // 
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem.Image = global::HECS.Properties.Resources.bullet53515300;
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem.Name = "toolStripButtonTransferTimeMonitorToolStripMenuItem";
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem.Text = "입출고 요청별 대기시간 모니터링";
            this.toolStripButtonTransferTimeMonitorToolStripMenuItem.Click += new System.EventHandler(this.toolStripButtonTransferTimeMonitorToolStripMenuItem_Click);
            // 
            // toolStripButtonSAPMovementIF
            // 
            this.toolStripButtonSAPMovementIF.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSAPMovementIF.Image = global::HECS.Properties.Resources.bullet53462100;
            this.toolStripButtonSAPMovementIF.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSAPMovementIF.Name = "toolStripButtonSAPMovementIF";
            this.toolStripButtonSAPMovementIF.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonSAPMovementIF.Text = "입출고 요청 I/F 관리";
            this.toolStripButtonSAPMovementIF.Click += new System.EventHandler(this.toolStripButtonSAPMovementIF_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 37);
            // 
            // toolStripButtonPalletStatus
            // 
            this.toolStripButtonPalletStatus.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonPalletStatus.Image = global::HECS.Properties.Resources.bullet53473600;
            this.toolStripButtonPalletStatus.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonPalletStatus.Name = "toolStripButtonPalletStatus";
            this.toolStripButtonPalletStatus.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonPalletStatus.Text = "Pallet 현황 관리";
            this.toolStripButtonPalletStatus.Click += new System.EventHandler(this.toolStripButtonPalletStatus_Click);
            // 
            // toolStripButtonHiRackStatus
            // 
            this.toolStripButtonHiRackStatus.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonHiRackStatus.Image = global::HECS.Properties.Resources.bullet50202700;
            this.toolStripButtonHiRackStatus.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonHiRackStatus.Name = "toolStripButtonHiRackStatus";
            this.toolStripButtonHiRackStatus.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonHiRackStatus.Text = "하이랙 현황 관리";
            this.toolStripButtonHiRackStatus.Click += new System.EventHandler(this.toolStripButtonHiRackStatus_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 37);
            // 
            // toolStripButtonMovementHistory
            // 
            this.toolStripButtonMovementHistory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonMovementHistory.Image = global::HECS.Properties.Resources.bullet50045400;
            this.toolStripButtonMovementHistory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMovementHistory.Name = "toolStripButtonMovementHistory";
            this.toolStripButtonMovementHistory.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonMovementHistory.Text = "입출고 이력 조회";
            this.toolStripButtonMovementHistory.Click += new System.EventHandler(this.toolStripButtonMovementHistory_Click);
            // 
            // toolStripButtonPalletHistory
            // 
            this.toolStripButtonPalletHistory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonPalletHistory.Image = global::HECS.Properties.Resources.bullet53461600;
            this.toolStripButtonPalletHistory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonPalletHistory.Name = "toolStripButtonPalletHistory";
            this.toolStripButtonPalletHistory.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonPalletHistory.Text = "Pallet 작업 이력 조회";
            this.toolStripButtonPalletHistory.Click += new System.EventHandler(this.toolStripButtonPalletHistory_Click);
            // 
            // toolStripButtonPalletBlockingHistory
            // 
            this.toolStripButtonPalletBlockingHistory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonPalletBlockingHistory.Image = global::HECS.Properties.Resources.bullet53462200;
            this.toolStripButtonPalletBlockingHistory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonPalletBlockingHistory.Name = "toolStripButtonPalletBlockingHistory";
            this.toolStripButtonPalletBlockingHistory.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonPalletBlockingHistory.Text = "Pallet Blocking 이력 조회";
            this.toolStripButtonPalletBlockingHistory.Click += new System.EventHandler(this.toolStripButtonPalletBlockingHistory_Click);
            // 
            // toolStripButtonHiRackHistory
            // 
            this.toolStripButtonHiRackHistory.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonHiRackHistory.Image = global::HECS.Properties.Resources.bullet53464900;
            this.toolStripButtonHiRackHistory.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonHiRackHistory.Name = "toolStripButtonHiRackHistory";
            this.toolStripButtonHiRackHistory.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonHiRackHistory.Text = "하이랙 재고 이력 조회";
            this.toolStripButtonHiRackHistory.Click += new System.EventHandler(this.toolStripButtonHiRackHistory_Click);
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
            this.toolStripButtonSystemInfo.Click += new System.EventHandler(this.toolStripButtonSystemInfo_Click);
            // 
            // toolStripButtonDeviceInfo
            // 
            this.toolStripButtonDeviceInfo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonDeviceInfo.Image = global::HECS.Properties.Resources.Inform2;
            this.toolStripButtonDeviceInfo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonDeviceInfo.Name = "toolStripButtonDeviceInfo";
            this.toolStripButtonDeviceInfo.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonDeviceInfo.Text = "장비 정보 관리";
            this.toolStripButtonDeviceInfo.Click += new System.EventHandler(this.toolStripButtonDeviceInfo_Click);
            // 
            // toolStripButtonMaterialInfo
            // 
            this.toolStripButtonMaterialInfo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonMaterialInfo.Image = global::HECS.Properties.Resources.bullet53501300;
            this.toolStripButtonMaterialInfo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMaterialInfo.Name = "toolStripButtonMaterialInfo";
            this.toolStripButtonMaterialInfo.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonMaterialInfo.Text = "자재 정보 관리";
            this.toolStripButtonMaterialInfo.Click += new System.EventHandler(this.toolStripButtonMaterialInfo_Click);
            // 
            // toolStripButtonBatchInfo
            // 
            this.toolStripButtonBatchInfo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonBatchInfo.Image = global::HECS.Properties.Resources.bullet53512700;
            this.toolStripButtonBatchInfo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonBatchInfo.Name = "toolStripButtonBatchInfo";
            this.toolStripButtonBatchInfo.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonBatchInfo.Text = "BATCH 정보 관리";
            this.toolStripButtonBatchInfo.Click += new System.EventHandler(this.toolStripButtonBatchInfo_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 37);
            // 
            // toolStripButtonAbout
            // 
            this.toolStripButtonAbout.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonAbout.Image = global::HECS.Properties.Resources.bullet53484500;
            this.toolStripButtonAbout.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonAbout.Name = "toolStripButtonAbout";
            this.toolStripButtonAbout.Size = new System.Drawing.Size(36, 34);
            this.toolStripButtonAbout.Text = "한독 ECS 정보";
            // 
            // ECSClientMainForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.AutoScrollMargin = new System.Drawing.Size(100, 100);
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(937, 676);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "ECSClientMainForm";
            this.Text = "한독약품 ECS Client";
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
        private System.Windows.Forms.ToolStripMenuItem TSMISapMovement;
        private System.Windows.Forms.ToolStripMenuItem TSMILocalMovement;
        private System.Windows.Forms.ToolStripMenuItem TSMIECSMovement;
        private System.Windows.Forms.ToolStripMenuItem TSMIPalletBlocking;
        private System.Windows.Forms.ToolStripMenuItem TSMIMovementMonitroing;
        private System.Windows.Forms.ToolStripMenuItem TSMIInventory;
        private System.Windows.Forms.ToolStripMenuItem TSMIPalletStatus;
        private System.Windows.Forms.ToolStripMenuItem TSMIHiRackStatus;
        private System.Windows.Forms.ToolStripMenuItem TSMIHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMIMovementHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMIPalletHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMIHiRackHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMIErrorHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMISystem;
        private System.Windows.Forms.ToolStripMenuItem TSMISAPIFHistory;
        private System.Windows.Forms.ToolStripMenuItem TSMISAPMovementIF;
        private System.Windows.Forms.ToolStripMenuItem TSMIUserManagement;
        private System.Windows.Forms.ToolStripMenuItem TSMIUserInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIProgramInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIRoleInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIRoleForUser;
        private System.Windows.Forms.ToolStripMenuItem TSMIProgramForRole;
        private System.Windows.Forms.ToolStripMenuItem TSMIMovementRule;
        private System.Windows.Forms.ToolStripMenuItem TSMISystemInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIDeviceInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIMaterialInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIWindowList;
        private System.Windows.Forms.ToolStripMenuItem TSMITransferTimeMonitorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem TSMIBatchInfo;
        private System.Windows.Forms.ToolStripMenuItem TSMIPalletBlockingHistory;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButtonLocalMovement;
        private System.Windows.Forms.ToolStripButton toolStripButtonECSMovement;
        private System.Windows.Forms.ToolStripButton toolStripButtonPalletBlocking;
        private System.Windows.Forms.ToolStripButton toolStripButtonMovementMonitroing;
        private System.Windows.Forms.ToolStripButton toolStripButtonTransferTimeMonitorToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButtonSAPMovementIF;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButtonPalletStatus;
        private System.Windows.Forms.ToolStripButton toolStripButtonHiRackStatus;
        private System.Windows.Forms.ToolStripButton toolStripButtonMovementHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonPalletHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonPalletBlockingHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonHiRackHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonErrorHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonSAPIFHistory;
        private System.Windows.Forms.ToolStripButton toolStripButtonMovementRule;
        private System.Windows.Forms.ToolStripButton toolStripButtonSystemInfo;
        private System.Windows.Forms.ToolStripButton toolStripButtonDeviceInfo;
        private System.Windows.Forms.ToolStripButton toolStripButtonSapMovement;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton toolStripButtonMaterialInfo;
        private System.Windows.Forms.ToolStripButton toolStripButtonBatchInfo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton toolStripButtonAbout;
    }
}

