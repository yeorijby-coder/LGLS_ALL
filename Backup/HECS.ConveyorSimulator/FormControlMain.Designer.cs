namespace HECS.ConveyorSimulator
{
    partial class FormControlMain
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
            this.panel3 = new System.Windows.Forms.Panel();
            this.buttonAlarmTest = new System.Windows.Forms.Button();
            this.buttonSpeedFast = new System.Windows.Forms.Button();
            this.buttonSpeedNormal = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.buttonSpeedSlow = new System.Windows.Forms.Button();
            this.buttonInitialize = new System.Windows.Forms.Button();
            this.buttonCycleTest = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.label2 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.panel1.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.panel3);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(1);
            this.panel1.Size = new System.Drawing.Size(287, 601);
            this.panel1.TabIndex = 4;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.buttonAlarmTest);
            this.panel3.Controls.Add(this.buttonSpeedFast);
            this.panel3.Controls.Add(this.buttonSpeedNormal);
            this.panel3.Controls.Add(this.button1);
            this.panel3.Controls.Add(this.buttonSpeedSlow);
            this.panel3.Controls.Add(this.buttonInitialize);
            this.panel3.Controls.Add(this.buttonCycleTest);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(1, 23);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(281, 573);
            this.panel3.TabIndex = 5;
            // 
            // buttonAlarmTest
            // 
            this.buttonAlarmTest.BackColor = System.Drawing.Color.Crimson;
            this.buttonAlarmTest.Location = new System.Drawing.Point(95, 3);
            this.buttonAlarmTest.Name = "buttonAlarmTest";
            this.buttonAlarmTest.Size = new System.Drawing.Size(90, 30);
            this.buttonAlarmTest.TabIndex = 4;
            this.buttonAlarmTest.Text = "알람 테스트";
            this.buttonAlarmTest.UseVisualStyleBackColor = false;
            // 
            // buttonSpeedFast
            // 
            this.buttonSpeedFast.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(128)))));
            this.buttonSpeedFast.Location = new System.Drawing.Point(187, 35);
            this.buttonSpeedFast.Name = "buttonSpeedFast";
            this.buttonSpeedFast.Size = new System.Drawing.Size(90, 30);
            this.buttonSpeedFast.TabIndex = 4;
            this.buttonSpeedFast.Text = "빠르게";
            this.buttonSpeedFast.UseVisualStyleBackColor = false;
            // 
            // buttonSpeedNormal
            // 
            this.buttonSpeedNormal.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(128)))));
            this.buttonSpeedNormal.Location = new System.Drawing.Point(95, 35);
            this.buttonSpeedNormal.Name = "buttonSpeedNormal";
            this.buttonSpeedNormal.Size = new System.Drawing.Size(90, 30);
            this.buttonSpeedNormal.TabIndex = 4;
            this.buttonSpeedNormal.Text = "기본속도(x1)";
            this.buttonSpeedNormal.UseVisualStyleBackColor = false;
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.DarkKhaki;
            this.button1.Location = new System.Drawing.Point(3, 66);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(90, 30);
            this.button1.TabIndex = 4;
            this.button1.Text = "AGVC 연결";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonSpeedSlow
            // 
            this.buttonSpeedSlow.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(128)))));
            this.buttonSpeedSlow.Location = new System.Drawing.Point(3, 35);
            this.buttonSpeedSlow.Name = "buttonSpeedSlow";
            this.buttonSpeedSlow.Size = new System.Drawing.Size(90, 30);
            this.buttonSpeedSlow.TabIndex = 4;
            this.buttonSpeedSlow.Text = "느리게";
            this.buttonSpeedSlow.UseVisualStyleBackColor = false;
            // 
            // buttonInitialize
            // 
            this.buttonInitialize.BackColor = System.Drawing.Color.Chartreuse;
            this.buttonInitialize.Location = new System.Drawing.Point(3, 3);
            this.buttonInitialize.Name = "buttonInitialize";
            this.buttonInitialize.Size = new System.Drawing.Size(90, 30);
            this.buttonInitialize.TabIndex = 4;
            this.buttonInitialize.Text = "초기화";
            this.buttonInitialize.UseVisualStyleBackColor = false;
            this.buttonInitialize.Click += new System.EventHandler(this.buttonInitialize_Click_1);
            // 
            // buttonCycleTest
            // 
            this.buttonCycleTest.BackColor = System.Drawing.Color.LimeGreen;
            this.buttonCycleTest.Location = new System.Drawing.Point(187, 3);
            this.buttonCycleTest.Name = "buttonCycleTest";
            this.buttonCycleTest.Size = new System.Drawing.Size(90, 30);
            this.buttonCycleTest.TabIndex = 4;
            this.buttonCycleTest.Text = "사이클 테스트";
            this.buttonCycleTest.UseVisualStyleBackColor = false;
            this.buttonCycleTest.Click += new System.EventHandler(this.buttonCycleTest_Click);
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Font = new System.Drawing.Font("돋움체", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.Location = new System.Drawing.Point(1, 1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(281, 22);
            this.label1.TabIndex = 4;
            this.label1.Text = "전역설정 및 제어";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2.Controls.Add(this.tabControl);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(287, 0);
            this.panel2.Name = "panel2";
            this.panel2.Padding = new System.Windows.Forms.Padding(1);
            this.panel2.Size = new System.Drawing.Size(538, 601);
            this.panel2.TabIndex = 5;
            // 
            // tabControl
            // 
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(1, 23);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(532, 573);
            this.tabControl.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Font = new System.Drawing.Font("돋움체", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label2.Location = new System.Drawing.Point(1, 1);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(532, 22);
            this.label2.TabIndex = 4;
            this.label2.Text = "단위 Device 제어 및 모니터링";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tabPage3
            // 
            this.tabPage3.Location = new System.Drawing.Point(4, 21);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(547, 548);
            this.tabPage3.TabIndex = 0;
            this.tabPage3.Text = "tabPage1";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // tabPage4
            // 
            this.tabPage4.Location = new System.Drawing.Point(4, 21);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(547, 548);
            this.tabPage4.TabIndex = 1;
            this.tabPage4.Text = "tabPage2";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // FormControlMain
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(825, 601);
            this.ControlBox = false;
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormControlMain";
            this.Text = "Conveyor Simulator";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.panel1.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Button buttonAlarmTest;
        private System.Windows.Forms.Button buttonInitialize;
        private System.Windows.Forms.Button buttonCycleTest;
        private System.Windows.Forms.Button buttonSpeedSlow;
        private System.Windows.Forms.Button buttonSpeedFast;
        private System.Windows.Forms.Button buttonSpeedNormal;
        private System.Windows.Forms.Button button1;
    }
}