using ECP.Gui.Device.Observe;

namespace ECP.Emulator.FieldBus
{
    partial class SharedMemoryForm
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
            this.observableView = new ECP.Gui.Device.Observe.ObservableViewControl();
            this.SuspendLayout();
            // 
            // observableView
            // 
            this.observableView.AccessMode = ECP.Gui.Device.Observe.ObservableViewAccessMode.Spy;
            this.observableView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.observableView.HexMode = false;
            this.observableView.Location = new System.Drawing.Point(0, 0);
            this.observableView.Name = "observableView";
            this.observableView.NameWidth = 220;
            this.observableView.Observables = null;
            this.observableView.Size = new System.Drawing.Size(458, 769);
            this.observableView.TabIndex = 0;
            this.observableView.Text = "observableView";
            // 
            // SharedMemoryForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(458, 769);
            this.Controls.Add(this.observableView);
            this.Name = "SharedMemoryForm";
            this.Text = "SharedMemoryForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.SharedMemoryForm_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private global::ECP.Gui.Device.Observe.ObservableViewControl observableView;
    }
}