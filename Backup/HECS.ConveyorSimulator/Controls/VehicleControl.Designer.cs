namespace HECS.ConveyorSimulator.Controls
{
    partial class VehicleControl
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
            this.textBoxFrom1 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonAlarmResetReportACK = new System.Windows.Forms.Button();
            this.buttonLoadCompleteACK = new System.Windows.Forms.Button();
            this.buttonAlarmSetReportACK = new System.Windows.Forms.Button();
            this.buttonPalletExist = new System.Windows.Forms.Button();
            this.labelVehicleId = new System.Windows.Forms.Label();
            this.buttonLoadComplete = new System.Windows.Forms.Button();
            this.buttonUnloadComplete = new System.Windows.Forms.Button();
            this.buttonAlarmResetReport = new System.Windows.Forms.Button();
            this.buttonTransferRequest = new System.Windows.Forms.Button();
            this.buttonAlarmSetReport = new System.Windows.Forms.Button();
            this.buttonTransferRequestACK = new System.Windows.Forms.Button();
            this.buttonUnloadCompleteACK = new System.Windows.Forms.Button();
            this.textBoxFrom2 = new System.Windows.Forms.TextBox();
            this.textBoxFrom3 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxTo1 = new System.Windows.Forms.TextBox();
            this.textBoxTo2 = new System.Windows.Forms.TextBox();
            this.textBoxTo3 = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.textBoxLocation1 = new System.Windows.Forms.TextBox();
            this.textBoxLocation2 = new System.Windows.Forms.TextBox();
            this.textBoxLocation3 = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.textBoxCompleteLocation1 = new System.Windows.Forms.TextBox();
            this.textBoxCompleteLocation2 = new System.Windows.Forms.TextBox();
            this.textBoxCompleteLocation3 = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.textBoxPalletIdTransfer = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxPalletIdOnVehicle = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxAlarmSetCode = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxAlarmResetCode = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBoxOperationMode = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBoxVehicleState = new System.Windows.Forms.TextBox();
            this.textBoxInputFrom1 = new System.Windows.Forms.TextBox();
            this.textBoxInputFrom2 = new System.Windows.Forms.TextBox();
            this.textBoxInputFrom3 = new System.Windows.Forms.TextBox();
            this.textBoxInputTo1 = new System.Windows.Forms.TextBox();
            this.textBoxInputTo2 = new System.Windows.Forms.TextBox();
            this.textBoxInputTo3 = new System.Windows.Forms.TextBox();
            this.textBoxInputPalletIdTransfer = new System.Windows.Forms.TextBox();
            this.textBoxInputLocation1 = new System.Windows.Forms.TextBox();
            this.textBoxInputLocation2 = new System.Windows.Forms.TextBox();
            this.textBoxInputLocation3 = new System.Windows.Forms.TextBox();
            this.textBoxInputCompleteLocation1 = new System.Windows.Forms.TextBox();
            this.textBoxInputCompleteLocation2 = new System.Windows.Forms.TextBox();
            this.textBoxInputCompleteLocation3 = new System.Windows.Forms.TextBox();
            this.textBoxInputPalletIdOnVehicle = new System.Windows.Forms.TextBox();
            this.textBoxInputAlarmSetCode = new System.Windows.Forms.TextBox();
            this.textBoxInputAlarmResetCode = new System.Windows.Forms.TextBox();
            this.textBoxInputOperationMode = new System.Windows.Forms.TextBox();
            this.textBoxInputVehicleState = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textBoxFrom1
            // 
            this.textBoxFrom1.AcceptsReturn = true;
            this.textBoxFrom1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxFrom1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxFrom1.Location = new System.Drawing.Point(227, 22);
            this.textBoxFrom1.Name = "textBoxFrom1";
            this.textBoxFrom1.ReadOnly = true;
            this.textBoxFrom1.Size = new System.Drawing.Size(18, 20);
            this.textBoxFrom1.TabIndex = 94;
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(127, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 20);
            this.label2.TabIndex = 89;
            this.label2.Text = "FROM";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonAlarmResetReportACK
            // 
            this.buttonAlarmResetReportACK.BackColor = System.Drawing.Color.LightGray;
            this.buttonAlarmResetReportACK.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonAlarmResetReportACK.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAlarmResetReportACK.ForeColor = System.Drawing.Color.Black;
            this.buttonAlarmResetReportACK.Location = new System.Drawing.Point(2, 159);
            this.buttonAlarmResetReportACK.Name = "buttonAlarmResetReportACK";
            this.buttonAlarmResetReportACK.Size = new System.Drawing.Size(126, 20);
            this.buttonAlarmResetReportACK.TabIndex = 80;
            this.buttonAlarmResetReportACK.Text = "Alarm Reset Report ACK";
            this.buttonAlarmResetReportACK.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonAlarmResetReportACK.UseVisualStyleBackColor = false;
            this.buttonAlarmResetReportACK.Click += new System.EventHandler(this.buttonAlarmResetReportACK_Click);
            // 
            // buttonLoadCompleteACK
            // 
            this.buttonLoadCompleteACK.BackColor = System.Drawing.Color.LightGray;
            this.buttonLoadCompleteACK.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonLoadCompleteACK.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonLoadCompleteACK.ForeColor = System.Drawing.Color.Black;
            this.buttonLoadCompleteACK.Location = new System.Drawing.Point(2, 179);
            this.buttonLoadCompleteACK.Name = "buttonLoadCompleteACK";
            this.buttonLoadCompleteACK.Size = new System.Drawing.Size(126, 20);
            this.buttonLoadCompleteACK.TabIndex = 77;
            this.buttonLoadCompleteACK.Text = "Load Complete ACK";
            this.buttonLoadCompleteACK.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonLoadCompleteACK.UseVisualStyleBackColor = false;
            this.buttonLoadCompleteACK.Click += new System.EventHandler(this.buttonLoadCompleteACK_Click);
            // 
            // buttonAlarmSetReportACK
            // 
            this.buttonAlarmSetReportACK.BackColor = System.Drawing.Color.LightGray;
            this.buttonAlarmSetReportACK.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonAlarmSetReportACK.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAlarmSetReportACK.ForeColor = System.Drawing.Color.Black;
            this.buttonAlarmSetReportACK.Location = new System.Drawing.Point(2, 139);
            this.buttonAlarmSetReportACK.Name = "buttonAlarmSetReportACK";
            this.buttonAlarmSetReportACK.Size = new System.Drawing.Size(126, 20);
            this.buttonAlarmSetReportACK.TabIndex = 76;
            this.buttonAlarmSetReportACK.Text = "Alarm Set Report ACK";
            this.buttonAlarmSetReportACK.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonAlarmSetReportACK.UseVisualStyleBackColor = false;
            this.buttonAlarmSetReportACK.Click += new System.EventHandler(this.buttonAlarmSetReportACK_Click);
            // 
            // buttonPalletExist
            // 
            this.buttonPalletExist.BackColor = System.Drawing.Color.LightGray;
            this.buttonPalletExist.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonPalletExist.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonPalletExist.ForeColor = System.Drawing.Color.Black;
            this.buttonPalletExist.Location = new System.Drawing.Point(2, 119);
            this.buttonPalletExist.Name = "buttonPalletExist";
            this.buttonPalletExist.Size = new System.Drawing.Size(126, 20);
            this.buttonPalletExist.TabIndex = 74;
            this.buttonPalletExist.Text = "Pallet Exist";
            this.buttonPalletExist.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonPalletExist.UseVisualStyleBackColor = false;
            this.buttonPalletExist.Click += new System.EventHandler(this.buttonPalletExist_Click);
            // 
            // labelVehicleId
            // 
            this.labelVehicleId.BackColor = System.Drawing.Color.Blue;
            this.labelVehicleId.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.labelVehicleId.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelVehicleId.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelVehicleId.ForeColor = System.Drawing.Color.White;
            this.labelVehicleId.Location = new System.Drawing.Point(1, 1);
            this.labelVehicleId.Name = "labelVehicleId";
            this.labelVehicleId.Size = new System.Drawing.Size(337, 16);
            this.labelVehicleId.TabIndex = 73;
            this.labelVehicleId.Text = "VehicleId";
            this.labelVehicleId.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // buttonLoadComplete
            // 
            this.buttonLoadComplete.BackColor = System.Drawing.Color.LightGray;
            this.buttonLoadComplete.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonLoadComplete.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonLoadComplete.ForeColor = System.Drawing.Color.Black;
            this.buttonLoadComplete.Location = new System.Drawing.Point(2, 59);
            this.buttonLoadComplete.Name = "buttonLoadComplete";
            this.buttonLoadComplete.Size = new System.Drawing.Size(126, 20);
            this.buttonLoadComplete.TabIndex = 71;
            this.buttonLoadComplete.Text = "Load Complete";
            this.buttonLoadComplete.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonLoadComplete.UseVisualStyleBackColor = false;
            this.buttonLoadComplete.Click += new System.EventHandler(this.buttonLoadComplete_Click);
            // 
            // buttonUnloadComplete
            // 
            this.buttonUnloadComplete.BackColor = System.Drawing.Color.LightGray;
            this.buttonUnloadComplete.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonUnloadComplete.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonUnloadComplete.ForeColor = System.Drawing.Color.Black;
            this.buttonUnloadComplete.Location = new System.Drawing.Point(2, 79);
            this.buttonUnloadComplete.Name = "buttonUnloadComplete";
            this.buttonUnloadComplete.Size = new System.Drawing.Size(126, 20);
            this.buttonUnloadComplete.TabIndex = 68;
            this.buttonUnloadComplete.Text = "Unload Complete";
            this.buttonUnloadComplete.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonUnloadComplete.UseVisualStyleBackColor = false;
            this.buttonUnloadComplete.Click += new System.EventHandler(this.buttonUnloadComplete_Click);
            // 
            // buttonAlarmResetReport
            // 
            this.buttonAlarmResetReport.BackColor = System.Drawing.Color.LightGray;
            this.buttonAlarmResetReport.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonAlarmResetReport.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAlarmResetReport.ForeColor = System.Drawing.Color.Black;
            this.buttonAlarmResetReport.Location = new System.Drawing.Point(2, 39);
            this.buttonAlarmResetReport.Name = "buttonAlarmResetReport";
            this.buttonAlarmResetReport.Size = new System.Drawing.Size(126, 20);
            this.buttonAlarmResetReport.TabIndex = 67;
            this.buttonAlarmResetReport.Text = "Alarm Reset Report";
            this.buttonAlarmResetReport.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonAlarmResetReport.UseVisualStyleBackColor = false;
            this.buttonAlarmResetReport.Click += new System.EventHandler(this.buttonAlarmResetReport_Click);
            // 
            // buttonTransferRequest
            // 
            this.buttonTransferRequest.BackColor = System.Drawing.Color.LightGray;
            this.buttonTransferRequest.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonTransferRequest.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonTransferRequest.ForeColor = System.Drawing.Color.Black;
            this.buttonTransferRequest.Location = new System.Drawing.Point(2, 99);
            this.buttonTransferRequest.Name = "buttonTransferRequest";
            this.buttonTransferRequest.Size = new System.Drawing.Size(126, 20);
            this.buttonTransferRequest.TabIndex = 63;
            this.buttonTransferRequest.Text = "Transfer Request";
            this.buttonTransferRequest.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonTransferRequest.UseVisualStyleBackColor = false;
            this.buttonTransferRequest.Click += new System.EventHandler(this.buttonTransferRequest_Click);
            // 
            // buttonAlarmSetReport
            // 
            this.buttonAlarmSetReport.BackColor = System.Drawing.Color.LightGray;
            this.buttonAlarmSetReport.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonAlarmSetReport.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAlarmSetReport.ForeColor = System.Drawing.Color.Black;
            this.buttonAlarmSetReport.Location = new System.Drawing.Point(2, 19);
            this.buttonAlarmSetReport.Name = "buttonAlarmSetReport";
            this.buttonAlarmSetReport.Size = new System.Drawing.Size(126, 20);
            this.buttonAlarmSetReport.TabIndex = 64;
            this.buttonAlarmSetReport.Text = "Alarm Set Report";
            this.buttonAlarmSetReport.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonAlarmSetReport.UseVisualStyleBackColor = false;
            this.buttonAlarmSetReport.Click += new System.EventHandler(this.buttonAlarmSetReport_Click);
            // 
            // buttonTransferRequestACK
            // 
            this.buttonTransferRequestACK.BackColor = System.Drawing.Color.LightGray;
            this.buttonTransferRequestACK.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonTransferRequestACK.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonTransferRequestACK.ForeColor = System.Drawing.Color.Black;
            this.buttonTransferRequestACK.Location = new System.Drawing.Point(2, 220);
            this.buttonTransferRequestACK.Name = "buttonTransferRequestACK";
            this.buttonTransferRequestACK.Size = new System.Drawing.Size(126, 20);
            this.buttonTransferRequestACK.TabIndex = 77;
            this.buttonTransferRequestACK.Text = "Transfer Request ACK";
            this.buttonTransferRequestACK.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonTransferRequestACK.UseVisualStyleBackColor = false;
            this.buttonTransferRequestACK.Click += new System.EventHandler(this.buttonTransferRequestACK_Click);
            // 
            // buttonUnloadCompleteACK
            // 
            this.buttonUnloadCompleteACK.BackColor = System.Drawing.Color.LightGray;
            this.buttonUnloadCompleteACK.FlatAppearance.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonUnloadCompleteACK.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonUnloadCompleteACK.ForeColor = System.Drawing.Color.Black;
            this.buttonUnloadCompleteACK.Location = new System.Drawing.Point(2, 200);
            this.buttonUnloadCompleteACK.Name = "buttonUnloadCompleteACK";
            this.buttonUnloadCompleteACK.Size = new System.Drawing.Size(126, 20);
            this.buttonUnloadCompleteACK.TabIndex = 80;
            this.buttonUnloadCompleteACK.Text = "Unload Complete ACK";
            this.buttonUnloadCompleteACK.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.buttonUnloadCompleteACK.UseVisualStyleBackColor = false;
            this.buttonUnloadCompleteACK.Click += new System.EventHandler(this.buttonUnloadCompleteACK_Click);
            // 
            // textBoxFrom2
            // 
            this.textBoxFrom2.AcceptsReturn = true;
            this.textBoxFrom2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxFrom2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxFrom2.Location = new System.Drawing.Point(245, 22);
            this.textBoxFrom2.Name = "textBoxFrom2";
            this.textBoxFrom2.ReadOnly = true;
            this.textBoxFrom2.Size = new System.Drawing.Size(18, 20);
            this.textBoxFrom2.TabIndex = 94;
            // 
            // textBoxFrom3
            // 
            this.textBoxFrom3.AcceptsReturn = true;
            this.textBoxFrom3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxFrom3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxFrom3.Location = new System.Drawing.Point(263, 22);
            this.textBoxFrom3.Name = "textBoxFrom3";
            this.textBoxFrom3.ReadOnly = true;
            this.textBoxFrom3.Size = new System.Drawing.Size(18, 20);
            this.textBoxFrom3.TabIndex = 94;
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(127, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(98, 20);
            this.label1.TabIndex = 89;
            this.label1.Text = "TO";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxTo1
            // 
            this.textBoxTo1.AcceptsReturn = true;
            this.textBoxTo1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxTo1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxTo1.Location = new System.Drawing.Point(227, 42);
            this.textBoxTo1.Name = "textBoxTo1";
            this.textBoxTo1.ReadOnly = true;
            this.textBoxTo1.Size = new System.Drawing.Size(18, 20);
            this.textBoxTo1.TabIndex = 94;
            // 
            // textBoxTo2
            // 
            this.textBoxTo2.AcceptsReturn = true;
            this.textBoxTo2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxTo2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxTo2.Location = new System.Drawing.Point(245, 42);
            this.textBoxTo2.Name = "textBoxTo2";
            this.textBoxTo2.ReadOnly = true;
            this.textBoxTo2.Size = new System.Drawing.Size(18, 20);
            this.textBoxTo2.TabIndex = 94;
            // 
            // textBoxTo3
            // 
            this.textBoxTo3.AcceptsReturn = true;
            this.textBoxTo3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxTo3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxTo3.Location = new System.Drawing.Point(263, 42);
            this.textBoxTo3.Name = "textBoxTo3";
            this.textBoxTo3.ReadOnly = true;
            this.textBoxTo3.Size = new System.Drawing.Size(18, 20);
            this.textBoxTo3.TabIndex = 94;
            // 
            // label11
            // 
            this.label11.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(127, 80);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(98, 20);
            this.label11.TabIndex = 89;
            this.label11.Text = "현위치";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxLocation1
            // 
            this.textBoxLocation1.AcceptsReturn = true;
            this.textBoxLocation1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxLocation1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxLocation1.Location = new System.Drawing.Point(227, 82);
            this.textBoxLocation1.Name = "textBoxLocation1";
            this.textBoxLocation1.ReadOnly = true;
            this.textBoxLocation1.Size = new System.Drawing.Size(19, 20);
            this.textBoxLocation1.TabIndex = 94;
            // 
            // textBoxLocation2
            // 
            this.textBoxLocation2.AcceptsReturn = true;
            this.textBoxLocation2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxLocation2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxLocation2.Location = new System.Drawing.Point(245, 82);
            this.textBoxLocation2.Name = "textBoxLocation2";
            this.textBoxLocation2.ReadOnly = true;
            this.textBoxLocation2.Size = new System.Drawing.Size(19, 20);
            this.textBoxLocation2.TabIndex = 94;
            // 
            // textBoxLocation3
            // 
            this.textBoxLocation3.AcceptsReturn = true;
            this.textBoxLocation3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxLocation3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxLocation3.Location = new System.Drawing.Point(262, 82);
            this.textBoxLocation3.Name = "textBoxLocation3";
            this.textBoxLocation3.ReadOnly = true;
            this.textBoxLocation3.Size = new System.Drawing.Size(19, 20);
            this.textBoxLocation3.TabIndex = 94;
            // 
            // label12
            // 
            this.label12.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(127, 100);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(98, 20);
            this.label12.TabIndex = 89;
            this.label12.Text = "완료위치";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxCompleteLocation1
            // 
            this.textBoxCompleteLocation1.AcceptsReturn = true;
            this.textBoxCompleteLocation1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxCompleteLocation1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxCompleteLocation1.Location = new System.Drawing.Point(227, 102);
            this.textBoxCompleteLocation1.Name = "textBoxCompleteLocation1";
            this.textBoxCompleteLocation1.ReadOnly = true;
            this.textBoxCompleteLocation1.Size = new System.Drawing.Size(19, 20);
            this.textBoxCompleteLocation1.TabIndex = 94;
            // 
            // textBoxCompleteLocation2
            // 
            this.textBoxCompleteLocation2.AcceptsReturn = true;
            this.textBoxCompleteLocation2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxCompleteLocation2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxCompleteLocation2.Location = new System.Drawing.Point(245, 102);
            this.textBoxCompleteLocation2.Name = "textBoxCompleteLocation2";
            this.textBoxCompleteLocation2.ReadOnly = true;
            this.textBoxCompleteLocation2.Size = new System.Drawing.Size(19, 20);
            this.textBoxCompleteLocation2.TabIndex = 94;
            // 
            // textBoxCompleteLocation3
            // 
            this.textBoxCompleteLocation3.AcceptsReturn = true;
            this.textBoxCompleteLocation3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxCompleteLocation3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxCompleteLocation3.Location = new System.Drawing.Point(262, 102);
            this.textBoxCompleteLocation3.Name = "textBoxCompleteLocation3";
            this.textBoxCompleteLocation3.ReadOnly = true;
            this.textBoxCompleteLocation3.Size = new System.Drawing.Size(19, 20);
            this.textBoxCompleteLocation3.TabIndex = 94;
            // 
            // label13
            // 
            this.label13.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(127, 60);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(98, 20);
            this.label13.TabIndex = 89;
            this.label13.Text = "Pallet ID";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxPalletIdTransfer
            // 
            this.textBoxPalletIdTransfer.AcceptsReturn = true;
            this.textBoxPalletIdTransfer.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxPalletIdTransfer.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxPalletIdTransfer.Location = new System.Drawing.Point(227, 62);
            this.textBoxPalletIdTransfer.Name = "textBoxPalletIdTransfer";
            this.textBoxPalletIdTransfer.ReadOnly = true;
            this.textBoxPalletIdTransfer.Size = new System.Drawing.Size(54, 20);
            this.textBoxPalletIdTransfer.TabIndex = 94;
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(127, 120);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(98, 20);
            this.label3.TabIndex = 89;
            this.label3.Text = "ID on Vehicle";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxPalletIdOnVehicle
            // 
            this.textBoxPalletIdOnVehicle.AcceptsReturn = true;
            this.textBoxPalletIdOnVehicle.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxPalletIdOnVehicle.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxPalletIdOnVehicle.Location = new System.Drawing.Point(227, 122);
            this.textBoxPalletIdOnVehicle.Name = "textBoxPalletIdOnVehicle";
            this.textBoxPalletIdOnVehicle.ReadOnly = true;
            this.textBoxPalletIdOnVehicle.Size = new System.Drawing.Size(54, 20);
            this.textBoxPalletIdOnVehicle.TabIndex = 94;
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(127, 140);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(98, 20);
            this.label4.TabIndex = 89;
            this.label4.Text = "ALM Set Code";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxAlarmSetCode
            // 
            this.textBoxAlarmSetCode.AcceptsReturn = true;
            this.textBoxAlarmSetCode.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxAlarmSetCode.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxAlarmSetCode.Location = new System.Drawing.Point(227, 142);
            this.textBoxAlarmSetCode.Name = "textBoxAlarmSetCode";
            this.textBoxAlarmSetCode.ReadOnly = true;
            this.textBoxAlarmSetCode.Size = new System.Drawing.Size(54, 20);
            this.textBoxAlarmSetCode.TabIndex = 94;
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(127, 160);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(98, 20);
            this.label5.TabIndex = 89;
            this.label5.Text = "ALM RST Code";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxAlarmResetCode
            // 
            this.textBoxAlarmResetCode.AcceptsReturn = true;
            this.textBoxAlarmResetCode.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxAlarmResetCode.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxAlarmResetCode.Location = new System.Drawing.Point(227, 162);
            this.textBoxAlarmResetCode.Name = "textBoxAlarmResetCode";
            this.textBoxAlarmResetCode.ReadOnly = true;
            this.textBoxAlarmResetCode.Size = new System.Drawing.Size(54, 20);
            this.textBoxAlarmResetCode.TabIndex = 94;
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(127, 180);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(98, 20);
            this.label6.TabIndex = 89;
            this.label6.Text = "OP Mode";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxOperationMode
            // 
            this.textBoxOperationMode.AcceptsReturn = true;
            this.textBoxOperationMode.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxOperationMode.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxOperationMode.Location = new System.Drawing.Point(227, 182);
            this.textBoxOperationMode.Name = "textBoxOperationMode";
            this.textBoxOperationMode.ReadOnly = true;
            this.textBoxOperationMode.Size = new System.Drawing.Size(54, 20);
            this.textBoxOperationMode.TabIndex = 94;
            // 
            // label7
            // 
            this.label7.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(127, 201);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(98, 20);
            this.label7.TabIndex = 89;
            this.label7.Text = "State";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxVehicleState
            // 
            this.textBoxVehicleState.AcceptsReturn = true;
            this.textBoxVehicleState.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.textBoxVehicleState.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxVehicleState.Location = new System.Drawing.Point(227, 203);
            this.textBoxVehicleState.Name = "textBoxVehicleState";
            this.textBoxVehicleState.ReadOnly = true;
            this.textBoxVehicleState.Size = new System.Drawing.Size(54, 20);
            this.textBoxVehicleState.TabIndex = 94;
            // 
            // textBoxInputFrom1
            // 
            this.textBoxInputFrom1.AcceptsReturn = true;
            this.textBoxInputFrom1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputFrom1.Location = new System.Drawing.Point(282, 22);
            this.textBoxInputFrom1.Name = "textBoxInputFrom1";
            this.textBoxInputFrom1.Size = new System.Drawing.Size(18, 20);
            this.textBoxInputFrom1.TabIndex = 94;
            this.textBoxInputFrom1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputFrom1_KeyDown);
            // 
            // textBoxInputFrom2
            // 
            this.textBoxInputFrom2.AcceptsReturn = true;
            this.textBoxInputFrom2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputFrom2.Location = new System.Drawing.Point(300, 22);
            this.textBoxInputFrom2.Name = "textBoxInputFrom2";
            this.textBoxInputFrom2.Size = new System.Drawing.Size(18, 20);
            this.textBoxInputFrom2.TabIndex = 94;
            this.textBoxInputFrom2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputFrom2_KeyDown);
            // 
            // textBoxInputFrom3
            // 
            this.textBoxInputFrom3.AcceptsReturn = true;
            this.textBoxInputFrom3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputFrom3.Location = new System.Drawing.Point(318, 22);
            this.textBoxInputFrom3.Name = "textBoxInputFrom3";
            this.textBoxInputFrom3.Size = new System.Drawing.Size(18, 20);
            this.textBoxInputFrom3.TabIndex = 94;
            this.textBoxInputFrom3.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputFrom3_KeyDown);
            // 
            // textBoxInputTo1
            // 
            this.textBoxInputTo1.AcceptsReturn = true;
            this.textBoxInputTo1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputTo1.Location = new System.Drawing.Point(282, 42);
            this.textBoxInputTo1.Name = "textBoxInputTo1";
            this.textBoxInputTo1.Size = new System.Drawing.Size(18, 20);
            this.textBoxInputTo1.TabIndex = 94;
            this.textBoxInputTo1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputTo1_KeyDown);
            // 
            // textBoxInputTo2
            // 
            this.textBoxInputTo2.AcceptsReturn = true;
            this.textBoxInputTo2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputTo2.Location = new System.Drawing.Point(300, 42);
            this.textBoxInputTo2.Name = "textBoxInputTo2";
            this.textBoxInputTo2.Size = new System.Drawing.Size(18, 20);
            this.textBoxInputTo2.TabIndex = 94;
            this.textBoxInputTo2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputTo2_KeyDown);
            // 
            // textBoxInputTo3
            // 
            this.textBoxInputTo3.AcceptsReturn = true;
            this.textBoxInputTo3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputTo3.Location = new System.Drawing.Point(318, 42);
            this.textBoxInputTo3.Name = "textBoxInputTo3";
            this.textBoxInputTo3.Size = new System.Drawing.Size(18, 20);
            this.textBoxInputTo3.TabIndex = 94;
            this.textBoxInputTo3.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputTo3_KeyDown);
            // 
            // textBoxInputPalletIdTransfer
            // 
            this.textBoxInputPalletIdTransfer.AcceptsReturn = true;
            this.textBoxInputPalletIdTransfer.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputPalletIdTransfer.Location = new System.Drawing.Point(282, 62);
            this.textBoxInputPalletIdTransfer.Name = "textBoxInputPalletIdTransfer";
            this.textBoxInputPalletIdTransfer.Size = new System.Drawing.Size(54, 20);
            this.textBoxInputPalletIdTransfer.TabIndex = 94;
            this.textBoxInputPalletIdTransfer.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputPalletIdTransfer_KeyDown);
            // 
            // textBoxInputLocation1
            // 
            this.textBoxInputLocation1.AcceptsReturn = true;
            this.textBoxInputLocation1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputLocation1.Location = new System.Drawing.Point(281, 82);
            this.textBoxInputLocation1.Name = "textBoxInputLocation1";
            this.textBoxInputLocation1.Size = new System.Drawing.Size(19, 20);
            this.textBoxInputLocation1.TabIndex = 94;
            this.textBoxInputLocation1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputLocation1_KeyDown);
            // 
            // textBoxInputLocation2
            // 
            this.textBoxInputLocation2.AcceptsReturn = true;
            this.textBoxInputLocation2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputLocation2.Location = new System.Drawing.Point(299, 82);
            this.textBoxInputLocation2.Name = "textBoxInputLocation2";
            this.textBoxInputLocation2.Size = new System.Drawing.Size(19, 20);
            this.textBoxInputLocation2.TabIndex = 94;
            this.textBoxInputLocation2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputLocation2_KeyDown);
            // 
            // textBoxInputLocation3
            // 
            this.textBoxInputLocation3.AcceptsReturn = true;
            this.textBoxInputLocation3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputLocation3.Location = new System.Drawing.Point(316, 82);
            this.textBoxInputLocation3.Name = "textBoxInputLocation3";
            this.textBoxInputLocation3.Size = new System.Drawing.Size(19, 20);
            this.textBoxInputLocation3.TabIndex = 94;
            this.textBoxInputLocation3.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputLocation3_KeyDown);
            // 
            // textBoxInputCompleteLocation1
            // 
            this.textBoxInputCompleteLocation1.AcceptsReturn = true;
            this.textBoxInputCompleteLocation1.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputCompleteLocation1.Location = new System.Drawing.Point(281, 102);
            this.textBoxInputCompleteLocation1.Name = "textBoxInputCompleteLocation1";
            this.textBoxInputCompleteLocation1.Size = new System.Drawing.Size(19, 20);
            this.textBoxInputCompleteLocation1.TabIndex = 94;
            this.textBoxInputCompleteLocation1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputCompleteLocation1_KeyDown);
            // 
            // textBoxInputCompleteLocation2
            // 
            this.textBoxInputCompleteLocation2.AcceptsReturn = true;
            this.textBoxInputCompleteLocation2.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputCompleteLocation2.Location = new System.Drawing.Point(299, 102);
            this.textBoxInputCompleteLocation2.Name = "textBoxInputCompleteLocation2";
            this.textBoxInputCompleteLocation2.Size = new System.Drawing.Size(19, 20);
            this.textBoxInputCompleteLocation2.TabIndex = 94;
            this.textBoxInputCompleteLocation2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputCompleteLocation2_KeyDown);
            // 
            // textBoxInputCompleteLocation3
            // 
            this.textBoxInputCompleteLocation3.AcceptsReturn = true;
            this.textBoxInputCompleteLocation3.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputCompleteLocation3.Location = new System.Drawing.Point(316, 102);
            this.textBoxInputCompleteLocation3.Name = "textBoxInputCompleteLocation3";
            this.textBoxInputCompleteLocation3.Size = new System.Drawing.Size(19, 20);
            this.textBoxInputCompleteLocation3.TabIndex = 94;
            this.textBoxInputCompleteLocation3.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputCompleteLocation3_KeyDown);
            // 
            // textBoxInputPalletIdOnVehicle
            // 
            this.textBoxInputPalletIdOnVehicle.AcceptsReturn = true;
            this.textBoxInputPalletIdOnVehicle.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputPalletIdOnVehicle.Location = new System.Drawing.Point(281, 122);
            this.textBoxInputPalletIdOnVehicle.Name = "textBoxInputPalletIdOnVehicle";
            this.textBoxInputPalletIdOnVehicle.Size = new System.Drawing.Size(54, 20);
            this.textBoxInputPalletIdOnVehicle.TabIndex = 94;
            this.textBoxInputPalletIdOnVehicle.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputPalletIdOnVehicle_KeyDown);
            // 
            // textBoxInputAlarmSetCode
            // 
            this.textBoxInputAlarmSetCode.AcceptsReturn = true;
            this.textBoxInputAlarmSetCode.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputAlarmSetCode.Location = new System.Drawing.Point(281, 142);
            this.textBoxInputAlarmSetCode.Name = "textBoxInputAlarmSetCode";
            this.textBoxInputAlarmSetCode.Size = new System.Drawing.Size(54, 20);
            this.textBoxInputAlarmSetCode.TabIndex = 94;
            this.textBoxInputAlarmSetCode.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputAlarmSetCode_KeyDown);
            // 
            // textBoxInputAlarmResetCode
            // 
            this.textBoxInputAlarmResetCode.AcceptsReturn = true;
            this.textBoxInputAlarmResetCode.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputAlarmResetCode.Location = new System.Drawing.Point(281, 162);
            this.textBoxInputAlarmResetCode.Name = "textBoxInputAlarmResetCode";
            this.textBoxInputAlarmResetCode.Size = new System.Drawing.Size(54, 20);
            this.textBoxInputAlarmResetCode.TabIndex = 94;
            this.textBoxInputAlarmResetCode.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputAlarmResetCode_KeyDown);
            // 
            // textBoxInputOperationMode
            // 
            this.textBoxInputOperationMode.AcceptsReturn = true;
            this.textBoxInputOperationMode.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputOperationMode.Location = new System.Drawing.Point(281, 182);
            this.textBoxInputOperationMode.Name = "textBoxInputOperationMode";
            this.textBoxInputOperationMode.Size = new System.Drawing.Size(54, 20);
            this.textBoxInputOperationMode.TabIndex = 94;
            this.textBoxInputOperationMode.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputOperationMode_KeyDown);
            // 
            // textBoxInputVehicleState
            // 
            this.textBoxInputVehicleState.AcceptsReturn = true;
            this.textBoxInputVehicleState.Font = new System.Drawing.Font("돋움", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBoxInputVehicleState.Location = new System.Drawing.Point(281, 203);
            this.textBoxInputVehicleState.Name = "textBoxInputVehicleState";
            this.textBoxInputVehicleState.Size = new System.Drawing.Size(54, 20);
            this.textBoxInputVehicleState.TabIndex = 94;
            this.textBoxInputVehicleState.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxInputVehicleState_KeyDown);
            // 
            // VehicleControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.BackColor = System.Drawing.Color.White;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.buttonUnloadCompleteACK);
            this.Controls.Add(this.buttonTransferRequestACK);
            this.Controls.Add(this.buttonAlarmResetReportACK);
            this.Controls.Add(this.buttonLoadCompleteACK);
            this.Controls.Add(this.buttonAlarmSetReportACK);
            this.Controls.Add(this.buttonPalletExist);
            this.Controls.Add(this.buttonLoadComplete);
            this.Controls.Add(this.buttonUnloadComplete);
            this.Controls.Add(this.buttonAlarmResetReport);
            this.Controls.Add(this.buttonTransferRequest);
            this.Controls.Add(this.buttonAlarmSetReport);
            this.Controls.Add(this.textBoxInputCompleteLocation3);
            this.Controls.Add(this.textBoxCompleteLocation3);
            this.Controls.Add(this.textBoxInputLocation3);
            this.Controls.Add(this.textBoxLocation3);
            this.Controls.Add(this.textBoxInputTo3);
            this.Controls.Add(this.textBoxTo3);
            this.Controls.Add(this.textBoxInputFrom3);
            this.Controls.Add(this.textBoxFrom3);
            this.Controls.Add(this.textBoxInputCompleteLocation2);
            this.Controls.Add(this.textBoxCompleteLocation2);
            this.Controls.Add(this.textBoxInputLocation2);
            this.Controls.Add(this.textBoxLocation2);
            this.Controls.Add(this.textBoxInputTo2);
            this.Controls.Add(this.textBoxTo2);
            this.Controls.Add(this.textBoxInputFrom2);
            this.Controls.Add(this.textBoxFrom2);
            this.Controls.Add(this.textBoxInputVehicleState);
            this.Controls.Add(this.textBoxVehicleState);
            this.Controls.Add(this.textBoxInputOperationMode);
            this.Controls.Add(this.textBoxOperationMode);
            this.Controls.Add(this.textBoxInputAlarmResetCode);
            this.Controls.Add(this.textBoxAlarmResetCode);
            this.Controls.Add(this.textBoxInputAlarmSetCode);
            this.Controls.Add(this.textBoxAlarmSetCode);
            this.Controls.Add(this.textBoxInputPalletIdOnVehicle);
            this.Controls.Add(this.textBoxPalletIdOnVehicle);
            this.Controls.Add(this.textBoxInputPalletIdTransfer);
            this.Controls.Add(this.textBoxPalletIdTransfer);
            this.Controls.Add(this.textBoxInputCompleteLocation1);
            this.Controls.Add(this.textBoxCompleteLocation1);
            this.Controls.Add(this.textBoxInputLocation1);
            this.Controls.Add(this.textBoxLocation1);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.textBoxInputTo1);
            this.Controls.Add(this.textBoxTo1);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.textBoxInputFrom1);
            this.Controls.Add(this.textBoxFrom1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.labelVehicleId);
            this.Name = "VehicleControl";
            this.Padding = new System.Windows.Forms.Padding(1);
            this.Size = new System.Drawing.Size(339, 242);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxFrom1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonAlarmResetReportACK;
        private System.Windows.Forms.Button buttonLoadCompleteACK;
        private System.Windows.Forms.Button buttonAlarmSetReportACK;
        private System.Windows.Forms.Button buttonPalletExist;
        private System.Windows.Forms.Label labelVehicleId;
        private System.Windows.Forms.Button buttonLoadComplete;
        private System.Windows.Forms.Button buttonUnloadComplete;
        private System.Windows.Forms.Button buttonAlarmResetReport;
        private System.Windows.Forms.Button buttonTransferRequest;
        private System.Windows.Forms.Button buttonAlarmSetReport;
        private System.Windows.Forms.Button buttonTransferRequestACK;
        private System.Windows.Forms.Button buttonUnloadCompleteACK;
        private System.Windows.Forms.TextBox textBoxFrom2;
        private System.Windows.Forms.TextBox textBoxFrom3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxTo1;
        private System.Windows.Forms.TextBox textBoxTo2;
        private System.Windows.Forms.TextBox textBoxTo3;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox textBoxLocation1;
        private System.Windows.Forms.TextBox textBoxLocation2;
        private System.Windows.Forms.TextBox textBoxLocation3;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBoxCompleteLocation1;
        private System.Windows.Forms.TextBox textBoxCompleteLocation2;
        private System.Windows.Forms.TextBox textBoxCompleteLocation3;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBoxPalletIdTransfer;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxPalletIdOnVehicle;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxAlarmSetCode;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxAlarmResetCode;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBoxOperationMode;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBoxVehicleState;
        private System.Windows.Forms.TextBox textBoxInputFrom1;
        private System.Windows.Forms.TextBox textBoxInputFrom2;
        private System.Windows.Forms.TextBox textBoxInputFrom3;
        private System.Windows.Forms.TextBox textBoxInputTo1;
        private System.Windows.Forms.TextBox textBoxInputTo2;
        private System.Windows.Forms.TextBox textBoxInputTo3;
        private System.Windows.Forms.TextBox textBoxInputPalletIdTransfer;
        private System.Windows.Forms.TextBox textBoxInputLocation1;
        private System.Windows.Forms.TextBox textBoxInputLocation2;
        private System.Windows.Forms.TextBox textBoxInputLocation3;
        private System.Windows.Forms.TextBox textBoxInputCompleteLocation1;
        private System.Windows.Forms.TextBox textBoxInputCompleteLocation2;
        private System.Windows.Forms.TextBox textBoxInputCompleteLocation3;
        private System.Windows.Forms.TextBox textBoxInputPalletIdOnVehicle;
        private System.Windows.Forms.TextBox textBoxInputAlarmSetCode;
        private System.Windows.Forms.TextBox textBoxInputAlarmResetCode;
        private System.Windows.Forms.TextBox textBoxInputOperationMode;
        private System.Windows.Forms.TextBox textBoxInputVehicleState;

    }
}
