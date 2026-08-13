namespace HECS.Gui.Monitor.Panels
{
    partial class MoveRequestMainPanel
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonXPRetrieve = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonXPExcel = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.dateTimePickerTo = new System.Windows.Forms.DateTimePicker();
            this.dateTimePickerFrom = new System.Windows.Forms.DateTimePicker();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.panel2 = new System.Windows.Forms.Panel();
            this.buttonXPAdd = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonXPModify = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonSave = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.panel1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightSteelBlue;
            this.panel1.Controls.Add(this.buttonXPRetrieve);
            this.panel1.Controls.Add(this.buttonXPExcel);
            this.panel1.Controls.Add(this.dateTimePickerTo);
            this.panel1.Controls.Add(this.dateTimePickerFrom);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(10);
            this.panel1.Size = new System.Drawing.Size(998, 68);
            this.panel1.TabIndex = 1;
            // 
            // buttonXPRetrieve
            // 
            this.buttonXPRetrieve.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXPRetrieve.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXPRetrieve.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXPRetrieve.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonXPRetrieve.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXPRetrieve.Image = global::HECS.Properties.Resources.bullet53348000;
            this.buttonXPRetrieve.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXPRetrieve.Location = new System.Drawing.Point(826, 10);
            this.buttonXPRetrieve.Name = "buttonXPRetrieve";
            this.buttonXPRetrieve.Size = new System.Drawing.Size(81, 48);
            this.buttonXPRetrieve.TabIndex = 29;
            this.buttonXPRetrieve.Text = "조회";
            this.buttonXPRetrieve.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXPRetrieve.UseVisualStyleBackColor = true;
            this.buttonXPRetrieve.Click += new System.EventHandler(this.buttonXPRetrieve_Click);
            // 
            // buttonXPExcel
            // 
            this.buttonXPExcel.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXPExcel.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXPExcel.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXPExcel.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonXPExcel.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXPExcel.Image = global::HECS.Properties.Resources.bullet50043600;
            this.buttonXPExcel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXPExcel.Location = new System.Drawing.Point(907, 10);
            this.buttonXPExcel.Name = "buttonXPExcel";
            this.buttonXPExcel.Size = new System.Drawing.Size(81, 48);
            this.buttonXPExcel.TabIndex = 28;
            this.buttonXPExcel.Text = "Excel";
            this.buttonXPExcel.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXPExcel.UseVisualStyleBackColor = true;
            this.buttonXPExcel.Visible = false;
            this.buttonXPExcel.Click += new System.EventHandler(this.buttonXPExcel_Click);
            // 
            // dateTimePickerTo
            // 
            this.dateTimePickerTo.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dateTimePickerTo.Location = new System.Drawing.Point(315, 24);
            this.dateTimePickerTo.Name = "dateTimePickerTo";
            this.dateTimePickerTo.Size = new System.Drawing.Size(167, 21);
            this.dateTimePickerTo.TabIndex = 26;
            // 
            // dateTimePickerFrom
            // 
            this.dateTimePickerFrom.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dateTimePickerFrom.Location = new System.Drawing.Point(127, 24);
            this.dateTimePickerFrom.Name = "dateTimePickerFrom";
            this.dateTimePickerFrom.Size = new System.Drawing.Size(167, 21);
            this.dateTimePickerFrom.TabIndex = 25;
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(295, 14);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(19, 40);
            this.label5.TabIndex = 22;
            this.label5.Text = "∼";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.Black;
            this.label4.Location = new System.Drawing.Point(12, 16);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(126, 36);
            this.label4.TabIndex = 23;
            this.label4.Text = "반송 요청 일자";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // splitContainer1
            // 
            this.splitContainer1.BackColor = System.Drawing.Color.RoyalBlue;
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 68);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Panel1.Controls.Add(this.dataGridView);
            this.splitContainer1.Panel1.Controls.Add(this.panel2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.AutoScroll = true;
            this.splitContainer1.Panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Size = new System.Drawing.Size(998, 519);
            this.splitContainer1.SplitterDistance = 327;
            this.splitContainer1.SplitterWidth = 3;
            this.splitContainer1.TabIndex = 9;
            // 
            // dataGridView
            // 
            this.dataGridView.AllowUserToAddRows = false;
            this.dataGridView.AllowUserToDeleteRows = false;
            this.dataGridView.AllowUserToOrderColumns = true;
            this.dataGridView.AllowUserToResizeRows = false;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle1.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.Color.White;
            this.dataGridView.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridView.BackgroundColor = System.Drawing.Color.LemonChiffon;
            this.dataGridView.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dataGridView.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.SingleHorizontal;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle2.BackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.DefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridView.GridColor = System.Drawing.Color.DarkKhaki;
            this.dataGridView.Location = new System.Drawing.Point(0, 0);
            this.dataGridView.Name = "dataGridView";
            this.dataGridView.ReadOnly = true;
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle4.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle4.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle4.ForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle4.NullValue = "☞";
            dataGridViewCellStyle4.SelectionBackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle4.SelectionForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
            this.dataGridView.RowHeadersVisible = false;
            this.dataGridView.RowHeadersWidth = 24;
            this.dataGridView.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            dataGridViewCellStyle5.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle5.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle5.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle5.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle5.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.RowsDefaultCellStyle = dataGridViewCellStyle5;
            this.dataGridView.RowTemplate.DefaultCellStyle.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dataGridView.RowTemplate.Height = 23;
            this.dataGridView.RowTemplate.ReadOnly = true;
            this.dataGridView.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView.ShowCellErrors = false;
            this.dataGridView.ShowCellToolTips = false;
            this.dataGridView.ShowEditingIcon = false;
            this.dataGridView.ShowRowErrors = false;
            this.dataGridView.Size = new System.Drawing.Size(994, 279);
            this.dataGridView.TabIndex = 9;
            this.dataGridView.TabStop = false;
            this.dataGridView.CellMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView_CellMouseClick);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.buttonXPAdd);
            this.panel2.Controls.Add(this.buttonXPModify);
            this.panel2.Controls.Add(this.buttonSave);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel2.Location = new System.Drawing.Point(0, 279);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(994, 44);
            this.panel2.TabIndex = 15;
            // 
            // buttonXPAdd
            // 
            this.buttonXPAdd.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXPAdd.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXPAdd.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXPAdd.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonXPAdd.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXPAdd.Image = global::HECS.Properties.Resources.bullet53308100;
            this.buttonXPAdd.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXPAdd.Location = new System.Drawing.Point(751, 0);
            this.buttonXPAdd.Name = "buttonXPAdd";
            this.buttonXPAdd.Size = new System.Drawing.Size(81, 44);
            this.buttonXPAdd.TabIndex = 19;
            this.buttonXPAdd.Text = "등록";
            this.buttonXPAdd.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXPAdd.UseVisualStyleBackColor = true;
            this.buttonXPAdd.Click += new System.EventHandler(this.buttonXPAdd_Click);
            // 
            // buttonXPModify
            // 
            this.buttonXPModify.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXPModify.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXPModify.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXPModify.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonXPModify.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXPModify.Image = global::HECS.Properties.Resources.bullet53309300;
            this.buttonXPModify.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXPModify.Location = new System.Drawing.Point(832, 0);
            this.buttonXPModify.Name = "buttonXPModify";
            this.buttonXPModify.Size = new System.Drawing.Size(81, 44);
            this.buttonXPModify.TabIndex = 18;
            this.buttonXPModify.Text = "반송    취소";
            this.buttonXPModify.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXPModify.UseVisualStyleBackColor = true;
            this.buttonXPModify.Click += new System.EventHandler(this.buttonXPModify_Click);
            // 
            // buttonSave
            // 
            this.buttonSave.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonSave.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonSave.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonSave.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonSave.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSave.Image = global::HECS.Properties.Resources.bullet53312300;
            this.buttonSave.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonSave.Location = new System.Drawing.Point(913, 0);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(81, 44);
            this.buttonSave.TabIndex = 17;
            this.buttonSave.Text = "저장";
            this.buttonSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // MoveRequestMainPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.panel1);
            this.Name = "MoveRequestMainPanel";
            this.Size = new System.Drawing.Size(998, 587);
            this.panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonModify;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.Panel panel2;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonSave;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXPExcel;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXPRetrieve;
        private System.Windows.Forms.DateTimePicker dateTimePickerTo;
        private System.Windows.Forms.DateTimePicker dateTimePickerFrom;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXPModify;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXPAdd;
    }
}
