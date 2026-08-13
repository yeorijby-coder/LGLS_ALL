namespace HECS.Gui.Monitor.Popups
{
    partial class TransferDetailForm
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle11 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle12 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle13 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle14 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle15 = new System.Windows.Forms.DataGridViewCellStyle();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonXPExcel = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonClose = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonRetrieve = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.panel3 = new System.Windows.Forms.Panel();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.panelBack.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // panelBack
            // 
            this.panelBack.Controls.Add(this.panel3);
            this.panelBack.Controls.Add(this.panel1);
            this.panelBack.Size = new System.Drawing.Size(1265, 448);
            this.panelBack.Text = "입출고 상세 이력";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightSteelBlue;
            this.panel1.Controls.Add(this.buttonXPExcel);
            this.panel1.Controls.Add(this.buttonClose);
            this.panel1.Controls.Add(this.buttonRetrieve);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(10, 21);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1246, 60);
            this.panel1.TabIndex = 0;
            // 
            // buttonXPExcel
            // 
            this.buttonXPExcel.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXPExcel.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXPExcel.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXPExcel.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXPExcel.Image = global::HECS.Properties.Resources.bullet50043600;
            this.buttonXPExcel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXPExcel.Location = new System.Drawing.Point(1038, 8);
            this.buttonXPExcel.Name = "buttonXPExcel";
            this.buttonXPExcel.Size = new System.Drawing.Size(101, 46);
            this.buttonXPExcel.TabIndex = 29;
            this.buttonXPExcel.Text = "Excel";
            this.buttonXPExcel.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXPExcel.UseVisualStyleBackColor = true;
            this.buttonXPExcel.Click += new System.EventHandler(this.buttonXPExcel_Click);
            // 
            // buttonClose
            // 
            this.buttonClose.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonClose.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonClose.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonClose.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonClose.Image = global::HECS.Properties.Resources.bullet53308600;
            this.buttonClose.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonClose.Location = new System.Drawing.Point(1142, 8);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(101, 46);
            this.buttonClose.TabIndex = 0;
            this.buttonClose.Text = "닫기";
            this.buttonClose.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // buttonRetrieve
            // 
            this.buttonRetrieve.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonRetrieve.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonRetrieve.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonRetrieve.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonRetrieve.Image = global::HECS.Properties.Resources.bullet50055600;
            this.buttonRetrieve.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonRetrieve.Location = new System.Drawing.Point(935, 8);
            this.buttonRetrieve.Name = "buttonRetrieve";
            this.buttonRetrieve.Size = new System.Drawing.Size(101, 46);
            this.buttonRetrieve.TabIndex = 2;
            this.buttonRetrieve.Text = "조회";
            this.buttonRetrieve.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonRetrieve.UseVisualStyleBackColor = true;
            this.buttonRetrieve.Click += new System.EventHandler(this.buttonRetrieve_Click);
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.dataGridView);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(10, 81);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(1246, 360);
            this.panel3.TabIndex = 1;
            // 
            // dataGridView
            // 
            this.dataGridView.AllowUserToAddRows = false;
            this.dataGridView.AllowUserToDeleteRows = false;
            this.dataGridView.AllowUserToOrderColumns = true;
            this.dataGridView.AllowUserToResizeRows = false;
            dataGridViewCellStyle11.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle11.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle11.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle11.SelectionForeColor = System.Drawing.Color.White;
            this.dataGridView.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle11;
            this.dataGridView.BackgroundColor = System.Drawing.Color.LemonChiffon;
            this.dataGridView.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dataGridView.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.SingleHorizontal;
            dataGridViewCellStyle12.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle12.BackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle12.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            dataGridViewCellStyle12.ForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle12.SelectionBackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle12.SelectionForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle12.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle12;
            this.dataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            dataGridViewCellStyle13.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle13.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle13.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            dataGridViewCellStyle13.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle13.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle13.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle13.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridView.DefaultCellStyle = dataGridViewCellStyle13;
            this.dataGridView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridView.GridColor = System.Drawing.Color.Khaki;
            this.dataGridView.Location = new System.Drawing.Point(0, 0);
            this.dataGridView.Name = "dataGridView";
            this.dataGridView.ReadOnly = true;
            dataGridViewCellStyle14.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle14.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle14.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            dataGridViewCellStyle14.ForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle14.NullValue = "☞";
            dataGridViewCellStyle14.SelectionBackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle14.SelectionForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle14.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.RowHeadersDefaultCellStyle = dataGridViewCellStyle14;
            this.dataGridView.RowHeadersVisible = false;
            this.dataGridView.RowHeadersWidth = 24;
            this.dataGridView.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            dataGridViewCellStyle15.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle15.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle15.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle15.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle15.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.RowsDefaultCellStyle = dataGridViewCellStyle15;
            this.dataGridView.RowTemplate.DefaultCellStyle.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dataGridView.RowTemplate.Height = 23;
            this.dataGridView.RowTemplate.ReadOnly = true;
            this.dataGridView.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView.ShowCellErrors = false;
            this.dataGridView.ShowCellToolTips = false;
            this.dataGridView.ShowEditingIcon = false;
            this.dataGridView.ShowRowErrors = false;
            this.dataGridView.Size = new System.Drawing.Size(1246, 360);
            this.dataGridView.TabIndex = 9;
            this.dataGridView.TabStop = false;
            // 
            // TransferDetailForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(1265, 448);
            this.Name = "TransferDetailForm";
            this.Text = "TransferControlForm";
            this.panelBack.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel3;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonRetrieve;
        private System.Windows.Forms.DataGridView dataGridView;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonClose;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXPExcel;

    }
}