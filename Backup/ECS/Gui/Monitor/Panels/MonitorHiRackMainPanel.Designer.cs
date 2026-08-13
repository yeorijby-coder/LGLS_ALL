namespace HECS.Gui.Monitor.Panels
{
    partial class MonitorHiRackMainPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MonitorHiRackMainPanel));
            this.panel1 = new System.Windows.Forms.Panel();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.hiRackDetailPanel1 = new HECS.Gui.Monitor.Panels.Monitoring.HiRackDetailPanel();
            this.hiRackCellInform1 = new HECS.Gui.Monitor.Panels.Monitoring.HiRackCellInform();
            this.buttonXP2 = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.buttonXP3 = new HECS.Gui.Jaeit.Buttons.ButtonXP();
            this.panel1.SuspendLayout();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightSteelBlue;
            this.panel1.Controls.Add(this.buttonXP2);
            this.panel1.Controls.Add(this.buttonXP3);
            this.panel1.Controls.Add(this.radioButton4);
            this.panel1.Controls.Add(this.radioButton3);
            this.panel1.Controls.Add(this.radioButton2);
            this.panel1.Controls.Add(this.radioButton1);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(909, 40);
            this.panel1.TabIndex = 0;
            // 
            // radioButton4
            // 
            this.radioButton4.Dock = System.Windows.Forms.DockStyle.Left;
            this.radioButton4.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radioButton4.ForeColor = System.Drawing.Color.Blue;
            this.radioButton4.Location = new System.Drawing.Point(213, 0);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(48, 40);
            this.radioButton4.TabIndex = 8;
            this.radioButton4.TabStop = true;
            this.radioButton4.Text = "#4";
            this.radioButton4.UseVisualStyleBackColor = true;
            // 
            // radioButton3
            // 
            this.radioButton3.Dock = System.Windows.Forms.DockStyle.Left;
            this.radioButton3.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radioButton3.ForeColor = System.Drawing.Color.Blue;
            this.radioButton3.Location = new System.Drawing.Point(165, 0);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(48, 40);
            this.radioButton3.TabIndex = 7;
            this.radioButton3.TabStop = true;
            this.radioButton3.Text = "#3";
            this.radioButton3.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            this.radioButton2.Dock = System.Windows.Forms.DockStyle.Left;
            this.radioButton2.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radioButton2.ForeColor = System.Drawing.Color.Blue;
            this.radioButton2.Location = new System.Drawing.Point(117, 0);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(48, 40);
            this.radioButton2.TabIndex = 6;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "#2";
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // radioButton1
            // 
            this.radioButton1.Dock = System.Windows.Forms.DockStyle.Left;
            this.radioButton1.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radioButton1.ForeColor = System.Drawing.Color.Blue;
            this.radioButton1.Location = new System.Drawing.Point(71, 0);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(46, 40);
            this.radioButton1.TabIndex = 5;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "#1";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Left;
            this.label1.Font = new System.Drawing.Font("Arial", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 40);
            this.label1.TabIndex = 0;
            this.label1.Text = "Hi-Rack";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // splitContainer
            // 
            this.splitContainer.BackColor = System.Drawing.Color.RoyalBlue;
            this.splitContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 40);
            this.splitContainer.Name = "splitContainer";
            this.splitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.BackColor = System.Drawing.Color.LemonChiffon;
            this.splitContainer.Panel1.Controls.Add(this.hiRackDetailPanel1);
            this.splitContainer.Panel1.Controls.Add(this.trackBar1);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.hiRackCellInform1);
            this.splitContainer.Size = new System.Drawing.Size(909, 645);
            this.splitContainer.SplitterDistance = 435;
            this.splitContainer.SplitterWidth = 3;
            this.splitContainer.TabIndex = 1;
            // 
            // trackBar1
            // 
            this.trackBar1.Dock = System.Windows.Forms.DockStyle.Right;
            this.trackBar1.Location = new System.Drawing.Point(863, 0);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar1.Size = new System.Drawing.Size(42, 431);
            this.trackBar1.TabIndex = 0;
            this.trackBar1.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.trackBar1.Value = 5;
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.RoyalBlue;
            this.label2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label2.Location = new System.Drawing.Point(0, 681);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(909, 4);
            this.label2.TabIndex = 2;
            // 
            // hiRackDetailPanel1
            // 
            this.hiRackDetailPanel1.AutoScroll = true;
            this.hiRackDetailPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.hiRackDetailPanel1.Location = new System.Drawing.Point(0, 0);
            this.hiRackDetailPanel1.Name = "hiRackDetailPanel1";
            this.hiRackDetailPanel1.Size = new System.Drawing.Size(863, 431);
            this.hiRackDetailPanel1.TabIndex = 1;
            // 
            // hiRackCellInform1
            // 
            this.hiRackCellInform1.AutoScroll = true;
            this.hiRackCellInform1.BackColor = System.Drawing.Color.LemonChiffon;
            this.hiRackCellInform1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.hiRackCellInform1.Location = new System.Drawing.Point(0, 0);
            this.hiRackCellInform1.Name = "hiRackCellInform1";
            this.hiRackCellInform1.Size = new System.Drawing.Size(905, 203);
            this.hiRackCellInform1.TabIndex = 0;
            // 
            // buttonXP2
            // 
            this.buttonXP2.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXP2.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXP2.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXP2.Dock = System.Windows.Forms.DockStyle.Left;
            this.buttonXP2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXP2.Image = global::HECS.Properties.Resources.bullet50043600;
            this.buttonXP2.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXP2.Location = new System.Drawing.Point(342, 0);
            this.buttonXP2.Name = "buttonXP2";
            this.buttonXP2.Size = new System.Drawing.Size(81, 40);
            this.buttonXP2.TabIndex = 3;
            this.buttonXP2.Text = "Excel";
            this.buttonXP2.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXP2.UseVisualStyleBackColor = true;
            // 
            // buttonXP3
            // 
            this.buttonXP3.AdjustImageLocation = new System.Drawing.Point(0, 0);
            this.buttonXP3.BtnShape = HECS.Gui.Jaeit.Buttons.emunType.BtnShape.Rectangle;
            this.buttonXP3.BtnStyle = HECS.Gui.Jaeit.Buttons.emunType.XPStyle.Default;
            this.buttonXP3.Dock = System.Windows.Forms.DockStyle.Left;
            this.buttonXP3.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonXP3.Image = ((System.Drawing.Image)(resources.GetObject("buttonXP3.Image")));
            this.buttonXP3.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonXP3.Location = new System.Drawing.Point(261, 0);
            this.buttonXP3.Name = "buttonXP3";
            this.buttonXP3.Size = new System.Drawing.Size(81, 40);
            this.buttonXP3.TabIndex = 4;
            this.buttonXP3.Text = "조회";
            this.buttonXP3.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.buttonXP3.UseVisualStyleBackColor = true;
            // 
            // MonitorHiRackMainPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Blue;
            this.Controls.Add(this.label2);
            this.Controls.Add(this.splitContainer);
            this.Controls.Add(this.panel1);
            this.Name = "MonitorHiRackMainPanel";
            this.Size = new System.Drawing.Size(909, 685);
            this.panel1.ResumeLayout(false);
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel1.PerformLayout();
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXP3;
        private HECS.Gui.Jaeit.Buttons.ButtonXP buttonXP2;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.SplitContainer splitContainer;
        private HECS.Gui.Monitor.Panels.Monitoring.HiRackCellInform hiRackCellInform1;
        private System.Windows.Forms.TrackBar trackBar1;
        private HECS.Gui.Monitor.Panels.Monitoring.HiRackDetailPanel hiRackDetailPanel1;
        private System.Windows.Forms.Label label2;

    }
}
