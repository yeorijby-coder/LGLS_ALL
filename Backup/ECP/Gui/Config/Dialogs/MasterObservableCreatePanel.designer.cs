namespace ECP.Gui.Config.Dialogs
{
    partial class MasterObservableCreatePanel
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
            this.panel3 = new System.Windows.Forms.Panel();
            this.listViewMasterObservable = new System.Windows.Forms.ListView();
            this.columnHeaderOwnerID = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderOwnerType = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderObservableID = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderObservableGUID = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderObservablePath = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderAccessType = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDataType = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderMin = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderMax = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDriverNo = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderMonFlag = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderConnectString = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDescription = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderLogEnable = new System.Windows.Forms.ColumnHeader();
            this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.columnHeaderValueInformation = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderRemark = new System.Windows.Forms.ColumnHeader();
            this.panel3.SuspendLayout();
            this.contextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.listViewMasterObservable);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(0, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(968, 353);
            this.panel3.TabIndex = 3;
            // 
            // listViewMasterObservable
            // 
            this.listViewMasterObservable.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.listViewMasterObservable.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderOwnerID,
            this.columnHeaderOwnerType,
            this.columnHeaderObservableID,
            this.columnHeaderObservableGUID,
            this.columnHeaderObservablePath,
            this.columnHeaderAccessType,
            this.columnHeaderDataType,
            this.columnHeaderMin,
            this.columnHeaderMax,
            this.columnHeaderDriverNo,
            this.columnHeaderMonFlag,
            this.columnHeaderConnectString,
            this.columnHeaderDescription,
            this.columnHeaderLogEnable,
            this.columnHeaderValueInformation,
            this.columnHeaderRemark});
            this.listViewMasterObservable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewMasterObservable.FullRowSelect = true;
            this.listViewMasterObservable.GridLines = true;
            this.listViewMasterObservable.Location = new System.Drawing.Point(0, 0);
            this.listViewMasterObservable.Name = "listViewMasterObservable";
            this.listViewMasterObservable.Size = new System.Drawing.Size(968, 353);
            this.listViewMasterObservable.TabIndex = 0;
            this.listViewMasterObservable.UseCompatibleStateImageBehavior = false;
            this.listViewMasterObservable.View = System.Windows.Forms.View.Details;
            this.listViewMasterObservable.DoubleClick += new System.EventHandler(this.listViewMasterObservable_DoubleClick);
            this.listViewMasterObservable.MouseUp += new System.Windows.Forms.MouseEventHandler(this.listViewMasterObservable_MouseUp);
            // 
            // columnHeaderOwnerID
            // 
            this.columnHeaderOwnerID.Text = "OwnerID";
            // 
            // columnHeaderOwnerType
            // 
            this.columnHeaderOwnerType.Text = "OwnerType";
            // 
            // columnHeaderObservableID
            // 
            this.columnHeaderObservableID.Text = "ObservableID";
            // 
            // columnHeaderObservableGUID
            // 
            this.columnHeaderObservableGUID.Text = "ObservableGUID";
            // 
            // columnHeaderObservablePath
            // 
            this.columnHeaderObservablePath.Text = "ObservablePath";
            // 
            // columnHeaderAccessType
            // 
            this.columnHeaderAccessType.Text = "AccessType";
            // 
            // columnHeaderDataType
            // 
            this.columnHeaderDataType.Text = "DataType";
            // 
            // columnHeaderMin
            // 
            this.columnHeaderMin.Text = "Min";
            // 
            // columnHeaderMax
            // 
            this.columnHeaderMax.Text = "Max";
            // 
            // columnHeaderDriverNo
            // 
            this.columnHeaderDriverNo.Text = "DriverNo";
            // 
            // columnHeaderMonFlag
            // 
            this.columnHeaderMonFlag.Text = "MonFlag";
            // 
            // columnHeaderConnectString
            // 
            this.columnHeaderConnectString.Text = "ConnectString";
            // 
            // columnHeaderDescription
            // 
            this.columnHeaderDescription.Text = "Description";
            // 
            // columnHeaderLogEnable
            // 
            this.columnHeaderLogEnable.Text = "LogEnable";
            // 
            // contextMenuStrip
            // 
            this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.editToolStripMenuItem,
            this.filterToolStripMenuItem,
            this.toolStripSeparator1,
            this.refreshToolStripMenuItem});
            this.contextMenuStrip.Name = "contextMenuStrip1";
            this.contextMenuStrip.Size = new System.Drawing.Size(138, 120);
            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(137, 22);
            this.addToolStripMenuItem.Text = "Add";
            this.addToolStripMenuItem.Click += new System.EventHandler(this.addToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(137, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(137, 22);
            this.editToolStripMenuItem.Text = "Edit";
            this.editToolStripMenuItem.Click += new System.EventHandler(this.editToolStripMenuItem_Click);
            // 
            // filterToolStripMenuItem
            // 
            this.filterToolStripMenuItem.Name = "filterToolStripMenuItem";
            this.filterToolStripMenuItem.Size = new System.Drawing.Size(137, 22);
            this.filterToolStripMenuItem.Text = "Filter";
            this.filterToolStripMenuItem.Click += new System.EventHandler(this.filterToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(134, 6);
            // 
            // refreshToolStripMenuItem
            // 
            this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
            this.refreshToolStripMenuItem.ShortcutKeyDisplayString = "F5";
            this.refreshToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.refreshToolStripMenuItem.Size = new System.Drawing.Size(137, 22);
            this.refreshToolStripMenuItem.Text = "Refresh";
            this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
            // 
            // columnHeaderValueInformation
            // 
            this.columnHeaderValueInformation.Text = "ValueInformation";
            // 
            // columnHeaderRemark
            // 
            this.columnHeaderRemark.Text = "Remark";
            // 
            // MasterObservableCreatePanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.Controls.Add(this.panel3);
            this.Name = "MasterObservableCreatePanel";
            this.Size = new System.Drawing.Size(968, 353);
            this.panel3.ResumeLayout(false);
            this.contextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.ColumnHeader columnHeaderOwnerID;
        private System.Windows.Forms.ColumnHeader columnHeaderOwnerType;
        private System.Windows.Forms.ColumnHeader columnHeaderObservableID;
        private System.Windows.Forms.ColumnHeader columnHeaderObservableGUID;
        private System.Windows.Forms.ColumnHeader columnHeaderObservablePath;
        private System.Windows.Forms.ColumnHeader columnHeaderAccessType;
        private System.Windows.Forms.ColumnHeader columnHeaderDataType;
        private System.Windows.Forms.ColumnHeader columnHeaderMin;
        private System.Windows.Forms.ColumnHeader columnHeaderMax;
        private System.Windows.Forms.ColumnHeader columnHeaderDriverNo;
        private System.Windows.Forms.ColumnHeader columnHeaderMonFlag;
        private System.Windows.Forms.ColumnHeader columnHeaderConnectString;
        private System.Windows.Forms.ColumnHeader columnHeaderDescription;
        private System.Windows.Forms.ColumnHeader columnHeaderLogEnable;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem filterToolStripMenuItem;
        private System.Windows.Forms.ListView listViewMasterObservable;
        private System.Windows.Forms.ColumnHeader columnHeaderValueInformation;
        private System.Windows.Forms.ColumnHeader columnHeaderRemark;

    }
}
