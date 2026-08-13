namespace HECS.Gui.Monitor.Panels
{
    partial class MovementRuleInfoMainPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MovementRuleInfoMainPanel));
            this.panel1 = new System.Windows.Forms.Panel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.panel3 = new System.Windows.Forms.Panel();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonSave1 = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonDown = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonUp = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonAdd = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonModify = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonDelete = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonSave = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonExcel = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonRetrieve = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.panel1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightSteelBlue;
            this.panel1.Controls.Add(this.buttonExcel);
            this.panel1.Controls.Add(this.buttonRetrieve);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(998, 40);
            this.panel1.TabIndex = 1;
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
            this.splitContainer1.Panel1.Controls.Add(this.panel3);
            this.splitContainer1.Panel1.Controls.Add(this.dataGridView);
            this.splitContainer1.Panel1.Controls.Add(this.panel2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.AutoScroll = true;
            this.splitContainer1.Panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Size = new System.Drawing.Size(998, 547);
            this.splitContainer1.SplitterDistance = 326;
            this.splitContainer1.SplitterWidth = 3;
            this.splitContainer1.TabIndex = 9;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.buttonSave1);
            this.panel3.Controls.Add(this.buttonDown);
            this.panel3.Controls.Add(this.buttonUp);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel3.Location = new System.Drawing.Point(916, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(78, 278);
            this.panel3.TabIndex = 16;
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
            this.dataGridView.Dock = System.Windows.Forms.DockStyle.Left;
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
            this.dataGridView.Size = new System.Drawing.Size(913, 278);
            this.dataGridView.TabIndex = 9;
            this.dataGridView.TabStop = false;
            this.dataGridView.CellMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView_CellMouseClick);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.buttonAdd);
            this.panel2.Controls.Add(this.buttonModify);
            this.panel2.Controls.Add(this.buttonDelete);
            this.panel2.Controls.Add(this.buttonSave);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel2.Location = new System.Drawing.Point(0, 278);
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
            // buttonSave1
            // 
            this.buttonSave1.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonSave1.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonSave1.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonSave1.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonSave1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSave1.Image = global::HECS.Properties.Resources.bullet53312300;
            this.buttonSave1.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonSave1.Location = new System.Drawing.Point(0, 88);
            this.buttonSave1.Name = "buttonSave1";
            this.buttonSave1.Size = new System.Drawing.Size(78, 44);
            this.buttonSave1.TabIndex = 20;
            this.buttonSave1.Text = "저장";
            this.buttonSave1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonSave1.UseVisualStyleBackColor = true;
            this.buttonSave1.Visible = false;
            this.buttonSave1.Click += new System.EventHandler(this.buttonSave1_Click);
            // 
            // buttonDown
            // 
            this.buttonDown.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonDown.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonDown.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonDown.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonDown.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonDown.Image = global::HECS.Properties.Resources.DOWN3;
            this.buttonDown.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonDown.Location = new System.Drawing.Point(0, 44);
            this.buttonDown.Name = "buttonDown";
            this.buttonDown.Size = new System.Drawing.Size(78, 44);
            this.buttonDown.TabIndex = 19;
            this.buttonDown.Text = "아래";
            this.buttonDown.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonDown.UseVisualStyleBackColor = true;
            this.buttonDown.Visible = false;
            this.buttonDown.Click += new System.EventHandler(this.buttonDown_Click);
            // 
            // buttonUp
            // 
            this.buttonUp.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonUp.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonUp.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonUp.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonUp.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonUp.Image = global::HECS.Properties.Resources.UP3;
            this.buttonUp.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonUp.Location = new System.Drawing.Point(0, 0);
            this.buttonUp.Name = "buttonUp";
            this.buttonUp.Size = new System.Drawing.Size(78, 44);
            this.buttonUp.TabIndex = 18;
            this.buttonUp.Text = "위";
            this.buttonUp.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonUp.UseVisualStyleBackColor = true;
            this.buttonUp.Visible = false;
            this.buttonUp.Click += new System.EventHandler(this.buttonUp_Click);
            // 
            // buttonAdd
            // 
            this.buttonAdd.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonAdd.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonAdd.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonAdd.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonAdd.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAdd.Image = global::HECS.Properties.Resources.bullet53308100;
            this.buttonAdd.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonAdd.Location = new System.Drawing.Point(670, 0);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(81, 44);
            this.buttonAdd.TabIndex = 18;
            this.buttonAdd.Text = "등록";
            this.buttonAdd.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Visible = false;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // buttonModify
            // 
            this.buttonModify.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonModify.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonModify.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonModify.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonModify.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonModify.Image = global::HECS.Properties.Resources.bullet53309300;
            this.buttonModify.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonModify.Location = new System.Drawing.Point(751, 0);
            this.buttonModify.Name = "buttonModify";
            this.buttonModify.Size = new System.Drawing.Size(81, 44);
            this.buttonModify.TabIndex = 16;
            this.buttonModify.Text = "수정";
            this.buttonModify.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonModify.UseVisualStyleBackColor = true;
            this.buttonModify.Click += new System.EventHandler(this.buttonModify_Click);
            // 
            // buttonDelete
            // 
            this.buttonDelete.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonDelete.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonDelete.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonDelete.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonDelete.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonDelete.Image = global::HECS.Properties.Resources.bullet53308600;
            this.buttonDelete.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonDelete.Location = new System.Drawing.Point(832, 0);
            this.buttonDelete.Name = "buttonDelete";
            this.buttonDelete.Size = new System.Drawing.Size(81, 44);
            this.buttonDelete.TabIndex = 15;
            this.buttonDelete.Text = "삭제";
            this.buttonDelete.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonDelete.UseVisualStyleBackColor = true;
            this.buttonDelete.Click += new System.EventHandler(this.buttonDelete_Click);
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
            // buttonExcel
            // 
            this.buttonExcel.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonExcel.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonExcel.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonExcel.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonExcel.Image = global::HECS.Properties.Resources.bullet50043600;
            this.buttonExcel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonExcel.Location = new System.Drawing.Point(674, 0);
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
            this.buttonRetrieve.Location = new System.Drawing.Point(594, 0);
            this.buttonRetrieve.Name = "buttonRetrieve";
            this.buttonRetrieve.Size = new System.Drawing.Size(81, 40);
            this.buttonRetrieve.TabIndex = 29;
            this.buttonRetrieve.Text = "조회";
            this.buttonRetrieve.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonRetrieve.UseVisualStyleBackColor = true;
            this.buttonRetrieve.Click += new System.EventHandler(this.buttonRetrieve_Click);
            // 
            // MovementRuleInfoMainPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.label2);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.panel1);
            this.Name = "MovementRuleInfoMainPanel";
            this.Size = new System.Drawing.Size(998, 587);
            this.panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel2;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonSave;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonModify;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonDelete;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonExcel;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonRetrieve;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonAdd;
        private System.Windows.Forms.Panel panel3;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonDown;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonUp;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonSave1;
    }
}
