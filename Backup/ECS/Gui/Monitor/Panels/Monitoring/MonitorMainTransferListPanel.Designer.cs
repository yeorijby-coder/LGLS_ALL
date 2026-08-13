namespace HECS.Gui.Monitor.Panels.Monitoring
{
    partial class MonitorMainTransferListPanel
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MonitorMainTransferListPanel));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle9 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle10 = new System.Windows.Forms.DataGridViewCellStyle();
            this.dataGridViewTransferList = new System.Windows.Forms.DataGridView();
            this.imageListTransferState = new System.Windows.Forms.ImageList(this.components);
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.dataGridViewTransferDetail = new System.Windows.Forms.DataGridView();
            this.panel1 = new System.Windows.Forms.Panel();
            this.labelBatch = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.buttonRetry = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonComplete = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.labelPalletNo = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelCommandId = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.buttonTransferControl = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonDown = new HECS.Gui.Jaeit.Buttons.ButtonRect();
            this.buttonUp = new HECS.Gui.Jaeit.Buttons.ButtonRect();
            this.textBoxPriority = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewTransferList)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewTransferDetail)).BeginInit();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
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
            dataGridViewCellStyle2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferList.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridViewTransferList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferList.DefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridViewTransferList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewTransferList.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridViewTransferList.GridColor = System.Drawing.Color.DarkKhaki;
            this.dataGridViewTransferList.Location = new System.Drawing.Point(2, 2);
            this.dataGridViewTransferList.MultiSelect = false;
            this.dataGridViewTransferList.Name = "dataGridViewTransferList";
            this.dataGridViewTransferList.ReadOnly = true;
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle4.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle4.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
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
            this.dataGridViewTransferList.Size = new System.Drawing.Size(394, 229);
            this.dataGridViewTransferList.TabIndex = 7;
            this.dataGridViewTransferList.TabStop = false;
            this.dataGridViewTransferList.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_CellClick);
            this.dataGridViewTransferList.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridViewTransferList_CellContentClick);
            // 
            // imageListTransferState
            // 
            this.imageListTransferState.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListTransferState.ImageStream")));
            this.imageListTransferState.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListTransferState.Images.SetKeyName(0, "NONE.ico");
            this.imageListTransferState.Images.SetKeyName(1, "Queued.ico");
            this.imageListTransferState.Images.SetKeyName(2, "Ready.ico");
            this.imageListTransferState.Images.SetKeyName(3, "NotReady.ico");
            this.imageListTransferState.Images.SetKeyName(4, "MoveRequest.ico");
            this.imageListTransferState.Images.SetKeyName(5, "Transferring.ico");
            this.imageListTransferState.Images.SetKeyName(6, "Paused.ico");
            this.imageListTransferState.Images.SetKeyName(7, "Alternate.ico");
            this.imageListTransferState.Images.SetKeyName(8, "AltHandoff.ico");
            this.imageListTransferState.Images.SetKeyName(9, "Canceling.ico");
            this.imageListTransferState.Images.SetKeyName(10, "Aborting.ico");
            this.imageListTransferState.Images.SetKeyName(11, "Complete.ico");
            this.imageListTransferState.Images.SetKeyName(12, "AbortComplete.ico");
            this.imageListTransferState.Images.SetKeyName(13, "AbortFail.ico");
            this.imageListTransferState.Images.SetKeyName(14, "CancelComplete.ico");
            this.imageListTransferState.Images.SetKeyName(15, "CancelFail.ico");
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(96, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Panel1.Controls.Add(this.dataGridViewTransferList);
            this.splitContainer1.Panel1.Padding = new System.Windows.Forms.Padding(2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer1.Panel2.Controls.Add(this.dataGridViewTransferDetail);
            this.splitContainer1.Panel2.Controls.Add(this.panel1);
            this.splitContainer1.Panel2.Padding = new System.Windows.Forms.Padding(2);
            this.splitContainer1.Size = new System.Drawing.Size(885, 233);
            this.splitContainer1.SplitterDistance = 398;
            this.splitContainer1.TabIndex = 8;
            // 
            // dataGridViewTransferDetail
            // 
            this.dataGridViewTransferDetail.AllowUserToAddRows = false;
            this.dataGridViewTransferDetail.AllowUserToDeleteRows = false;
            this.dataGridViewTransferDetail.AllowUserToOrderColumns = true;
            this.dataGridViewTransferDetail.AllowUserToResizeRows = false;
            dataGridViewCellStyle6.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle6.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle6.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle6.SelectionForeColor = System.Drawing.Color.White;
            this.dataGridViewTransferDetail.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle6;
            this.dataGridViewTransferDetail.BackgroundColor = System.Drawing.Color.LemonChiffon;
            this.dataGridViewTransferDetail.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dataGridViewTransferDetail.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.SingleHorizontal;
            dataGridViewCellStyle7.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle7.BackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle7.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle7.ForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle7.SelectionBackColor = System.Drawing.Color.Khaki;
            dataGridViewCellStyle7.SelectionForeColor = System.Drawing.Color.Olive;
            dataGridViewCellStyle7.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferDetail.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle7;
            this.dataGridViewTransferDetail.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            dataGridViewCellStyle8.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle8.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle8.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle8.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle8.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle8.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle8.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferDetail.DefaultCellStyle = dataGridViewCellStyle8;
            this.dataGridViewTransferDetail.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewTransferDetail.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dataGridViewTransferDetail.GridColor = System.Drawing.Color.DarkKhaki;
            this.dataGridViewTransferDetail.Location = new System.Drawing.Point(2, 19);
            this.dataGridViewTransferDetail.Name = "dataGridViewTransferDetail";
            this.dataGridViewTransferDetail.ReadOnly = true;
            dataGridViewCellStyle9.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle9.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle9.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle9.ForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle9.NullValue = "☞";
            dataGridViewCellStyle9.SelectionBackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle9.SelectionForeColor = System.Drawing.Color.Red;
            dataGridViewCellStyle9.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferDetail.RowHeadersDefaultCellStyle = dataGridViewCellStyle9;
            this.dataGridViewTransferDetail.RowHeadersVisible = false;
            this.dataGridViewTransferDetail.RowHeadersWidth = 20;
            this.dataGridViewTransferDetail.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            dataGridViewCellStyle10.BackColor = System.Drawing.Color.LemonChiffon;
            dataGridViewCellStyle10.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle10.SelectionBackColor = System.Drawing.Color.RoyalBlue;
            dataGridViewCellStyle10.SelectionForeColor = System.Drawing.Color.White;
            dataGridViewCellStyle10.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewTransferDetail.RowsDefaultCellStyle = dataGridViewCellStyle10;
            this.dataGridViewTransferDetail.RowTemplate.DefaultCellStyle.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dataGridViewTransferDetail.RowTemplate.Height = 20;
            this.dataGridViewTransferDetail.RowTemplate.ReadOnly = true;
            this.dataGridViewTransferDetail.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridViewTransferDetail.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewTransferDetail.ShowCellErrors = false;
            this.dataGridViewTransferDetail.ShowCellToolTips = false;
            this.dataGridViewTransferDetail.ShowEditingIcon = false;
            this.dataGridViewTransferDetail.ShowRowErrors = false;
            this.dataGridViewTransferDetail.Size = new System.Drawing.Size(479, 212);
            this.dataGridViewTransferDetail.TabIndex = 11;
            this.dataGridViewTransferDetail.TabStop = false;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Khaki;
            this.panel1.Controls.Add(this.labelBatch);
            this.panel1.Controls.Add(this.label7);
            this.panel1.Controls.Add(this.buttonRetry);
            this.panel1.Controls.Add(this.buttonComplete);
            this.panel1.Controls.Add(this.labelPalletNo);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.labelCommandId);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(2, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(479, 17);
            this.panel1.TabIndex = 12;
            // 
            // labelBatch
            // 
            this.labelBatch.BackColor = System.Drawing.Color.Khaki;
            this.labelBatch.Dock = System.Windows.Forms.DockStyle.Left;
            this.labelBatch.Font = new System.Drawing.Font("Dotum", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelBatch.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.labelBatch.Location = new System.Drawing.Point(331, 0);
            this.labelBatch.Name = "labelBatch";
            this.labelBatch.Size = new System.Drawing.Size(25, 17);
            this.labelBatch.TabIndex = 10;
            this.labelBatch.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.labelBatch.Visible = false;
            // 
            // label7
            // 
            this.label7.BackColor = System.Drawing.Color.Khaki;
            this.label7.Dock = System.Windows.Forms.DockStyle.Left;
            this.label7.Font = new System.Drawing.Font("Dotum", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label7.ForeColor = System.Drawing.Color.DimGray;
            this.label7.Location = new System.Drawing.Point(321, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(10, 17);
            this.label7.TabIndex = 9;
            this.label7.Text = "Batch";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.label7.Visible = false;
            // 
            // buttonRetry
            // 
            this.buttonRetry.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonRetry.BackColor = System.Drawing.Color.LemonChiffon;
            this.buttonRetry.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonRetry.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonRetry.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonRetry.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonRetry.ForeColor = System.Drawing.Color.Green;
            this.buttonRetry.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonRetry.Location = new System.Drawing.Point(327, 0);
            this.buttonRetry.Name = "buttonRetry";
            this.buttonRetry.Size = new System.Drawing.Size(70, 17);
            this.buttonRetry.TabIndex = 17;
            this.buttonRetry.Text = "재 지시";
            this.buttonRetry.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonRetry.UseVisualStyleBackColor = false;
            this.buttonRetry.Visible = false;
            // 
            // buttonComplete
            // 
            this.buttonComplete.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonComplete.BackColor = System.Drawing.Color.LemonChiffon;
            this.buttonComplete.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonComplete.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonComplete.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonComplete.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonComplete.ForeColor = System.Drawing.Color.Crimson;
            this.buttonComplete.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonComplete.Location = new System.Drawing.Point(397, 0);
            this.buttonComplete.Name = "buttonComplete";
            this.buttonComplete.Size = new System.Drawing.Size(82, 17);
            this.buttonComplete.TabIndex = 16;
            this.buttonComplete.Text = "완료처리";
            this.buttonComplete.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonComplete.UseVisualStyleBackColor = false;
            this.buttonComplete.Click += new System.EventHandler(this.buttonComplete_Click);
            // 
            // labelPalletNo
            // 
            this.labelPalletNo.BackColor = System.Drawing.Color.Khaki;
            this.labelPalletNo.Dock = System.Windows.Forms.DockStyle.Left;
            this.labelPalletNo.Font = new System.Drawing.Font("Dotum", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelPalletNo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.labelPalletNo.Location = new System.Drawing.Point(243, 0);
            this.labelPalletNo.Name = "labelPalletNo";
            this.labelPalletNo.Size = new System.Drawing.Size(78, 17);
            this.labelPalletNo.TabIndex = 15;
            this.labelPalletNo.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.BackColor = System.Drawing.Color.Khaki;
            this.label3.Dock = System.Windows.Forms.DockStyle.Left;
            this.label3.Font = new System.Drawing.Font("Dotum", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label3.ForeColor = System.Drawing.Color.DimGray;
            this.label3.Location = new System.Drawing.Point(180, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 17);
            this.label3.TabIndex = 14;
            this.label3.Text = "작업번호";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelCommandId
            // 
            this.labelCommandId.BackColor = System.Drawing.Color.Khaki;
            this.labelCommandId.Dock = System.Windows.Forms.DockStyle.Left;
            this.labelCommandId.Font = new System.Drawing.Font("Dotum", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelCommandId.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.labelCommandId.Location = new System.Drawing.Point(64, 0);
            this.labelCommandId.Name = "labelCommandId";
            this.labelCommandId.Size = new System.Drawing.Size(116, 17);
            this.labelCommandId.TabIndex = 12;
            this.labelCommandId.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.Khaki;
            this.label2.Dock = System.Windows.Forms.DockStyle.Left;
            this.label2.Font = new System.Drawing.Font("Dotum", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label2.ForeColor = System.Drawing.Color.DimGray;
            this.label2.Location = new System.Drawing.Point(0, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 17);
            this.label2.TabIndex = 11;
            this.label2.Text = "ECS번호";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Khaki;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.buttonTransferControl);
            this.panel2.Controls.Add(this.buttonDown);
            this.panel2.Controls.Add(this.buttonUp);
            this.panel2.Controls.Add(this.textBoxPriority);
            this.panel2.Controls.Add(this.label6);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(4);
            this.panel2.Size = new System.Drawing.Size(92, 233);
            this.panel2.TabIndex = 9;
            // 
            // buttonTransferControl
            // 
            this.buttonTransferControl.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonTransferControl.BackColor = System.Drawing.Color.LemonChiffon;
            this.buttonTransferControl.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonTransferControl.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonTransferControl.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonTransferControl.Image = global::HECS.Properties.Resources.Option;
            this.buttonTransferControl.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonTransferControl.Location = new System.Drawing.Point(4, 109);
            this.buttonTransferControl.Name = "buttonTransferControl";
            this.buttonTransferControl.Size = new System.Drawing.Size(81, 55);
            this.buttonTransferControl.TabIndex = 3;
            this.buttonTransferControl.Text = "반송조정";
            this.buttonTransferControl.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonTransferControl.UseVisualStyleBackColor = false;
            this.buttonTransferControl.Click += new System.EventHandler(this.buttonTransferControl_Click);
            // 
            // buttonDown
            // 
            this.buttonDown.ButtonForm = HECS.Gui.Jaeit.Buttons.eButtonForm.Rectangle;
            this.buttonDown.HighlightColor = System.Drawing.Color.Transparent;
            this.buttonDown.HottrackImage = null;
            this.buttonDown.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonDown.Location = new System.Drawing.Point(52, 65);
            this.buttonDown.Name = "buttonDown";
            this.buttonDown.NormalImage = global::HECS.Properties.Resources.DOWN3;
            this.buttonDown.OnlyShowBitmap = false;
            this.buttonDown.PressedImage = null;
            this.buttonDown.Size = new System.Drawing.Size(31, 38);
            this.buttonDown.TextAlign = HECS.Gui.Jaeit.Buttons.eTextAlign.Bottom;
            this.buttonDown.ToolTip = null;
            // 
            // buttonUp
            // 
            this.buttonUp.ButtonForm = HECS.Gui.Jaeit.Buttons.eButtonForm.Rectangle;
            this.buttonUp.HighlightColor = System.Drawing.Color.Transparent;
            this.buttonUp.HottrackImage = null;
            this.buttonUp.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonUp.Location = new System.Drawing.Point(7, 65);
            this.buttonUp.Name = "buttonUp";
            this.buttonUp.NormalImage = global::HECS.Properties.Resources.UP;
            this.buttonUp.OnlyShowBitmap = false;
            this.buttonUp.PressedImage = null;
            this.buttonUp.Size = new System.Drawing.Size(31, 38);
            this.buttonUp.TextAlign = HECS.Gui.Jaeit.Buttons.eTextAlign.Bottom;
            this.buttonUp.ToolTip = null;
            this.buttonUp.Click += new System.EventHandler(this.buttonUp_Click);
            // 
            // textBoxPriority
            // 
            this.textBoxPriority.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxPriority.Font = new System.Drawing.Font("Arial", 21.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxPriority.Location = new System.Drawing.Point(4, 22);
            this.textBoxPriority.Name = "textBoxPriority";
            this.textBoxPriority.Size = new System.Drawing.Size(82, 41);
            this.textBoxPriority.TabIndex = 1;
            this.textBoxPriority.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label6
            // 
            this.label6.Dock = System.Windows.Forms.DockStyle.Top;
            this.label6.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(4, 4);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(82, 18);
            this.label6.TabIndex = 2;
            this.label6.Text = "우선순위";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label1.Dock = System.Windows.Forms.DockStyle.Left;
            this.label1.Location = new System.Drawing.Point(92, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(4, 233);
            this.label1.TabIndex = 10;
            // 
            // MonitorMainTransferListPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.RoyalBlue;
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.panel2);
            this.DoubleBuffered = true;
            this.Name = "MonitorMainTransferListPanel";
            this.Size = new System.Drawing.Size(981, 233);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewTransferList)).EndInit();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewTransferDetail)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridViewTransferList;
        private System.Windows.Forms.ImageList imageListTransferState;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.DataGridView dataGridViewTransferDetail;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label labelCommandId;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelBatch;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label labelPalletNo;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox textBoxPriority;
        private HECS.Gui.Jaeit.Buttons.ButtonRect buttonUp;
        private HECS.Gui.Jaeit.Buttons.ButtonRect buttonDown;
        private System.Windows.Forms.Label label1;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonTransferControl;
        private System.Windows.Forms.Label label6;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonRetry;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonComplete;

    }
}
