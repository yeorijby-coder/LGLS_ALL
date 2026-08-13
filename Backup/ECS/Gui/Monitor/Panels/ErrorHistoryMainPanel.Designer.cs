namespace HECS.Gui.Monitor.Panels
{
    partial class ErrorHistoryMainPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ErrorHistoryMainPanel));
            this.panel1 = new System.Windows.Forms.Panel();
            this.comboBoxErrortype = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.dateTimePickerTo = new System.Windows.Forms.DateTimePicker();
            this.dateTimePickerFrom = new System.Windows.Forms.DateTimePicker();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.panelProgress = new System.Windows.Forms.Panel();
            this.labelProgressMessage = new System.Windows.Forms.Label();
            this.buttonExcel = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonRetrieve = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.panel1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.panelProgress.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightSteelBlue;
            this.panel1.Controls.Add(this.comboBoxErrortype);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.buttonExcel);
            this.panel1.Controls.Add(this.buttonRetrieve);
            this.panel1.Controls.Add(this.dateTimePickerTo);
            this.panel1.Controls.Add(this.dateTimePickerFrom);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(998, 40);
            this.panel1.TabIndex = 1;
            // 
            // comboBoxErrortype
            // 
            this.comboBoxErrortype.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxErrortype.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBoxErrortype.FormattingEnabled = true;
            this.comboBoxErrortype.Location = new System.Drawing.Point(79, 10);
            this.comboBoxErrortype.Name = "comboBoxErrortype";
            this.comboBoxErrortype.Size = new System.Drawing.Size(151, 23);
            this.comboBoxErrortype.TabIndex = 33;
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.Black;
            this.label6.Location = new System.Drawing.Point(2, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(71, 40);
            this.label6.TabIndex = 31;
            this.label6.Text = "Error 유형";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // dateTimePickerTo
            // 
            this.dateTimePickerTo.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dateTimePickerTo.Location = new System.Drawing.Point(496, 12);
            this.dateTimePickerTo.Name = "dateTimePickerTo";
            this.dateTimePickerTo.Size = new System.Drawing.Size(167, 21);
            this.dateTimePickerTo.TabIndex = 26;
            // 
            // dateTimePickerFrom
            // 
            this.dateTimePickerFrom.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dateTimePickerFrom.Location = new System.Drawing.Point(308, 12);
            this.dateTimePickerFrom.Name = "dateTimePickerFrom";
            this.dateTimePickerFrom.Size = new System.Drawing.Size(167, 21);
            this.dateTimePickerFrom.TabIndex = 25;
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(476, 2);
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
            this.label4.Location = new System.Drawing.Point(236, 2);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(69, 40);
            this.label4.TabIndex = 23;
            this.label4.Text = "조회 기간";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // splitContainer1
            // 
            this.splitContainer1.BackColor = System.Drawing.Color.RoyalBlue;
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 40);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Panel1.Controls.Add(this.panelProgress);
            this.splitContainer1.Panel1.Controls.Add(this.dataGridView);
            this.splitContainer1.Panel1.Controls.Add(this.panel2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.AutoScroll = true;
            this.splitContainer1.Panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Panel2Collapsed = true;
            this.splitContainer1.Size = new System.Drawing.Size(998, 547);
            this.splitContainer1.SplitterDistance = 326;
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
            this.dataGridView.Size = new System.Drawing.Size(994, 499);
            this.dataGridView.TabIndex = 9;
            this.dataGridView.TabStop = false;
            // 
            // panel2
            // 
            this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel2.Location = new System.Drawing.Point(0, 499);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(994, 44);
            this.panel2.TabIndex = 15;
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.RoyalBlue;
            this.label2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label2.Location = new System.Drawing.Point(0, 583);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(998, 4);
            this.label2.TabIndex = 10;
            // 
            // panelProgress
            // 
            this.panelProgress.BackColor = System.Drawing.Color.White;
            this.panelProgress.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelProgress.Controls.Add(this.labelProgressMessage);
            this.panelProgress.Location = new System.Drawing.Point(265, 228);
            this.panelProgress.Name = "panelProgress";
            this.panelProgress.Padding = new System.Windows.Forms.Padding(3);
            this.panelProgress.Size = new System.Drawing.Size(485, 54);
            this.panelProgress.TabIndex = 21;
            this.panelProgress.Visible = false;
            // 
            // labelProgressMessage
            // 
            this.labelProgressMessage.BackColor = System.Drawing.Color.Red;
            this.labelProgressMessage.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelProgressMessage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelProgressMessage.Font = new System.Drawing.Font("돋움체", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelProgressMessage.ForeColor = System.Drawing.Color.White;
            this.labelProgressMessage.Location = new System.Drawing.Point(3, 3);
            this.labelProgressMessage.Name = "labelProgressMessage";
            this.labelProgressMessage.Size = new System.Drawing.Size(477, 46);
            this.labelProgressMessage.TabIndex = 1;
            this.labelProgressMessage.Text = "데이터를 읽고 있습니다!! 잠시 기다리세요!!";
            this.labelProgressMessage.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // buttonExcel
            // 
            this.buttonExcel.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonExcel.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonExcel.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonExcel.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonExcel.Image = global::HECS.Properties.Resources.bullet50043600;
            this.buttonExcel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonExcel.Location = new System.Drawing.Point(834, 1);
            this.buttonExcel.Name = "buttonExcel";
            this.buttonExcel.Size = new System.Drawing.Size(81, 40);
            this.buttonExcel.TabIndex = 28;
            this.buttonExcel.Text = "Excel";
            this.buttonExcel.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonExcel.UseVisualStyleBackColor = true;
            this.buttonExcel.Click += new System.EventHandler(this.buttonExcel_Click);
            // 
            // buttonRetrieve
            // 
            this.buttonRetrieve.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonRetrieve.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonRetrieve.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonRetrieve.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonRetrieve.Image = ((System.Drawing.Image)(resources.GetObject("buttonRetrieve.Image")));
            this.buttonRetrieve.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonRetrieve.Location = new System.Drawing.Point(754, 1);
            this.buttonRetrieve.Name = "buttonRetrieve";
            this.buttonRetrieve.Size = new System.Drawing.Size(81, 40);
            this.buttonRetrieve.TabIndex = 29;
            this.buttonRetrieve.Text = "조회";
            this.buttonRetrieve.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonRetrieve.UseVisualStyleBackColor = true;
            this.buttonRetrieve.Click += new System.EventHandler(this.buttonRetrieve_Click);
            // 
            // ErrorHistoryMainPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.label2);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.panel1);
            this.Name = "ErrorHistoryMainPanel";
            this.Size = new System.Drawing.Size(998, 587);
            this.panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.panelProgress.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel2;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonExcel;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonRetrieve;
        private System.Windows.Forms.DateTimePicker dateTimePickerTo;
        private System.Windows.Forms.DateTimePicker dateTimePickerFrom;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox comboBoxErrortype;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Panel panelProgress;
        private System.Windows.Forms.Label labelProgressMessage;
    }
}
