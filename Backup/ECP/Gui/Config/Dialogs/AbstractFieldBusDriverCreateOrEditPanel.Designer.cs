namespace ECP.Gui.Config.Dialogs
{
    partial class AbstractFieldBusDriverCreateOrEditPanel
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
            this.components = new System.ComponentModel.Container();
            this.listViewAbstractFieldBusDriver = new System.Windows.Forms.ListView();
            this.columnHeaderDriverNo = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDriverType = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderOwnerID = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderOwnerType = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderOpenRetrySec = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderTimeOutSec = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderConnectString = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderScanInterval = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderUseFlag = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDescription = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderAssemblyName = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderClassName = new System.Windows.Forms.ColumnHeader();
            this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.selectDriverStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.refreshlStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1 = new System.Windows.Forms.Panel();
            this.contextMenuStrip.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewAbstractFieldBusDriver
            // 
            this.listViewAbstractFieldBusDriver.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.listViewAbstractFieldBusDriver.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderDriverNo,
            this.columnHeaderDriverType,
            this.columnHeaderOwnerID,
            this.columnHeaderOwnerType,
            this.columnHeaderOpenRetrySec,
            this.columnHeaderTimeOutSec,
            this.columnHeaderConnectString,
            this.columnHeaderScanInterval,
            this.columnHeaderUseFlag,
            this.columnHeaderDescription,
            this.columnHeaderAssemblyName,
            this.columnHeaderClassName});
            this.listViewAbstractFieldBusDriver.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewAbstractFieldBusDriver.FullRowSelect = true;
            this.listViewAbstractFieldBusDriver.GridLines = true;
            this.listViewAbstractFieldBusDriver.Location = new System.Drawing.Point(0, 0);
            this.listViewAbstractFieldBusDriver.Name = "listViewAbstractFieldBusDriver";
            this.listViewAbstractFieldBusDriver.Size = new System.Drawing.Size(700, 200);
            this.listViewAbstractFieldBusDriver.TabIndex = 0;
            this.listViewAbstractFieldBusDriver.UseCompatibleStateImageBehavior = false;
            this.listViewAbstractFieldBusDriver.View = System.Windows.Forms.View.Details;
            this.listViewAbstractFieldBusDriver.MouseUp += new System.Windows.Forms.MouseEventHandler(this.listViewAbstractFieldBusDriver_MouseUp);
            // 
            // columnHeaderDriverNo
            // 
            this.columnHeaderDriverNo.Text = "DriverNo";
            this.columnHeaderDriverNo.Width = 64;
            // 
            // columnHeaderDriverType
            // 
            this.columnHeaderDriverType.Text = "DriverType";
            this.columnHeaderDriverType.Width = 77;
            // 
            // columnHeaderOwnerID
            // 
            this.columnHeaderOwnerID.Text = "OwnerID";
            // 
            // columnHeaderOwnerType
            // 
            this.columnHeaderOwnerType.Text = "OwnerType";
            // 
            // columnHeaderOpenRetrySec
            // 
            this.columnHeaderOpenRetrySec.Text = "OpenRetrySec";
            // 
            // columnHeaderTimeOutSec
            // 
            this.columnHeaderTimeOutSec.Text = "TimeOutSec";
            // 
            // columnHeaderConnectString
            // 
            this.columnHeaderConnectString.Text = "ConnectString";
            this.columnHeaderConnectString.Width = 133;
            // 
            // columnHeaderScanInterval
            // 
            this.columnHeaderScanInterval.Text = "ScanInterval";
            // 
            // columnHeaderUseFlag
            // 
            this.columnHeaderUseFlag.Text = "UseFlag";
            this.columnHeaderUseFlag.Width = 42;
            // 
            // columnHeaderDescription
            // 
            this.columnHeaderDescription.Text = "Description";
            // 
            // columnHeaderAssemblyName
            // 
            this.columnHeaderAssemblyName.Text = "AssemblyName";
            // 
            // columnHeaderClassName
            // 
            this.columnHeaderClassName.Text = "ClassName";
            // 
            // contextMenuStrip
            // 
            this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.editToolStripMenuItem,
            this.toolStripSeparator1,
            this.selectDriverStripMenuItem,
            this.refreshlStripMenuItem});
            this.contextMenuStrip.Name = "contextMenuStrip";
            this.contextMenuStrip.Size = new System.Drawing.Size(148, 120);
            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.addToolStripMenuItem.Text = "Add";
            this.addToolStripMenuItem.Click += new System.EventHandler(this.addToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.editToolStripMenuItem.Text = "Edit";
            this.editToolStripMenuItem.Click += new System.EventHandler(this.editToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(144, 6);
            // 
            // selectDriverStripMenuItem
            // 
            this.selectDriverStripMenuItem.Name = "selectDriverStripMenuItem";
            this.selectDriverStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.selectDriverStripMenuItem.Text = "Select Driver";
            this.selectDriverStripMenuItem.Click += new System.EventHandler(this.selectDriverStripMenuItem_Click);
            // 
            // refreshlStripMenuItem
            // 
            this.refreshlStripMenuItem.Name = "refreshlStripMenuItem";
            this.refreshlStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.refreshlStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.refreshlStripMenuItem.Text = "Refresh";
            this.refreshlStripMenuItem.Click += new System.EventHandler(this.refreshlStripMenuItem_Click);
            // 
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.Controls.Add(this.listViewAbstractFieldBusDriver);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(700, 200);
            this.panel1.TabIndex = 2;
            // 
            // AbstractFieldBusDriverCreateOrEditPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.Controls.Add(this.panel1);
            this.Name = "AbstractFieldBusDriverCreateOrEditPanel";
            this.Size = new System.Drawing.Size(700, 200);
            this.contextMenuStrip.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewAbstractFieldBusDriver;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ColumnHeader columnHeaderDriverNo;
        private System.Windows.Forms.ColumnHeader columnHeaderDriverType;
        private System.Windows.Forms.ColumnHeader columnHeaderOwnerID;
        private System.Windows.Forms.ColumnHeader columnHeaderOwnerType;
        private System.Windows.Forms.ColumnHeader columnHeaderOpenRetrySec;
        private System.Windows.Forms.ColumnHeader columnHeaderTimeOutSec;
        private System.Windows.Forms.ColumnHeader columnHeaderConnectString;
        private System.Windows.Forms.ColumnHeader columnHeaderScanInterval;
        private System.Windows.Forms.ColumnHeader columnHeaderUseFlag;
        private System.Windows.Forms.ColumnHeader columnHeaderDescription;
        private System.Windows.Forms.ColumnHeader columnHeaderAssemblyName;
        private System.Windows.Forms.ColumnHeader columnHeaderClassName;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem selectDriverStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem refreshlStripMenuItem;
    }
}
