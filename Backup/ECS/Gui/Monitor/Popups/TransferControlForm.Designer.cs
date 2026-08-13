namespace HECS.Gui.Monitor.Popups
{
    partial class TransferControlForm
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panelContent = new System.Windows.Forms.Panel();
            this.dataGridViewTransferList = new System.Windows.Forms.DataGridView();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonRefresh = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.buttonNo = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.panelBack.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panelContent.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewTransferList)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelBack
            // 
            this.panelBack.Controls.Add(this.panel2);
            this.panelBack.Size = new System.Drawing.Size(793, 560);
            this.panelBack.Text = "반송명령조정";
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2.Controls.Add(this.panelContent);
            this.panel2.Controls.Add(this.panel1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(10, 21);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(2);
            this.panel2.Size = new System.Drawing.Size(774, 532);
            this.panel2.TabIndex = 8;
            // 
            // panelContent
            // 
            this.panelContent.AutoScroll = true;
            this.panelContent.Controls.Add(this.dataGridViewTransferList);
            this.panelContent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelContent.Location = new System.Drawing.Point(2, 2);
            this.panelContent.Name = "panelContent";
            this.panelContent.Padding = new System.Windows.Forms.Padding(2, 0, 0, 0);
            this.panelContent.Size = new System.Drawing.Size(766, 466);
            this.panelContent.TabIndex = 14;
            this.panelContent.VisibleChanged += new System.EventHandler(this.panelContent_VisibleChanged);
            // 
            // dataGridViewTransferList
            // 
            this.dataGridViewTransferList.AllowUserToAddRows = false;
            this.dataGridViewTransferList.AllowUserToDeleteRows = false;
            this.dataGridViewTransferList.AllowUserToOrderColumns = true;
            this.dataGridViewTransferList.AllowUserToResizeRows = false;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle1.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.Color.White;
            this.dataGridViewTransferList.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridViewTransferList.BackgroundColor = System.Drawing.Color.LemonChiffon;
            this.dataGridViewTransferList.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dataGridViewTransferList.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.SingleHorizontal;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle2.BackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferList.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridViewTransferList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferList.DefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridViewTransferList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewTransferList.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridViewTransferList.GridColor = System.Drawing.Color.DarkKhaki;
            this.dataGridViewTransferList.Location = new System.Drawing.Point(2, 0);
            this.dataGridViewTransferList.MultiSelect = false;
            this.dataGridViewTransferList.Name = "dataGridViewTransferList";
            this.dataGridViewTransferList.ReadOnly = true;
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle4.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle4.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            dataGridViewCellStyle4.ForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle4.NullValue = "☞";
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle4.SelectionForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferList.RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
            this.dataGridViewTransferList.RowHeadersVisible = false;
            this.dataGridViewTransferList.RowHeadersWidth = 20;
            this.dataGridViewTransferList.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            dataGridViewCellStyle5.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle5.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle5.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle5.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle5.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferList.RowsDefaultCellStyle = dataGridViewCellStyle5;
            this.dataGridViewTransferList.RowTemplate.DefaultCellStyle.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dataGridViewTransferList.RowTemplate.Height = 20;
            this.dataGridViewTransferList.RowTemplate.ReadOnly = true;
            this.dataGridViewTransferList.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridViewTransferList.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewTransferList.ShowCellErrors = false;
            this.dataGridViewTransferList.ShowCellToolTips = false;
            this.dataGridViewTransferList.ShowEditingIcon = false;
            this.dataGridViewTransferList.ShowRowErrors = false;
            this.dataGridViewTransferList.Size = new System.Drawing.Size(764, 466);
            this.dataGridViewTransferList.TabIndex = 8;
            this.dataGridViewTransferList.TabStop = false;
            this.dataGridViewTransferList.MouseClick += new System.Windows.Forms.MouseEventHandler(this.dataGridViewTransferList_MouseClick);
            this.dataGridViewTransferList.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridViewTransferList_CellEndEdit);
            this.dataGridViewTransferList.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridViewTransferList_CellClick);
            this.dataGridViewTransferList.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridViewTransferList_CellContentClick);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.buttonRefresh);
            this.panel1.Controls.Add(this.buttonNo);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(2, 468);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(766, 58);
            this.panel1.TabIndex = 12;
            // 
            // buttonRefresh
            // 
            this.buttonRefresh.BackColor = System.Drawing.Color.RoyalBlue;
            this.buttonRefresh.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonRefresh.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonRefresh.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonRefresh.ForeColor = System.Drawing.Color.Black;
            this.buttonRefresh.Image = global::HECS.Properties.Resources.bullet53348000;
            this.buttonRefresh.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonRefresh.Location = new System.Drawing.Point(245, 2);
            this.buttonRefresh.Name = "buttonRefresh";
            this.buttonRefresh.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonRefresh.Size = new System.Drawing.Size(101, 54);
            this.buttonRefresh.TabIndex = 10;
            this.buttonRefresh.Text = "새로고침";
            this.buttonRefresh.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonRefresh.Click += new System.EventHandler(this.buttonRefresh_Click);
            // 
            // buttonNo
            // 
            this.buttonNo.BackColor = System.Drawing.Color.RoyalBlue;
            this.buttonNo.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonNo.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonNo.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonNo.ForeColor = System.Drawing.Color.Black;
            this.buttonNo.Image = global::HECS.Properties.Resources.OK4;
            this.buttonNo.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonNo.Location = new System.Drawing.Point(351, 2);
            this.buttonNo.Name = "buttonNo";
            this.buttonNo.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonNo.Size = new System.Drawing.Size(101, 54);
            this.buttonNo.TabIndex = 10;
            this.buttonNo.Text = "닫기";
            this.buttonNo.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonNo.Click += new System.EventHandler(this.buttonNo_Click);
            // 
            // TransferControlForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(793, 560);
            this.Name = "TransferControlForm";
            this.Text = "TransferControlForm";
            this.Shown += new System.EventHandler(this.TransferControlForm_Shown);
            this.panelBack.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panelContent.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewTransferList)).EndInit();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel2;
        private HECS.Gui.Jaeit.Buttons.ButtonGlass buttonNo;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panelContent;
        private HECS.Gui.Jaeit.Buttons.ButtonGlass buttonRefresh;
        private System.Windows.Forms.DataGridView dataGridViewTransferList;
    }
}