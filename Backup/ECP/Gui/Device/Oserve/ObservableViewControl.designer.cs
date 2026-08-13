namespace ECP.Gui.Device.Observe
{
    partial class ObservableViewControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ObservableViewControl));
            this.vsScroll = new System.Windows.Forms.VScrollBar();
            this.imlType = new System.Windows.Forms.ImageList(this.components);
            this.tbInPlaceEdit = new System.Windows.Forms.MaskedTextBox();
            this.cmsMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.menuCollapseAll = new System.Windows.Forms.ToolStripMenuItem();
            this.menuExpandAll = new System.Windows.Forms.ToolStripMenuItem();
            this.cmsMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // vsScroll
            // 
            this.vsScroll.Dock = System.Windows.Forms.DockStyle.Right;
            this.vsScroll.Location = new System.Drawing.Point(-17, 0);
            this.vsScroll.Maximum = 10;
            this.vsScroll.Name = "vsScroll";
            this.vsScroll.Size = new System.Drawing.Size(17, 0);
            this.vsScroll.SmallChange = 0;
            this.vsScroll.TabIndex = 3;
            this.vsScroll.ValueChanged += new System.EventHandler(this.VsScroll_ValueChanged);
            // 
            // imlType
            // 
            this.imlType.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imlType.ImageStream")));
            this.imlType.TransparentColor = System.Drawing.Color.White;
            this.imlType.Images.SetKeyName(0, "ChIn.bmp");
            this.imlType.Images.SetKeyName(1, "ChOut.bmp");
            this.imlType.Images.SetKeyName(2, "ChVirtual.bmp");
            this.imlType.Images.SetKeyName(3, "refresh.bmp");
            // 
            // tbInPlaceEdit
            // 
            this.tbInPlaceEdit.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbInPlaceEdit.Location = new System.Drawing.Point(3, 3);
            this.tbInPlaceEdit.Name = "tbInPlaceEdit";
            this.tbInPlaceEdit.Size = new System.Drawing.Size(100, 14);
            this.tbInPlaceEdit.TabIndex = 2;
            this.tbInPlaceEdit.Visible = false;
            this.tbInPlaceEdit.KeyDown += new System.Windows.Forms.KeyEventHandler(this.TbInPlaceEdit_KeyDown);
            // 
            // cmsMenu
            // 
            this.cmsMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuCollapseAll,
            this.menuExpandAll});
            this.cmsMenu.Name = "contextMenuStrip1";
            this.cmsMenu.Size = new System.Drawing.Size(145, 48);
            // 
            // menuCollapseAll
            // 
            this.menuCollapseAll.Name = "menuCollapseAll";
            this.menuCollapseAll.Size = new System.Drawing.Size(144, 22);
            this.menuCollapseAll.Text = "&Collapse All";
            this.menuCollapseAll.Click += new System.EventHandler(this.menuCollapseAll_Click);
            // 
            // menuExpandAll
            // 
            this.menuExpandAll.Name = "menuExpandAll";
            this.menuExpandAll.Size = new System.Drawing.Size(144, 22);
            this.menuExpandAll.Text = "&Expand All";
            this.menuExpandAll.Click += new System.EventHandler(this.menuExpandAll_Click);
            // 
            // ObservableViewControl
            // 
            this.ContextMenuStrip = this.cmsMenu;
            this.Controls.Add(this.vsScroll);
            this.Controls.Add(this.tbInPlaceEdit);
            this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.ObservableViewControl_MouseWheel);
            this.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.ObservableViewControl_PreviewKeyDown);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ObservableViewControl_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ObservableViewControl_MouseMove);
            this.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.ObservableViewControl_MouseDoubleClick);
            this.Resize += new System.EventHandler(this.ObservableViewControl_Resize);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ObservableViewControl_MouseUp);
            this.cmsMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.VScrollBar vsScroll;
        private System.Windows.Forms.ImageList imlType;
        private System.Windows.Forms.MaskedTextBox tbInPlaceEdit;
        private System.Windows.Forms.ContextMenuStrip cmsMenu;
        private System.Windows.Forms.ToolStripMenuItem menuCollapseAll;
        private System.Windows.Forms.ToolStripMenuItem menuExpandAll;
    }
}
