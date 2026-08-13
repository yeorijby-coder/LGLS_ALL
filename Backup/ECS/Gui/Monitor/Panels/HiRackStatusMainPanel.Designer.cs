namespace HECS.Gui.Monitor.Panels
{
    partial class HiRackStatusMainPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HiRackStatusMainPanel));
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.panelProgress = new System.Windows.Forms.Panel();
            this.labelProgressMessage = new System.Windows.Forms.Label();
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.checkBoxEmpty = new System.Windows.Forms.CheckBox();
            this.checkBoxBlockingFlag = new System.Windows.Forms.CheckBox();
            this.comboBoxLevel = new System.Windows.Forms.ComboBox();
            this.comboBoxBay = new System.Windows.Forms.ComboBox();
            this.comboBoxBank = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.comboBoxBatchno = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.comboBoxMaterialno = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxPalletid = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.comboBoxCompanytype = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.buttonModify = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonSave = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonXPExcel = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonXPRetrieve = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.panelProgress.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.BackColor = System.Drawing.Color.RoyalBlue;
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 99);
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
            this.splitContainer1.Panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Size = new System.Drawing.Size(863, 409);
            this.splitContainer1.SplitterDistance = 242;
            this.splitContainer1.SplitterWidth = 3;
            this.splitContainer1.TabIndex = 13;
            // 
            // panelProgress
            // 
            this.panelProgress.BackColor = System.Drawing.Color.White;
            this.panelProgress.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelProgress.Controls.Add(this.labelProgressMessage);
            this.panelProgress.Location = new System.Drawing.Point(188, 71);
            this.panelProgress.Name = "panelProgress";
            this.panelProgress.Padding = new System.Windows.Forms.Padding(3);
            this.panelProgress.Size = new System.Drawing.Size(485, 54);
            this.panelProgress.TabIndex = 17;
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
            dataGridViewCellStyle2.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
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
            dataGridViewCellStyle4.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
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
            this.dataGridView.Size = new System.Drawing.Size(859, 194);
            this.dataGridView.TabIndex = 9;
            this.dataGridView.TabStop = false;
            this.dataGridView.CellMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView_CellMouseClick);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.buttonModify);
            this.panel2.Controls.Add(this.buttonSave);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel2.Location = new System.Drawing.Point(0, 194);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(859, 44);
            this.panel2.TabIndex = 15;
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.RoyalBlue;
            this.label2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label2.Location = new System.Drawing.Point(0, 504);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(863, 4);
            this.label2.TabIndex = 15;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightSteelBlue;
            this.panel1.Controls.Add(this.label8);
            this.panel1.Controls.Add(this.comboBoxCompanytype);
            this.panel1.Controls.Add(this.textBoxPalletid);
            this.panel1.Controls.Add(this.label7);
            this.panel1.Controls.Add(this.checkBoxEmpty);
            this.panel1.Controls.Add(this.checkBoxBlockingFlag);
            this.panel1.Controls.Add(this.buttonXPExcel);
            this.panel1.Controls.Add(this.buttonXPRetrieve);
            this.panel1.Controls.Add(this.comboBoxLevel);
            this.panel1.Controls.Add(this.comboBoxBay);
            this.panel1.Controls.Add(this.comboBoxBank);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.comboBoxBatchno);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.comboBoxMaterialno);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(863, 99);
            this.panel1.TabIndex = 18;
            // 
            // checkBoxEmpty
            // 
            this.checkBoxEmpty.AutoSize = true;
            this.checkBoxEmpty.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.checkBoxEmpty.Location = new System.Drawing.Point(479, 44);
            this.checkBoxEmpty.Name = "checkBoxEmpty";
            this.checkBoxEmpty.Size = new System.Drawing.Size(121, 19);
            this.checkBoxEmpty.TabIndex = 30;
            this.checkBoxEmpty.Text = "빈 Cell 포함 여부";
            this.checkBoxEmpty.UseVisualStyleBackColor = true;
            // 
            // checkBoxBlockingFlag
            // 
            this.checkBoxBlockingFlag.AutoSize = true;
            this.checkBoxBlockingFlag.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.checkBoxBlockingFlag.Location = new System.Drawing.Point(479, 13);
            this.checkBoxBlockingFlag.Name = "checkBoxBlockingFlag";
            this.checkBoxBlockingFlag.Size = new System.Drawing.Size(139, 19);
            this.checkBoxBlockingFlag.TabIndex = 30;
            this.checkBoxBlockingFlag.Text = "Pallet Blocking 여부";
            this.checkBoxBlockingFlag.UseVisualStyleBackColor = true;
            // 
            // comboBoxLevel
            // 
            this.comboBoxLevel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxLevel.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBoxLevel.FormattingEnabled = true;
            this.comboBoxLevel.Location = new System.Drawing.Point(352, 40);
            this.comboBoxLevel.Name = "comboBoxLevel";
            this.comboBoxLevel.Size = new System.Drawing.Size(60, 23);
            this.comboBoxLevel.TabIndex = 24;
            // 
            // comboBoxBay
            // 
            this.comboBoxBay.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxBay.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBoxBay.FormattingEnabled = true;
            this.comboBoxBay.Location = new System.Drawing.Point(213, 40);
            this.comboBoxBay.Name = "comboBoxBay";
            this.comboBoxBay.Size = new System.Drawing.Size(59, 23);
            this.comboBoxBay.TabIndex = 24;
            // 
            // comboBoxBank
            // 
            this.comboBoxBank.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxBank.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBoxBank.FormattingEnabled = true;
            this.comboBoxBank.Location = new System.Drawing.Point(80, 40);
            this.comboBoxBank.Name = "comboBoxBank";
            this.comboBoxBank.Size = new System.Drawing.Size(56, 23);
            this.comboBoxBank.TabIndex = 24;
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.Black;
            this.label6.Location = new System.Drawing.Point(278, 33);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(71, 40);
            this.label6.TabIndex = 21;
            this.label6.Text = "Level 번호";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // comboBoxBatchno
            // 
            this.comboBoxBatchno.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBoxBatchno.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBoxBatchno.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBoxBatchno.FormattingEnabled = true;
            this.comboBoxBatchno.Location = new System.Drawing.Point(321, 11);
            this.comboBoxBatchno.Name = "comboBoxBatchno";
            this.comboBoxBatchno.Size = new System.Drawing.Size(152, 23);
            this.comboBoxBatchno.TabIndex = 24;
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(139, 33);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(64, 40);
            this.label5.TabIndex = 21;
            this.label5.Text = "Bay 번호";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // comboBoxMaterialno
            // 
            this.comboBoxMaterialno.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBoxMaterialno.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBoxMaterialno.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBoxMaterialno.FormattingEnabled = true;
            this.comboBoxMaterialno.Location = new System.Drawing.Point(80, 11);
            this.comboBoxMaterialno.Name = "comboBoxMaterialno";
            this.comboBoxMaterialno.Size = new System.Drawing.Size(165, 23);
            this.comboBoxMaterialno.TabIndex = 24;
            this.comboBoxMaterialno.SelectedIndexChanged += new System.EventHandler(this.comboBoxMaterialno_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.Black;
            this.label4.Location = new System.Drawing.Point(6, 33);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(71, 40);
            this.label4.TabIndex = 21;
            this.label4.Text = "Bank 번호";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(242, 1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 40);
            this.label1.TabIndex = 21;
            this.label1.Text = "Batch 번호";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.Black;
            this.label3.Location = new System.Drawing.Point(6, 1);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(71, 40);
            this.label3.TabIndex = 21;
            this.label3.Text = "자재 번호";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // textBoxPalletid
            // 
            this.textBoxPalletid.Location = new System.Drawing.Point(80, 69);
            this.textBoxPalletid.Name = "textBoxPalletid";
            this.textBoxPalletid.Size = new System.Drawing.Size(123, 21);
            this.textBoxPalletid.TabIndex = 32;
            // 
            // label7
            // 
            this.label7.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.ForeColor = System.Drawing.Color.Black;
            this.label7.Location = new System.Drawing.Point(6, 69);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(71, 23);
            this.label7.TabIndex = 31;
            this.label7.Text = "Pallet 번호";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // comboBoxCompanytype
            // 
            this.comboBoxCompanytype.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxCompanytype.FormattingEnabled = true;
            this.comboBoxCompanytype.Location = new System.Drawing.Point(281, 70);
            this.comboBoxCompanytype.Name = "comboBoxCompanytype";
            this.comboBoxCompanytype.Size = new System.Drawing.Size(206, 20);
            this.comboBoxCompanytype.TabIndex = 33;
            // 
            // label8
            // 
            this.label8.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.ForeColor = System.Drawing.Color.Black;
            this.label8.Location = new System.Drawing.Point(210, 69);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(71, 23);
            this.label8.TabIndex = 34;
            this.label8.Text = "회사 유형";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
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
            this.buttonModify.Location = new System.Drawing.Point(697, 0);
            this.buttonModify.Name = "buttonModify";
            this.buttonModify.Size = new System.Drawing.Size(81, 44);
            this.buttonModify.TabIndex = 16;
            this.buttonModify.Text = "수정";
            this.buttonModify.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonModify.UseVisualStyleBackColor = true;
            this.buttonModify.Click += new System.EventHandler(this.buttonModify_Click);
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
            this.buttonSave.Location = new System.Drawing.Point(778, 0);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(81, 44);
            this.buttonSave.TabIndex = 17;
            this.buttonSave.Text = "저장";
            this.buttonSave.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // buttonXPExcel
            // 
            this.buttonXPExcel.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXPExcel.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXPExcel.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXPExcel.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXPExcel.Image = global::HECS.Properties.Resources.bullet50043600;
            this.buttonXPExcel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXPExcel.Location = new System.Drawing.Point(770, 53);
            this.buttonXPExcel.Name = "buttonXPExcel";
            this.buttonXPExcel.Size = new System.Drawing.Size(81, 40);
            this.buttonXPExcel.TabIndex = 28;
            this.buttonXPExcel.Text = "Excel";
            this.buttonXPExcel.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXPExcel.UseVisualStyleBackColor = true;
            this.buttonXPExcel.Click += new System.EventHandler(this.buttonXPExcel_Click);
            // 
            // buttonXPRetrieve
            // 
            this.buttonXPRetrieve.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXPRetrieve.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXPRetrieve.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXPRetrieve.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXPRetrieve.Image = ((System.Drawing.Image)(resources.GetObject("buttonXPRetrieve.Image")));
            this.buttonXPRetrieve.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXPRetrieve.Location = new System.Drawing.Point(690, 53);
            this.buttonXPRetrieve.Name = "buttonXPRetrieve";
            this.buttonXPRetrieve.Size = new System.Drawing.Size(81, 40);
            this.buttonXPRetrieve.TabIndex = 29;
            this.buttonXPRetrieve.Text = "조회";
            this.buttonXPRetrieve.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXPRetrieve.UseVisualStyleBackColor = true;
            this.buttonXPRetrieve.Click += new System.EventHandler(this.buttonXPRetrieve_Click);
            // 
            // HiRackStatusMainPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.label2);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.panel1);
            this.Name = "HiRackStatusMainPanel";
            this.Size = new System.Drawing.Size(863, 508);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.panelProgress.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.Panel panel2;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonModify;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonSave;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel1;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXPExcel;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXPRetrieve;
        private System.Windows.Forms.ComboBox comboBoxMaterialno;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox comboBoxBatchno;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBoxLevel;
        private System.Windows.Forms.ComboBox comboBoxBay;
        private System.Windows.Forms.ComboBox comboBoxBank;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox checkBoxBlockingFlag;
        private System.Windows.Forms.CheckBox checkBoxEmpty;
        private System.Windows.Forms.Panel panelProgress;
        private System.Windows.Forms.Label labelProgressMessage;
        private System.Windows.Forms.TextBox textBoxPalletid;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox comboBoxCompanytype;
    }
}
