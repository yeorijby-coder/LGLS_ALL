namespace ECP.Gui.Util
{
    partial class EventViewerForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EventViewerForm));
            this.eventTree = new System.Windows.Forms.TreeView();
            this.nodeImageList = new System.Windows.Forms.ImageList(this.components);
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.currectSubscriptionSubjectLabel = new System.Windows.Forms.Label();
            this.currentlySubscribedLabel = new System.Windows.Forms.Label();
            this.eventSubjectLabel = new System.Windows.Forms.Label();
            this.eventSubjectTextField = new System.Windows.Forms.TextBox();
            this.clearButton = new System.Windows.Forms.Button();
            this.pauseButton = new System.Windows.Forms.Button();
            this.applyButton = new System.Windows.Forms.Button();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // eventTree
            // 
            this.eventTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.eventTree.ImageIndex = 0;
            this.eventTree.ImageList = this.nodeImageList;
            this.eventTree.Location = new System.Drawing.Point(0, 0);
            this.eventTree.Name = "eventTree";
            this.eventTree.SelectedImageIndex = 0;
            this.eventTree.Size = new System.Drawing.Size(446, 395);
            this.eventTree.TabIndex = 0;
            // 
            // nodeImageList
            // 
            this.nodeImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("nodeImageList.ImageStream")));
            this.nodeImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.nodeImageList.Images.SetKeyName(0, "");
            this.nodeImageList.Images.SetKeyName(1, "RecordHS.png");
            // 
            // splitContainer
            // 
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 0);
            this.splitContainer.Name = "splitContainer";
            this.splitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.currectSubscriptionSubjectLabel);
            this.splitContainer.Panel1.Controls.Add(this.currentlySubscribedLabel);
            this.splitContainer.Panel1.Controls.Add(this.eventSubjectLabel);
            this.splitContainer.Panel1.Controls.Add(this.eventSubjectTextField);
            this.splitContainer.Panel1.Controls.Add(this.clearButton);
            this.splitContainer.Panel1.Controls.Add(this.pauseButton);
            this.splitContainer.Panel1.Controls.Add(this.applyButton);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.eventTree);
            this.splitContainer.Size = new System.Drawing.Size(446, 514);
            this.splitContainer.SplitterDistance = 115;
            this.splitContainer.TabIndex = 1;
            // 
            // currectSubscriptionSubjectLabel
            // 
            this.currectSubscriptionSubjectLabel.AutoSize = true;
            this.currectSubscriptionSubjectLabel.Location = new System.Drawing.Point(183, 49);
            this.currectSubscriptionSubjectLabel.Name = "currectSubscriptionSubjectLabel";
            this.currectSubscriptionSubjectLabel.Size = new System.Drawing.Size(13, 12);
            this.currectSubscriptionSubjectLabel.TabIndex = 2;
            this.currectSubscriptionSubjectLabel.Text = ">";
            // 
            // currentlySubscribedLabel
            // 
            this.currentlySubscribedLabel.AutoSize = true;
            this.currentlySubscribedLabel.Location = new System.Drawing.Point(38, 49);
            this.currentlySubscribedLabel.Name = "currentlySubscribedLabel";
            this.currentlySubscribedLabel.Size = new System.Drawing.Size(139, 12);
            this.currentlySubscribedLabel.TabIndex = 2;
            this.currentlySubscribedLabel.Text = "Current Event Subject : ";
            // 
            // eventSubjectLabel
            // 
            this.eventSubjectLabel.AutoSize = true;
            this.eventSubjectLabel.Location = new System.Drawing.Point(81, 20);
            this.eventSubjectLabel.Name = "eventSubjectLabel";
            this.eventSubjectLabel.Size = new System.Drawing.Size(82, 12);
            this.eventSubjectLabel.TabIndex = 2;
            this.eventSubjectLabel.Text = "Event Subject";
            // 
            // eventSubjectTextField
            // 
            this.eventSubjectTextField.Location = new System.Drawing.Point(181, 16);
            this.eventSubjectTextField.Name = "eventSubjectTextField";
            this.eventSubjectTextField.Size = new System.Drawing.Size(187, 21);
            this.eventSubjectTextField.TabIndex = 1;
            // 
            // clearButton
            // 
            this.clearButton.Location = new System.Drawing.Point(293, 75);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(81, 28);
            this.clearButton.TabIndex = 0;
            this.clearButton.Text = "Clear";
            this.clearButton.UseVisualStyleBackColor = true;
            this.clearButton.Click += new System.EventHandler(this.ClearButton_Click);
            // 
            // pauseButton
            // 
            this.pauseButton.Location = new System.Drawing.Point(189, 75);
            this.pauseButton.Name = "pauseButton";
            this.pauseButton.Size = new System.Drawing.Size(81, 28);
            this.pauseButton.TabIndex = 0;
            this.pauseButton.Text = "Pause";
            this.pauseButton.UseVisualStyleBackColor = true;
            this.pauseButton.Click += new System.EventHandler(this.PauseButton_Click);
            // 
            // applyButton
            // 
            this.applyButton.Location = new System.Drawing.Point(80, 75);
            this.applyButton.Name = "applyButton";
            this.applyButton.Size = new System.Drawing.Size(81, 28);
            this.applyButton.TabIndex = 0;
            this.applyButton.Text = "Apply";
            this.applyButton.UseVisualStyleBackColor = true;
            this.applyButton.Click += new System.EventHandler(this.applyButton_Click);
            // 
            // EventViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None ;
            this.ClientSize = new System.Drawing.Size(446, 514);
            this.Controls.Add(this.splitContainer);
            this.Name = "EventViewerForm";
            this.Text = "TestEventViewerForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TestEventViewerForm_FormClosing);
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel1.PerformLayout();
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView eventTree;
        private System.Windows.Forms.ImageList nodeImageList;
        private System.Windows.Forms.SplitContainer splitContainer;
        private System.Windows.Forms.Label currentlySubscribedLabel;
        private System.Windows.Forms.Label eventSubjectLabel;
        private System.Windows.Forms.TextBox eventSubjectTextField;
        private System.Windows.Forms.Button clearButton;
        private System.Windows.Forms.Button pauseButton;
        private System.Windows.Forms.Button applyButton;
        private System.Windows.Forms.Label currectSubscriptionSubjectLabel;        
    }
}