using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Event;



using ECP.Service.Statistics;
using HECS.Util;


namespace HECS.Gui.Monitor.Popups
{


    public partial class MessageDialog : DialogBaseForm
    {
        System.Timers.Timer timer = new System.Timers.Timer();
        bool isDialogResult = false;
        int tick = 0;
        DialogResult ret = DialogResult.Cancel;
        private string messageData = "";
        public string DisplayMessage
        {
            get
            {
                return messageData;
            }
            set
            {
                isDialogResult = false;
                messageData = value;
                UpdateDisplayText();
            }
        }
        public MessageDialog()
        {
            InitializeComponent();
            timer.Interval = 500;
            timer.Elapsed += new System.Timers.ElapsedEventHandler(timer_Elapsed);
        }

        public DialogResult DisplayDialog(string message)
        {
            isDialogResult = true;
            messageData = message;
            UpdateDisplayText();
            ShowDialog();
            return ret;
        }
        private delegate void delegateUpdateWidget();
        public void UpdateDisplayText()
        {
            delegateUpdateWidget update = new delegateUpdateWidget(UpdateDialog);
            update.Invoke();
        }
        public void FormHide()
        {
            delegateUpdateWidget update = new delegateUpdateWidget(HideForm);
            update.Invoke();
        }

        private void HideForm()
        {
            Hide();
        }
        private void UpdateDialog()
        {
            this.txtDesciption.Text = messageData;
            this.txtDesciption.Visible = true;
            timer.Enabled = true;
            tick = 0;
            if (isDialogResult == true)
            {
                buttonYes.Visible = true;
                buttonNo.Visible = true;
                buttonOK.Visible = false;
            }
            else
            {
                buttonYes.Visible = false;
                buttonNo.Visible = false;
                buttonOK.Visible = true;
                Show();
                //ShowDialog();
            }
            Invalidate();
        }



        void timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            tick = tick + 1;
            if (tick >= 120)
            {
                tick = 0;
                timer.Enabled = false;
                FormHide();
            }
        }





        private void buttonOK_Click(object sender, EventArgs e)
        {
            Hide();
        }

        private void buttonYes_Click(object sender, EventArgs e)
        {
            ret = DialogResult.OK;
            Hide();
        }

        private void buttonNo_Click(object sender, EventArgs e)
        {
            ret = DialogResult.Cancel;
            Hide();
        }


    }


}

