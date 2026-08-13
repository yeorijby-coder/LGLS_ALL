namespace ECP.Gui.User
{
    partial class UserForm
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
            this.components = new System.ComponentModel.Container();
            this.listUserView = new System.Windows.Forms.ListView();
            this.columnUserId = new System.Windows.Forms.ColumnHeader();
            this.columnUserName = new System.Windows.Forms.ColumnHeader();
            this.columnUserDesc = new System.Windows.Forms.ColumnHeader();
            this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.closeStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // listUserView
            // 
            this.listUserView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.listUserView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnUserId,
            this.columnUserName,
            this.columnUserDesc});
            this.listUserView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listUserView.FullRowSelect = true;
            this.listUserView.GridLines = true;
            this.listUserView.Location = new System.Drawing.Point(0, 0);
            this.listUserView.MultiSelect = false;
            this.listUserView.Name = "listUserView";
            this.listUserView.Size = new System.Drawing.Size(492, 450);
            this.listUserView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.listUserView.TabIndex = 0;
            this.listUserView.UseCompatibleStateImageBehavior = false;
            this.listUserView.View = System.Windows.Forms.View.Details;
            this.listUserView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.listUserView_MouseUp);
            // 
            // columnUserId
            // 
            this.columnUserId.Text = "UserID";
            this.columnUserId.Width = 153;
            // 
            // columnUserName
            // 
            this.columnUserName.Text = "UserName";
            this.columnUserName.Width = 166;
            // 
            // columnUserDesc
            // 
            this.columnUserDesc.Text = "Description";
            this.columnUserDesc.Width = 169;
            // 
            // contextMenuStrip
            // 
            this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addStripMenuItem,
            this.deleteStripMenuItem,
            this.toolStripSeparator1,
            this.closeStripMenuItem});
            this.contextMenuStrip.Name = "contextMenuStrip";
            this.contextMenuStrip.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.contextMenuStrip.Size = new System.Drawing.Size(112, 76);
            // 
            // addStripMenuItem
            // 
            this.addStripMenuItem.Name = "addStripMenuItem";
            this.addStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.addStripMenuItem.Text = "Add";
            this.addStripMenuItem.Click += new System.EventHandler(this.addStripMenuItem_Click);
            // 
            // deleteStripMenuItem
            // 
            this.deleteStripMenuItem.Name = "deleteStripMenuItem";
            this.deleteStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.deleteStripMenuItem.Text = "Delete";
            this.deleteStripMenuItem.Click += new System.EventHandler(this.deleteStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(108, 6);
            // 
            // closeStripMenuItem
            // 
            this.closeStripMenuItem.Name = "closeStripMenuItem";
            this.closeStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.closeStripMenuItem.Text = "Close";
            this.closeStripMenuItem.Click += new System.EventHandler(this.closeStripMenuItem_Click);
            // 
            // UserForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(492, 450);
            this.Controls.Add(this.listUserView);
            this.Name = "UserForm";
            this.Text = "User";
            this.contextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listUserView;
        private System.Windows.Forms.ColumnHeader columnUserId;
        private System.Windows.Forms.ColumnHeader columnUserName;
        private System.Windows.Forms.ColumnHeader columnUserDesc;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem addStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem closeStripMenuItem;
    }
}