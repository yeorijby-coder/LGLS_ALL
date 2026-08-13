namespace HECS.Gui.Monitor.Panels.MoveRequest
{
    partial class MoveRequestAddPanel
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
            this.label1 = new System.Windows.Forms.Label();
            this.labelCommandid = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonMakeNo = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxTosublocation = new System.Windows.Forms.TextBox();
            this.textBoxFromsublocation = new System.Windows.Forms.TextBox();
            this.textBoxPalletId = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.comboBoxFromposition = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.comboBoxToposition = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.comboBoxFromdevice = new System.Windows.Forms.ComboBox();
            this.comboBoxTodevice = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.comboBoxFromlocation = new System.Windows.Forms.ComboBox();
            this.comboBoxTolocation = new System.Windows.Forms.ComboBox();
            this.textBoxMaterialno = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxQty = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBoxUnit = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("돋움", 9.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.ForeColor = System.Drawing.Color.Blue;
            this.label1.Location = new System.Drawing.Point(13, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(156, 15);
            this.label1.TabIndex = 9;
            this.label1.Text = "이동 반송 등록";
            // 
            // labelCommandid
            // 
            this.labelCommandid.BackColor = System.Drawing.Color.PowderBlue;
            this.labelCommandid.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelCommandid.Font = new System.Drawing.Font("돋움", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelCommandid.ForeColor = System.Drawing.Color.Blue;
            this.labelCommandid.Location = new System.Drawing.Point(177, 29);
            this.labelCommandid.Name = "labelCommandid";
            this.labelCommandid.Size = new System.Drawing.Size(207, 24);
            this.labelCommandid.TabIndex = 14;
            this.labelCommandid.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label3.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label3.ForeColor = System.Drawing.Color.Black;
            this.label3.Location = new System.Drawing.Point(19, 29);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(156, 24);
            this.label3.TabIndex = 13;
            this.label3.Text = "요청 번호";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonMakeNo
            // 
            this.buttonMakeNo.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.buttonMakeNo.Location = new System.Drawing.Point(390, 29);
            this.buttonMakeNo.Name = "buttonMakeNo";
            this.buttonMakeNo.Size = new System.Drawing.Size(154, 24);
            this.buttonMakeNo.TabIndex = 16;
            this.buttonMakeNo.Text = "이동 요청 번호 생성";
            this.buttonMakeNo.UseVisualStyleBackColor = true;
            this.buttonMakeNo.Click += new System.EventHandler(this.buttonMakeNo_Click);
            // 
            // label4
            // 
            this.label4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label4.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label4.ForeColor = System.Drawing.Color.Black;
            this.label4.Location = new System.Drawing.Point(389, 175);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(156, 24);
            this.label4.TabIndex = 13;
            this.label4.Text = "도착 세부 Location 번호";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label6
            // 
            this.label6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label6.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label6.ForeColor = System.Drawing.Color.Black;
            this.label6.Location = new System.Drawing.Point(19, 175);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(156, 24);
            this.label6.TabIndex = 13;
            this.label6.Text = "출발 세부 Location 번호";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label8
            // 
            this.label8.AccessibleDescription = "";
            this.label8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label8.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label8.ForeColor = System.Drawing.Color.Black;
            this.label8.Location = new System.Drawing.Point(388, 53);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(156, 24);
            this.label8.TabIndex = 13;
            this.label8.Text = "Pallet Id";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxTosublocation
            // 
            this.textBoxTosublocation.Location = new System.Drawing.Point(547, 177);
            this.textBoxTosublocation.Name = "textBoxTosublocation";
            this.textBoxTosublocation.Size = new System.Drawing.Size(207, 21);
            this.textBoxTosublocation.TabIndex = 17;
            // 
            // textBoxFromsublocation
            // 
            this.textBoxFromsublocation.Location = new System.Drawing.Point(177, 177);
            this.textBoxFromsublocation.Name = "textBoxFromsublocation";
            this.textBoxFromsublocation.Size = new System.Drawing.Size(207, 21);
            this.textBoxFromsublocation.TabIndex = 17;
            // 
            // textBoxPalletId
            // 
            this.textBoxPalletId.Location = new System.Drawing.Point(546, 55);
            this.textBoxPalletId.Name = "textBoxPalletId";
            this.textBoxPalletId.Size = new System.Drawing.Size(207, 21);
            this.textBoxPalletId.TabIndex = 17;
            // 
            // label5
            // 
            this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label5.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(19, 103);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(156, 24);
            this.label5.TabIndex = 13;
            this.label5.Text = "출발 위치";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // comboBoxFromposition
            // 
            this.comboBoxFromposition.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFromposition.FormattingEnabled = true;
            this.comboBoxFromposition.Location = new System.Drawing.Point(177, 105);
            this.comboBoxFromposition.Name = "comboBoxFromposition";
            this.comboBoxFromposition.Size = new System.Drawing.Size(206, 20);
            this.comboBoxFromposition.TabIndex = 15;
            this.comboBoxFromposition.SelectedIndexChanged += new System.EventHandler(this.comboBoxFromposition_SelectedIndexChanged);
            // 
            // label7
            // 
            this.label7.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label7.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label7.ForeColor = System.Drawing.Color.Black;
            this.label7.Location = new System.Drawing.Point(389, 103);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(156, 24);
            this.label7.TabIndex = 13;
            this.label7.Text = "도착 위치";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // comboBoxToposition
            // 
            this.comboBoxToposition.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxToposition.FormattingEnabled = true;
            this.comboBoxToposition.Location = new System.Drawing.Point(547, 105);
            this.comboBoxToposition.Name = "comboBoxToposition";
            this.comboBoxToposition.Size = new System.Drawing.Size(206, 20);
            this.comboBoxToposition.TabIndex = 15;
            this.comboBoxToposition.SelectedIndexChanged += new System.EventHandler(this.comboBoxToposition_SelectedIndexChanged);
            // 
            // label11
            // 
            this.label11.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label11.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label11.ForeColor = System.Drawing.Color.Black;
            this.label11.Location = new System.Drawing.Point(19, 127);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(156, 24);
            this.label11.TabIndex = 13;
            this.label11.Text = "출발 장비";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label12
            // 
            this.label12.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label12.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label12.ForeColor = System.Drawing.Color.Black;
            this.label12.Location = new System.Drawing.Point(389, 127);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(156, 24);
            this.label12.TabIndex = 13;
            this.label12.Text = "도착 장비";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // comboBoxFromdevice
            // 
            this.comboBoxFromdevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFromdevice.FormattingEnabled = true;
            this.comboBoxFromdevice.Location = new System.Drawing.Point(177, 129);
            this.comboBoxFromdevice.Name = "comboBoxFromdevice";
            this.comboBoxFromdevice.Size = new System.Drawing.Size(206, 20);
            this.comboBoxFromdevice.TabIndex = 15;
            this.comboBoxFromdevice.SelectedIndexChanged += new System.EventHandler(this.comboBoxFromdevice_SelectedIndexChanged);
            // 
            // comboBoxTodevice
            // 
            this.comboBoxTodevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTodevice.FormattingEnabled = true;
            this.comboBoxTodevice.Location = new System.Drawing.Point(547, 129);
            this.comboBoxTodevice.Name = "comboBoxTodevice";
            this.comboBoxTodevice.Size = new System.Drawing.Size(206, 20);
            this.comboBoxTodevice.TabIndex = 15;
            this.comboBoxTodevice.SelectedIndexChanged += new System.EventHandler(this.comboBoxTodevice_SelectedIndexChanged);
            // 
            // label13
            // 
            this.label13.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label13.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label13.ForeColor = System.Drawing.Color.Black;
            this.label13.Location = new System.Drawing.Point(19, 151);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(156, 24);
            this.label13.TabIndex = 13;
            this.label13.Text = "출발 Location";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label14
            // 
            this.label14.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label14.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label14.ForeColor = System.Drawing.Color.Black;
            this.label14.Location = new System.Drawing.Point(389, 151);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(156, 24);
            this.label14.TabIndex = 13;
            this.label14.Text = "도착 Location";
            this.label14.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // comboBoxFromlocation
            // 
            this.comboBoxFromlocation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFromlocation.FormattingEnabled = true;
            this.comboBoxFromlocation.Location = new System.Drawing.Point(177, 153);
            this.comboBoxFromlocation.Name = "comboBoxFromlocation";
            this.comboBoxFromlocation.Size = new System.Drawing.Size(206, 20);
            this.comboBoxFromlocation.TabIndex = 15;
            this.comboBoxFromlocation.SelectedIndexChanged += new System.EventHandler(this.comboBoxFromlocation_SelectedIndexChanged);
            // 
            // comboBoxTolocation
            // 
            this.comboBoxTolocation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTolocation.FormattingEnabled = true;
            this.comboBoxTolocation.Location = new System.Drawing.Point(547, 153);
            this.comboBoxTolocation.Name = "comboBoxTolocation";
            this.comboBoxTolocation.Size = new System.Drawing.Size(206, 20);
            this.comboBoxTolocation.TabIndex = 15;
            this.comboBoxTolocation.SelectedIndexChanged += new System.EventHandler(this.comboBoxTolocation_SelectedIndexChanged);
            // 
            // textBoxMaterialno
            // 
            this.textBoxMaterialno.Location = new System.Drawing.Point(177, 55);
            this.textBoxMaterialno.Name = "textBoxMaterialno";
            this.textBoxMaterialno.Size = new System.Drawing.Size(207, 21);
            this.textBoxMaterialno.TabIndex = 19;
            // 
            // label2
            // 
            this.label2.AccessibleDescription = "";
            this.label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label2.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(19, 53);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(156, 24);
            this.label2.TabIndex = 18;
            this.label2.Text = "자재 번호";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxQty
            // 
            this.textBoxQty.Location = new System.Drawing.Point(177, 79);
            this.textBoxQty.Name = "textBoxQty";
            this.textBoxQty.Size = new System.Drawing.Size(207, 21);
            this.textBoxQty.TabIndex = 21;
            // 
            // label10
            // 
            this.label10.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label10.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label10.ForeColor = System.Drawing.Color.Black;
            this.label10.Location = new System.Drawing.Point(19, 77);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(156, 24);
            this.label10.TabIndex = 20;
            this.label10.Text = "수량";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBoxUnit
            // 
            this.textBoxUnit.Location = new System.Drawing.Point(546, 80);
            this.textBoxUnit.Name = "textBoxUnit";
            this.textBoxUnit.Size = new System.Drawing.Size(207, 21);
            this.textBoxUnit.TabIndex = 23;
            // 
            // label9
            // 
            this.label9.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label9.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label9.ForeColor = System.Drawing.Color.Black;
            this.label9.Location = new System.Drawing.Point(388, 78);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(156, 24);
            this.label9.TabIndex = 22;
            this.label9.Text = "단위";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // MoveRequestAddPanel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.textBoxUnit);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.textBoxQty);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.textBoxMaterialno);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBoxPalletId);
            this.Controls.Add(this.textBoxFromsublocation);
            this.Controls.Add(this.textBoxTosublocation);
            this.Controls.Add(this.buttonMakeNo);
            this.Controls.Add(this.comboBoxTolocation);
            this.Controls.Add(this.comboBoxTodevice);
            this.Controls.Add(this.comboBoxFromlocation);
            this.Controls.Add(this.comboBoxFromdevice);
            this.Controls.Add(this.comboBoxToposition);
            this.Controls.Add(this.comboBoxFromposition);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.labelCommandid);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Name = "MoveRequestAddPanel";
            this.Size = new System.Drawing.Size(763, 209);
            this.Load += new System.EventHandler(this.MoveRequestAddPanel_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        protected System.Windows.Forms.Label label1;
        protected System.Windows.Forms.Label labelCommandid;
        protected System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonMakeNo;
        protected System.Windows.Forms.Label label4;
        protected System.Windows.Forms.Label label6;
        protected System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxTosublocation;
        private System.Windows.Forms.TextBox textBoxFromsublocation;
        private System.Windows.Forms.TextBox textBoxPalletId;
        protected System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboBoxFromposition;
        protected System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox comboBoxToposition;
        protected System.Windows.Forms.Label label11;
        protected System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox comboBoxFromdevice;
        private System.Windows.Forms.ComboBox comboBoxTodevice;
        protected System.Windows.Forms.Label label13;
        protected System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox comboBoxFromlocation;
        private System.Windows.Forms.ComboBox comboBoxTolocation;
        private System.Windows.Forms.TextBox textBoxMaterialno;
        protected System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxQty;
        protected System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBoxUnit;
        protected System.Windows.Forms.Label label9;

    }
}
