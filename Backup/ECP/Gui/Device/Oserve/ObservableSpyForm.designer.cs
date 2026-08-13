namespace ECP.Gui.Device.Oserve
{
    partial class ObservableSpyForm
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
            this.components = new System.ComponentModel.Container();
            this.observableSpy = new ECP.Gui.Device.Observe.ObservableViewControl();
            this.SuspendLayout();
            // 
            // observableSpy
            // 
            this.observableSpy.AccessMode = ECP.Gui.Device.Observe.ObservableViewAccessMode.Spy;
            this.observableSpy.Dock = System.Windows.Forms.DockStyle.Fill;
            this.observableSpy.HexMode = false;
            this.observableSpy.Location = new System.Drawing.Point(0, 0);
            this.observableSpy.Name = "observableSpy";
            this.observableSpy.NameWidth = 150;
            this.observableSpy.Observables = null;
            this.observableSpy.Size = new System.Drawing.Size(458, 769);
            this.observableSpy.TabIndex = 0;
            this.observableSpy.Text = "observableViewControl1";
            // 
            // ObservableSpyForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(458, 769);
            this.Controls.Add(this.observableSpy);
            this.Name = "ObservableSpyForm";
            this.Text = "Observable Spy";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ObservableSpyForm_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private ECP.Gui.Device.Observe.ObservableViewControl observableSpy;
    }
}