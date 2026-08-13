namespace HECS.Gui.Monitor.Popups
{
    partial class LiftForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel4 = new System.Windows.Forms.Panel();
            this.buttonAbnormalComplete = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.labelAdvice = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.buttonRetry = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.label26 = new System.Windows.Forms.Label();
            this.buttonForceComplete = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.labelCommandSeq = new System.Windows.Forms.Label();
            this.labelEndLocation = new System.Windows.Forms.Label();
            this.labelCommandStatus = new System.Windows.Forms.Label();
            this.labelStartLocation = new System.Windows.Forms.Label();
            this.labelTransferPallet = new System.Windows.Forms.Label();
            this.labelMaterialCode = new System.Windows.Forms.Label();
            this.labelBatchNo = new System.Windows.Forms.Label();
            this.labelCommandId = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.buttonAlarmResetReportACK = new System.Windows.Forms.Button();
            this.buttonUnloadCompleteACK = new System.Windows.Forms.Button();
            this.buttonAlarmSetReportACK = new System.Windows.Forms.Button();
            this.buttonTransferRequestACK = new System.Windows.Forms.Button();
            this.buttonLoadCompleteACK = new System.Windows.Forms.Button();
            this.label13 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonAlarmResetReport = new System.Windows.Forms.Button();
            this.buttonPalletExist = new System.Windows.Forms.Button();
            this.buttonUnloadComplete = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonAlarmSetReport = new System.Windows.Forms.Button();
            this.buttonTransferRequest = new System.Windows.Forms.Button();
            this.buttonLoadComplete = new System.Windows.Forms.Button();
            this.labelAlarmCode = new System.Windows.Forms.Label();
            this.labelComplete03 = new System.Windows.Forms.Label();
            this.labelTo03 = new System.Windows.Forms.Label();
            this.labelComplete02 = new System.Windows.Forms.Label();
            this.labelFrom03 = new System.Windows.Forms.Label();
            this.labelTo02 = new System.Windows.Forms.Label();
            this.labelComplete01 = new System.Windows.Forms.Label();
            this.labelLocation03 = new System.Windows.Forms.Label();
            this.labelTo01 = new System.Windows.Forms.Label();
            this.labelFrom02 = new System.Windows.Forms.Label();
            this.labelFrom01 = new System.Windows.Forms.Label();
            this.labelLocation02 = new System.Windows.Forms.Label();
            this.labelLocation01 = new System.Windows.Forms.Label();
            this.labelPalletId = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.labelVehicleName = new System.Windows.Forms.Label();
            this.labelVehicleDescription = new System.Windows.Forms.Label();
            this.labelVehicleStatus = new HECS.Gui.Jaeit.Labels.LabelGradient();
            this.panel5 = new System.Windows.Forms.Panel();
            this.buttonYes = new HECS.Gui.Jaeit.Buttons.ButtonGlass();
            this.checkBoxAvailible = new System.Windows.Forms.CheckBox();
            this.panelBack.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel4.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel5.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelBack
            // 
            this.panelBack.Controls.Add(this.panel1);
            this.panelBack.Padding = new System.Windows.Forms.Padding(9, 6, 9, 8);
            this.panelBack.Size = new System.Drawing.Size(523, 377);
            this.panelBack.Text = "[Stacker Crane 정보]";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LemonChiffon;
            this.panel1.Controls.Add(this.panel4);
            this.panel1.Controls.Add(this.panel3);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Controls.Add(this.panel5);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(9, 20);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(505, 349);
            this.panel1.TabIndex = 0;
            // 
            // panel4
            // 
            this.panel4.BackColor = System.Drawing.Color.LemonChiffon;
            this.panel4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel4.Controls.Add(this.buttonAbnormalComplete);
            this.panel4.Controls.Add(this.labelAdvice);
            this.panel4.Controls.Add(this.label22);
            this.panel4.Controls.Add(this.buttonRetry);
            this.panel4.Controls.Add(this.label26);
            this.panel4.Controls.Add(this.buttonForceComplete);
            this.panel4.Controls.Add(this.label21);
            this.panel4.Controls.Add(this.label20);
            this.panel4.Controls.Add(this.label19);
            this.panel4.Controls.Add(this.labelCommandSeq);
            this.panel4.Controls.Add(this.labelEndLocation);
            this.panel4.Controls.Add(this.labelCommandStatus);
            this.panel4.Controls.Add(this.labelStartLocation);
            this.panel4.Controls.Add(this.labelTransferPallet);
            this.panel4.Controls.Add(this.labelMaterialCode);
            this.panel4.Controls.Add(this.labelBatchNo);
            this.panel4.Controls.Add(this.labelCommandId);
            this.panel4.Controls.Add(this.label25);
            this.panel4.Controls.Add(this.label18);
            this.panel4.Location = new System.Drawing.Point(3, 124);
            this.panel4.Name = "panel4";
            this.panel4.Padding = new System.Windows.Forms.Padding(10, 10, 10, 2);
            this.panel4.Size = new System.Drawing.Size(498, 161);
            this.panel4.TabIndex = 8;
            // 
            // buttonAbnormalComplete
            // 
            this.buttonAbnormalComplete.BackColor = System.Drawing.Color.Crimson;
            this.buttonAbnormalComplete.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonAbnormalComplete.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonAbnormalComplete.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAbnormalComplete.ForeColor = System.Drawing.Color.Black;
            this.buttonAbnormalComplete.GlowColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.buttonAbnormalComplete.Image = global::HECS.Properties.Resources.bullet50090600;
            this.buttonAbnormalComplete.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonAbnormalComplete.Location = new System.Drawing.Point(372, 95);
            this.buttonAbnormalComplete.Name = "buttonAbnormalComplete";
            this.buttonAbnormalComplete.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonAbnormalComplete.Size = new System.Drawing.Size(120, 44);
            this.buttonAbnormalComplete.TabIndex = 17;
            this.buttonAbnormalComplete.TabStop = false;
            this.buttonAbnormalComplete.Text = "이상종료";
            this.buttonAbnormalComplete.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonAbnormalComplete.Click += new System.EventHandler(this.buttonAbnormalComplete_Click);
            // 
            // labelAdvice
            // 
            this.labelAdvice.BackColor = System.Drawing.Color.Red;
            this.labelAdvice.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.labelAdvice.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelAdvice.ForeColor = System.Drawing.Color.White;
            this.labelAdvice.Location = new System.Drawing.Point(10, 141);
            this.labelAdvice.Name = "labelAdvice";
            this.labelAdvice.Size = new System.Drawing.Size(476, 16);
            this.labelAdvice.TabIndex = 6;
            this.labelAdvice.Text = "입/출고 상세요청번호가 DB에 없습니다.  ";
            this.labelAdvice.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.labelAdvice.Visible = false;
            // 
            // label22
            // 
            this.label22.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label22.Location = new System.Drawing.Point(8, 114);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(67, 18);
            this.label22.TabIndex = 5;
            this.label22.Text = "도착위치";
            this.label22.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonRetry
            // 
            this.buttonRetry.BackColor = System.Drawing.Color.White;
            this.buttonRetry.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonRetry.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonRetry.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonRetry.ForeColor = System.Drawing.Color.Black;
            this.buttonRetry.Image = global::HECS.Properties.Resources.bullet53463600;
            this.buttonRetry.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonRetry.Location = new System.Drawing.Point(372, 5);
            this.buttonRetry.Name = "buttonRetry";
            this.buttonRetry.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonRetry.Size = new System.Drawing.Size(120, 89);
            this.buttonRetry.TabIndex = 16;
            this.buttonRetry.TabStop = false;
            this.buttonRetry.Text = "명령 재전송";
            this.buttonRetry.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonRetry.Click += new System.EventHandler(this.buttonRetry_Click);
            // 
            // label26
            // 
            this.label26.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label26.Location = new System.Drawing.Point(8, 72);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(67, 18);
            this.label26.TabIndex = 5;
            this.label26.Text = "팔렛";
            this.label26.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.label26.Click += new System.EventHandler(this.label21_Click);
            // 
            // buttonForceComplete
            // 
            this.buttonForceComplete.BackColor = System.Drawing.Color.DodgerBlue;
            this.buttonForceComplete.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonForceComplete.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonForceComplete.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonForceComplete.ForeColor = System.Drawing.Color.Black;
            this.buttonForceComplete.GlowColor = System.Drawing.Color.White;
            this.buttonForceComplete.Image = global::HECS.Properties.Resources.bullet53512700;
            this.buttonForceComplete.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonForceComplete.Location = new System.Drawing.Point(372, 50);
            this.buttonForceComplete.Name = "buttonForceComplete";
            this.buttonForceComplete.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonForceComplete.Size = new System.Drawing.Size(120, 44);
            this.buttonForceComplete.TabIndex = 15;
            this.buttonForceComplete.TabStop = false;
            this.buttonForceComplete.Text = "완료처리";
            this.buttonForceComplete.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonForceComplete.Visible = false;
            this.buttonForceComplete.Click += new System.EventHandler(this.buttonForceComplete_Click);
            // 
            // label21
            // 
            this.label21.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label21.Location = new System.Drawing.Point(8, 93);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(67, 18);
            this.label21.TabIndex = 5;
            this.label21.Text = "출발위치";
            this.label21.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.label21.Click += new System.EventHandler(this.label21_Click);
            // 
            // label20
            // 
            this.label20.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label20.Location = new System.Drawing.Point(8, 51);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(67, 18);
            this.label20.TabIndex = 5;
            this.label20.Text = "자재코드";
            this.label20.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label19
            // 
            this.label19.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label19.Location = new System.Drawing.Point(8, 30);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(67, 18);
            this.label19.TabIndex = 5;
            this.label19.Text = "배치번호";
            this.label19.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelCommandSeq
            // 
            this.labelCommandSeq.BackColor = System.Drawing.Color.Khaki;
            this.labelCommandSeq.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelCommandSeq.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCommandSeq.Location = new System.Drawing.Point(217, 9);
            this.labelCommandSeq.Name = "labelCommandSeq";
            this.labelCommandSeq.Size = new System.Drawing.Size(138, 18);
            this.labelCommandSeq.TabIndex = 5;
            this.labelCommandSeq.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelEndLocation
            // 
            this.labelEndLocation.BackColor = System.Drawing.Color.Khaki;
            this.labelEndLocation.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelEndLocation.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelEndLocation.ForeColor = System.Drawing.Color.Blue;
            this.labelEndLocation.Location = new System.Drawing.Point(78, 114);
            this.labelEndLocation.Name = "labelEndLocation";
            this.labelEndLocation.Size = new System.Drawing.Size(277, 18);
            this.labelEndLocation.TabIndex = 5;
            this.labelEndLocation.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelCommandStatus
            // 
            this.labelCommandStatus.BackColor = System.Drawing.Color.Yellow;
            this.labelCommandStatus.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelCommandStatus.Font = new System.Drawing.Font("돋움", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelCommandStatus.ForeColor = System.Drawing.Color.Blue;
            this.labelCommandStatus.Location = new System.Drawing.Point(283, 70);
            this.labelCommandStatus.Name = "labelCommandStatus";
            this.labelCommandStatus.Size = new System.Drawing.Size(72, 20);
            this.labelCommandStatus.TabIndex = 5;
            this.labelCommandStatus.Text = "입고";
            this.labelCommandStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelStartLocation
            // 
            this.labelStartLocation.BackColor = System.Drawing.Color.Khaki;
            this.labelStartLocation.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelStartLocation.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStartLocation.ForeColor = System.Drawing.Color.Blue;
            this.labelStartLocation.Location = new System.Drawing.Point(78, 93);
            this.labelStartLocation.Name = "labelStartLocation";
            this.labelStartLocation.Size = new System.Drawing.Size(277, 18);
            this.labelStartLocation.TabIndex = 5;
            this.labelStartLocation.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelTransferPallet
            // 
            this.labelTransferPallet.BackColor = System.Drawing.Color.Khaki;
            this.labelTransferPallet.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelTransferPallet.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTransferPallet.Location = new System.Drawing.Point(78, 72);
            this.labelTransferPallet.Name = "labelTransferPallet";
            this.labelTransferPallet.Size = new System.Drawing.Size(199, 18);
            this.labelTransferPallet.TabIndex = 5;
            this.labelTransferPallet.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelMaterialCode
            // 
            this.labelMaterialCode.BackColor = System.Drawing.Color.Khaki;
            this.labelMaterialCode.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelMaterialCode.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelMaterialCode.Location = new System.Drawing.Point(78, 51);
            this.labelMaterialCode.Name = "labelMaterialCode";
            this.labelMaterialCode.Size = new System.Drawing.Size(277, 18);
            this.labelMaterialCode.TabIndex = 5;
            this.labelMaterialCode.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelBatchNo
            // 
            this.labelBatchNo.BackColor = System.Drawing.Color.Khaki;
            this.labelBatchNo.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelBatchNo.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelBatchNo.Location = new System.Drawing.Point(78, 30);
            this.labelBatchNo.Name = "labelBatchNo";
            this.labelBatchNo.Size = new System.Drawing.Size(277, 18);
            this.labelBatchNo.TabIndex = 5;
            this.labelBatchNo.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelCommandId
            // 
            this.labelCommandId.BackColor = System.Drawing.Color.Khaki;
            this.labelCommandId.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelCommandId.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCommandId.Location = new System.Drawing.Point(78, 9);
            this.labelCommandId.Name = "labelCommandId";
            this.labelCommandId.Size = new System.Drawing.Size(119, 18);
            this.labelCommandId.TabIndex = 5;
            this.labelCommandId.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label25
            // 
            this.label25.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label25.Location = new System.Drawing.Point(198, 8);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(20, 18);
            this.label25.TabIndex = 5;
            this.label25.Text = "―";
            this.label25.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label18
            // 
            this.label18.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label18.Location = new System.Drawing.Point(8, 9);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(67, 18);
            this.label18.TabIndex = 5;
            this.label18.Text = "요청번호";
            this.label18.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.buttonAlarmResetReportACK);
            this.panel3.Controls.Add(this.buttonUnloadCompleteACK);
            this.panel3.Controls.Add(this.buttonAlarmSetReportACK);
            this.panel3.Controls.Add(this.buttonTransferRequestACK);
            this.panel3.Controls.Add(this.buttonLoadCompleteACK);
            this.panel3.Controls.Add(this.label13);
            this.panel3.Controls.Add(this.label4);
            this.panel3.Controls.Add(this.buttonAlarmResetReport);
            this.panel3.Controls.Add(this.buttonPalletExist);
            this.panel3.Controls.Add(this.buttonUnloadComplete);
            this.panel3.Controls.Add(this.label12);
            this.panel3.Controls.Add(this.label8);
            this.panel3.Controls.Add(this.label2);
            this.panel3.Controls.Add(this.label11);
            this.panel3.Controls.Add(this.label7);
            this.panel3.Controls.Add(this.label3);
            this.panel3.Controls.Add(this.buttonAlarmSetReport);
            this.panel3.Controls.Add(this.buttonTransferRequest);
            this.panel3.Controls.Add(this.buttonLoadComplete);
            this.panel3.Controls.Add(this.labelAlarmCode);
            this.panel3.Controls.Add(this.labelComplete03);
            this.panel3.Controls.Add(this.labelTo03);
            this.panel3.Controls.Add(this.labelComplete02);
            this.panel3.Controls.Add(this.labelFrom03);
            this.panel3.Controls.Add(this.labelTo02);
            this.panel3.Controls.Add(this.labelComplete01);
            this.panel3.Controls.Add(this.labelLocation03);
            this.panel3.Controls.Add(this.labelTo01);
            this.panel3.Controls.Add(this.labelFrom02);
            this.panel3.Controls.Add(this.labelFrom01);
            this.panel3.Controls.Add(this.labelLocation02);
            this.panel3.Controls.Add(this.labelLocation01);
            this.panel3.Controls.Add(this.labelPalletId);
            this.panel3.Controls.Add(this.label14);
            this.panel3.Controls.Add(this.label5);
            this.panel3.Controls.Add(this.label17);
            this.panel3.Controls.Add(this.label16);
            this.panel3.Controls.Add(this.label15);
            this.panel3.Controls.Add(this.label9);
            this.panel3.Controls.Add(this.label10);
            this.panel3.Controls.Add(this.label6);
            this.panel3.Controls.Add(this.label1);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel3.Location = new System.Drawing.Point(0, 46);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(505, 80);
            this.panel3.TabIndex = 7;
            // 
            // buttonAlarmResetReportACK
            // 
            this.buttonAlarmResetReportACK.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonAlarmResetReportACK.Enabled = false;
            this.buttonAlarmResetReportACK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonAlarmResetReportACK.Location = new System.Drawing.Point(383, 39);
            this.buttonAlarmResetReportACK.Name = "buttonAlarmResetReportACK";
            this.buttonAlarmResetReportACK.Size = new System.Drawing.Size(14, 13);
            this.buttonAlarmResetReportACK.TabIndex = 8;
            this.buttonAlarmResetReportACK.UseVisualStyleBackColor = false;
            // 
            // buttonUnloadCompleteACK
            // 
            this.buttonUnloadCompleteACK.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonUnloadCompleteACK.Enabled = false;
            this.buttonUnloadCompleteACK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonUnloadCompleteACK.Location = new System.Drawing.Point(383, 7);
            this.buttonUnloadCompleteACK.Name = "buttonUnloadCompleteACK";
            this.buttonUnloadCompleteACK.Size = new System.Drawing.Size(14, 13);
            this.buttonUnloadCompleteACK.TabIndex = 8;
            this.buttonUnloadCompleteACK.UseVisualStyleBackColor = false;
            // 
            // buttonAlarmSetReportACK
            // 
            this.buttonAlarmSetReportACK.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonAlarmSetReportACK.Enabled = false;
            this.buttonAlarmSetReportACK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonAlarmSetReportACK.Location = new System.Drawing.Point(131, 38);
            this.buttonAlarmSetReportACK.Name = "buttonAlarmSetReportACK";
            this.buttonAlarmSetReportACK.Size = new System.Drawing.Size(14, 13);
            this.buttonAlarmSetReportACK.TabIndex = 8;
            this.buttonAlarmSetReportACK.UseVisualStyleBackColor = false;
            // 
            // buttonTransferRequestACK
            // 
            this.buttonTransferRequestACK.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonTransferRequestACK.Enabled = false;
            this.buttonTransferRequestACK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonTransferRequestACK.Location = new System.Drawing.Point(131, 22);
            this.buttonTransferRequestACK.Name = "buttonTransferRequestACK";
            this.buttonTransferRequestACK.Size = new System.Drawing.Size(14, 13);
            this.buttonTransferRequestACK.TabIndex = 8;
            this.buttonTransferRequestACK.UseVisualStyleBackColor = false;
            // 
            // buttonLoadCompleteACK
            // 
            this.buttonLoadCompleteACK.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonLoadCompleteACK.Enabled = false;
            this.buttonLoadCompleteACK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonLoadCompleteACK.Location = new System.Drawing.Point(131, 6);
            this.buttonLoadCompleteACK.Name = "buttonLoadCompleteACK";
            this.buttonLoadCompleteACK.Size = new System.Drawing.Size(14, 13);
            this.buttonLoadCompleteACK.TabIndex = 8;
            this.buttonLoadCompleteACK.UseVisualStyleBackColor = false;
            // 
            // label13
            // 
            this.label13.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(399, 38);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(103, 16);
            this.label13.TabIndex = 7;
            this.label13.Text = "Alarm Reset ACK";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(399, 6);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(103, 16);
            this.label4.TabIndex = 7;
            this.label4.Text = "Unload Complete ACK";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // buttonAlarmResetReport
            // 
            this.buttonAlarmResetReport.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonAlarmResetReport.Enabled = false;
            this.buttonAlarmResetReport.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonAlarmResetReport.Location = new System.Drawing.Point(257, 39);
            this.buttonAlarmResetReport.Name = "buttonAlarmResetReport";
            this.buttonAlarmResetReport.Size = new System.Drawing.Size(14, 13);
            this.buttonAlarmResetReport.TabIndex = 6;
            this.buttonAlarmResetReport.UseVisualStyleBackColor = false;
            // 
            // buttonPalletExist
            // 
            this.buttonPalletExist.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonPalletExist.Enabled = false;
            this.buttonPalletExist.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonPalletExist.Location = new System.Drawing.Point(257, 23);
            this.buttonPalletExist.Name = "buttonPalletExist";
            this.buttonPalletExist.Size = new System.Drawing.Size(14, 13);
            this.buttonPalletExist.TabIndex = 6;
            this.buttonPalletExist.UseVisualStyleBackColor = false;
            // 
            // buttonUnloadComplete
            // 
            this.buttonUnloadComplete.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonUnloadComplete.Enabled = false;
            this.buttonUnloadComplete.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonUnloadComplete.Location = new System.Drawing.Point(257, 7);
            this.buttonUnloadComplete.Name = "buttonUnloadComplete";
            this.buttonUnloadComplete.Size = new System.Drawing.Size(14, 13);
            this.buttonUnloadComplete.TabIndex = 6;
            this.buttonUnloadComplete.UseVisualStyleBackColor = false;
            // 
            // label12
            // 
            this.label12.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(147, 37);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(103, 16);
            this.label12.TabIndex = 7;
            this.label12.Text = "Alarm Set ACK";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label8
            // 
            this.label8.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(147, 21);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(103, 16);
            this.label8.TabIndex = 7;
            this.label8.Text = "Transfer Request ACK";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(147, 5);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(103, 16);
            this.label2.TabIndex = 7;
            this.label2.Text = "Load Complete ACK";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label11
            // 
            this.label11.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(273, 38);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(103, 16);
            this.label11.TabIndex = 5;
            this.label11.Text = "Alarm Reset";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label7
            // 
            this.label7.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(273, 22);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(103, 16);
            this.label7.TabIndex = 5;
            this.label7.Text = "Pallet Exist";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(273, 6);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(103, 16);
            this.label3.TabIndex = 5;
            this.label3.Text = "Unload Complete";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // buttonAlarmSetReport
            // 
            this.buttonAlarmSetReport.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonAlarmSetReport.Enabled = false;
            this.buttonAlarmSetReport.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonAlarmSetReport.Location = new System.Drawing.Point(5, 38);
            this.buttonAlarmSetReport.Name = "buttonAlarmSetReport";
            this.buttonAlarmSetReport.Size = new System.Drawing.Size(14, 13);
            this.buttonAlarmSetReport.TabIndex = 6;
            this.buttonAlarmSetReport.UseVisualStyleBackColor = false;
            // 
            // buttonTransferRequest
            // 
            this.buttonTransferRequest.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonTransferRequest.Enabled = false;
            this.buttonTransferRequest.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonTransferRequest.Location = new System.Drawing.Point(5, 22);
            this.buttonTransferRequest.Name = "buttonTransferRequest";
            this.buttonTransferRequest.Size = new System.Drawing.Size(14, 13);
            this.buttonTransferRequest.TabIndex = 6;
            this.buttonTransferRequest.UseVisualStyleBackColor = false;
            // 
            // buttonLoadComplete
            // 
            this.buttonLoadComplete.BackColor = System.Drawing.Color.DarkGreen;
            this.buttonLoadComplete.Enabled = false;
            this.buttonLoadComplete.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonLoadComplete.Location = new System.Drawing.Point(5, 6);
            this.buttonLoadComplete.Name = "buttonLoadComplete";
            this.buttonLoadComplete.Size = new System.Drawing.Size(14, 13);
            this.buttonLoadComplete.TabIndex = 6;
            this.buttonLoadComplete.UseVisualStyleBackColor = false;
            // 
            // labelAlarmCode
            // 
            this.labelAlarmCode.BackColor = System.Drawing.Color.MistyRose;
            this.labelAlarmCode.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelAlarmCode.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelAlarmCode.ForeColor = System.Drawing.Color.Red;
            this.labelAlarmCode.Location = new System.Drawing.Point(462, 59);
            this.labelAlarmCode.Name = "labelAlarmCode";
            this.labelAlarmCode.Size = new System.Drawing.Size(17, 14);
            this.labelAlarmCode.TabIndex = 5;
            this.labelAlarmCode.Text = "00";
            this.labelAlarmCode.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelComplete03
            // 
            this.labelComplete03.BackColor = System.Drawing.Color.Khaki;
            this.labelComplete03.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelComplete03.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelComplete03.Location = new System.Drawing.Point(387, 58);
            this.labelComplete03.Name = "labelComplete03";
            this.labelComplete03.Size = new System.Drawing.Size(17, 14);
            this.labelComplete03.TabIndex = 5;
            this.labelComplete03.Text = "00";
            this.labelComplete03.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelTo03
            // 
            this.labelTo03.BackColor = System.Drawing.Color.Khaki;
            this.labelTo03.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelTo03.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTo03.Location = new System.Drawing.Point(278, 58);
            this.labelTo03.Name = "labelTo03";
            this.labelTo03.Size = new System.Drawing.Size(17, 14);
            this.labelTo03.TabIndex = 5;
            this.labelTo03.Text = "00";
            this.labelTo03.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelComplete02
            // 
            this.labelComplete02.BackColor = System.Drawing.Color.Khaki;
            this.labelComplete02.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelComplete02.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelComplete02.Location = new System.Drawing.Point(369, 58);
            this.labelComplete02.Name = "labelComplete02";
            this.labelComplete02.Size = new System.Drawing.Size(17, 14);
            this.labelComplete02.TabIndex = 5;
            this.labelComplete02.Text = "00";
            this.labelComplete02.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelFrom03
            // 
            this.labelFrom03.BackColor = System.Drawing.Color.Khaki;
            this.labelFrom03.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelFrom03.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelFrom03.Location = new System.Drawing.Point(179, 58);
            this.labelFrom03.Name = "labelFrom03";
            this.labelFrom03.Size = new System.Drawing.Size(17, 14);
            this.labelFrom03.TabIndex = 5;
            this.labelFrom03.Text = "00";
            this.labelFrom03.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelTo02
            // 
            this.labelTo02.BackColor = System.Drawing.Color.Khaki;
            this.labelTo02.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelTo02.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTo02.Location = new System.Drawing.Point(260, 58);
            this.labelTo02.Name = "labelTo02";
            this.labelTo02.Size = new System.Drawing.Size(17, 14);
            this.labelTo02.TabIndex = 5;
            this.labelTo02.Text = "00";
            this.labelTo02.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelComplete01
            // 
            this.labelComplete01.BackColor = System.Drawing.Color.Khaki;
            this.labelComplete01.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelComplete01.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelComplete01.Location = new System.Drawing.Point(351, 58);
            this.labelComplete01.Name = "labelComplete01";
            this.labelComplete01.Size = new System.Drawing.Size(17, 14);
            this.labelComplete01.TabIndex = 5;
            this.labelComplete01.Text = "00";
            this.labelComplete01.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelLocation03
            // 
            this.labelLocation03.BackColor = System.Drawing.Color.Khaki;
            this.labelLocation03.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelLocation03.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelLocation03.Location = new System.Drawing.Point(84, 58);
            this.labelLocation03.Name = "labelLocation03";
            this.labelLocation03.Size = new System.Drawing.Size(17, 14);
            this.labelLocation03.TabIndex = 5;
            this.labelLocation03.Text = "00";
            this.labelLocation03.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelTo01
            // 
            this.labelTo01.BackColor = System.Drawing.Color.Khaki;
            this.labelTo01.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelTo01.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTo01.Location = new System.Drawing.Point(242, 58);
            this.labelTo01.Name = "labelTo01";
            this.labelTo01.Size = new System.Drawing.Size(17, 14);
            this.labelTo01.TabIndex = 5;
            this.labelTo01.Text = "00";
            this.labelTo01.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelFrom02
            // 
            this.labelFrom02.BackColor = System.Drawing.Color.Khaki;
            this.labelFrom02.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelFrom02.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelFrom02.Location = new System.Drawing.Point(161, 58);
            this.labelFrom02.Name = "labelFrom02";
            this.labelFrom02.Size = new System.Drawing.Size(17, 14);
            this.labelFrom02.TabIndex = 5;
            this.labelFrom02.Text = "00";
            this.labelFrom02.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelFrom01
            // 
            this.labelFrom01.BackColor = System.Drawing.Color.Khaki;
            this.labelFrom01.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelFrom01.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelFrom01.Location = new System.Drawing.Point(143, 58);
            this.labelFrom01.Name = "labelFrom01";
            this.labelFrom01.Size = new System.Drawing.Size(17, 14);
            this.labelFrom01.TabIndex = 5;
            this.labelFrom01.Text = "00";
            this.labelFrom01.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelLocation02
            // 
            this.labelLocation02.BackColor = System.Drawing.Color.Khaki;
            this.labelLocation02.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelLocation02.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelLocation02.Location = new System.Drawing.Point(66, 58);
            this.labelLocation02.Name = "labelLocation02";
            this.labelLocation02.Size = new System.Drawing.Size(17, 14);
            this.labelLocation02.TabIndex = 5;
            this.labelLocation02.Text = "00";
            this.labelLocation02.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelLocation01
            // 
            this.labelLocation01.BackColor = System.Drawing.Color.Khaki;
            this.labelLocation01.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelLocation01.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelLocation01.Location = new System.Drawing.Point(48, 58);
            this.labelLocation01.Name = "labelLocation01";
            this.labelLocation01.Size = new System.Drawing.Size(17, 14);
            this.labelLocation01.TabIndex = 5;
            this.labelLocation01.Text = "00";
            this.labelLocation01.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelPalletId
            // 
            this.labelPalletId.BackColor = System.Drawing.Color.Khaki;
            this.labelPalletId.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelPalletId.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelPalletId.Location = new System.Drawing.Point(425, 23);
            this.labelPalletId.Name = "labelPalletId";
            this.labelPalletId.Size = new System.Drawing.Size(75, 16);
            this.labelPalletId.TabIndex = 5;
            this.labelPalletId.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label14
            // 
            this.label14.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(383, 23);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(55, 16);
            this.label14.TabIndex = 5;
            this.label14.Text = "Pallet ID";
            this.label14.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(422, 58);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(49, 16);
            this.label5.TabIndex = 5;
            this.label5.Text = "알람코드";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label17
            // 
            this.label17.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.Location = new System.Drawing.Point(311, 57);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(49, 16);
            this.label17.TabIndex = 5;
            this.label17.Text = "완료위치";
            this.label17.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label16
            // 
            this.label16.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label16.Location = new System.Drawing.Point(210, 57);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(40, 16);
            this.label16.TabIndex = 5;
            this.label16.Text = "도착지";
            this.label16.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label15
            // 
            this.label15.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(112, 57);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(40, 16);
            this.label15.TabIndex = 5;
            this.label15.Text = "출발지";
            this.label15.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label9
            // 
            this.label9.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(7, 57);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(55, 16);
            this.label9.TabIndex = 5;
            this.label9.Text = "현재위치";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label10
            // 
            this.label10.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(21, 37);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(103, 16);
            this.label10.TabIndex = 5;
            this.label10.Text = "Alarm Set";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(21, 21);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(103, 16);
            this.label6.TabIndex = 5;
            this.label6.Text = "Transfer Request";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Arial", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(21, 5);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(103, 16);
            this.label1.TabIndex = 5;
            this.label1.Text = "Load Complete";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Khaki;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.labelVehicleName);
            this.panel2.Controls.Add(this.labelVehicleDescription);
            this.panel2.Controls.Add(this.labelVehicleStatus);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(4);
            this.panel2.Size = new System.Drawing.Size(505, 46);
            this.panel2.TabIndex = 0;
            // 
            // labelVehicleName
            // 
            this.labelVehicleName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelVehicleName.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelVehicleName.ForeColor = System.Drawing.Color.Blue;
            this.labelVehicleName.Location = new System.Drawing.Point(61, 4);
            this.labelVehicleName.Name = "labelVehicleName";
            this.labelVehicleName.Size = new System.Drawing.Size(438, 19);
            this.labelVehicleName.TabIndex = 4;
            this.labelVehicleName.Text = "ConveyorSEM1";
            this.labelVehicleName.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            // 
            // labelVehicleDescription
            // 
            this.labelVehicleDescription.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.labelVehicleDescription.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelVehicleDescription.ForeColor = System.Drawing.Color.Blue;
            this.labelVehicleDescription.Location = new System.Drawing.Point(61, 23);
            this.labelVehicleDescription.Name = "labelVehicleDescription";
            this.labelVehicleDescription.Size = new System.Drawing.Size(438, 17);
            this.labelVehicleDescription.TabIndex = 3;
            this.labelVehicleDescription.Text = "(출고대)";
            this.labelVehicleDescription.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelVehicleStatus
            // 
            this.labelVehicleStatus.Dock = System.Windows.Forms.DockStyle.Left;
            this.labelVehicleStatus.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.labelVehicleStatus.ForeColor = System.Drawing.Color.DarkGray;
            this.labelVehicleStatus.GradientColorOne = System.Drawing.Color.Red;
            this.labelVehicleStatus.GradientColorTwo = System.Drawing.Color.Red;
            this.labelVehicleStatus.Location = new System.Drawing.Point(4, 4);
            this.labelVehicleStatus.Name = "labelVehicleStatus";
            this.labelVehicleStatus.Size = new System.Drawing.Size(57, 36);
            this.labelVehicleStatus.TabIndex = 1;
            this.labelVehicleStatus.Text = "DOWN";
            this.labelVehicleStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.labelVehicleStatus.UseCompatibleTextRendering = true;
            // 
            // panel5
            // 
            this.panel5.AutoScroll = true;
            this.panel5.Controls.Add(this.checkBoxAvailible);
            this.panel5.Controls.Add(this.buttonYes);
            this.panel5.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel5.Location = new System.Drawing.Point(0, 288);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(505, 61);
            this.panel5.TabIndex = 6;
            // 
            // buttonYes
            // 
            this.buttonYes.BackColor = System.Drawing.Color.Teal;
            this.buttonYes.Cursor = System.Windows.Forms.Cursors.Hand;
            this.buttonYes.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonYes.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonYes.ForeColor = System.Drawing.Color.Black;
            this.buttonYes.Image = global::HECS.Properties.Resources.OK4;
            this.buttonYes.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonYes.Location = new System.Drawing.Point(209, 4);
            this.buttonYes.Name = "buttonYes";
            this.buttonYes.OuterBorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.buttonYes.Size = new System.Drawing.Size(86, 52);
            this.buttonYes.TabIndex = 13;
            this.buttonYes.TabStop = false;
            this.buttonYes.Text = "확인";
            this.buttonYes.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.buttonYes.Click += new System.EventHandler(this.buttonYes_Click);
            // 
            // checkBoxAvailible
            // 
            this.checkBoxAvailible.AutoSize = true;
            this.checkBoxAvailible.Location = new System.Drawing.Point(9, 24);
            this.checkBoxAvailible.Name = "checkBoxAvailible";
            this.checkBoxAvailible.Size = new System.Drawing.Size(76, 16);
            this.checkBoxAvailible.TabIndex = 14;
            this.checkBoxAvailible.Text = "사용금지";
            this.checkBoxAvailible.UseVisualStyleBackColor = true;
            this.checkBoxAvailible.CheckedChanged += new System.EventHandler(this.checkBoxAvailible_CheckedChanged);
            // 
            // LiftForm
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(523, 377);
            this.Name = "LiftForm";
            this.Text = "[Conveyor 정보]";
            this.panelBack.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel4.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private HECS.Gui.Jaeit.Labels.LabelGradient labelVehicleStatus;
        private System.Windows.Forms.Label labelVehicleName;
        private System.Windows.Forms.Label labelVehicleDescription;
        private System.Windows.Forms.Panel panel5;
        private HECS.Gui.Jaeit.Buttons.ButtonGlass buttonYes;
        private System.Windows.Forms.Panel panel4;
        private HECS.Gui.Jaeit.Buttons.ButtonGlass buttonAbnormalComplete;
        private HECS.Gui.Jaeit.Buttons.ButtonGlass buttonRetry;
        private HECS.Gui.Jaeit.Buttons.ButtonGlass buttonForceComplete;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Button buttonAlarmResetReportACK;
        private System.Windows.Forms.Button buttonUnloadCompleteACK;
        private System.Windows.Forms.Button buttonAlarmSetReportACK;
        private System.Windows.Forms.Button buttonTransferRequestACK;
        private System.Windows.Forms.Button buttonLoadCompleteACK;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button buttonAlarmResetReport;
        private System.Windows.Forms.Button buttonPalletExist;
        private System.Windows.Forms.Button buttonUnloadComplete;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonAlarmSetReport;
        private System.Windows.Forms.Button buttonTransferRequest;
        private System.Windows.Forms.Button buttonLoadComplete;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelComplete03;
        private System.Windows.Forms.Label labelTo03;
        private System.Windows.Forms.Label labelComplete02;
        private System.Windows.Forms.Label labelFrom03;
        private System.Windows.Forms.Label labelTo02;
        private System.Windows.Forms.Label labelComplete01;
        private System.Windows.Forms.Label labelLocation03;
        private System.Windows.Forms.Label labelTo01;
        private System.Windows.Forms.Label labelFrom02;
        private System.Windows.Forms.Label labelFrom01;
        private System.Windows.Forms.Label labelLocation02;
        private System.Windows.Forms.Label labelLocation01;
        private System.Windows.Forms.Label labelPalletId;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label labelAdvice;
        private System.Windows.Forms.Label labelAlarmCode;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label labelCommandSeq;
        private System.Windows.Forms.Label labelCommandId;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label labelEndLocation;
        private System.Windows.Forms.Label labelStartLocation;
        private System.Windows.Forms.Label labelTransferPallet;
        private System.Windows.Forms.Label labelMaterialCode;
        private System.Windows.Forms.Label labelBatchNo;
        private System.Windows.Forms.Label labelCommandStatus;
        private System.Windows.Forms.CheckBox checkBoxAvailible;
    }
}